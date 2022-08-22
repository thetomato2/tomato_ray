#define PBSTR   "############################################################"
#define PBWIDTH 60

namespace tom
{

fn void print_progress(f32 percentage)
{
    s32 val  = (int)(percentage * 100);
    s32 lpad = (int)(percentage * PBWIDTH);
    s32 rpad = PBWIDTH - lpad;
    printf("\rRendering: %3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

fn void print_finished()
{
    print_progress(1.0f);
    printf("\nFinished!");
}

fn f32 exact_linear_to_sRGB(f32 c)
{
    if (c < 0.0f) {
        c = 0.0f;
    }

    if (c > 1.0f) {
        c = 1.0f;
    }

    f32 S = c * 12.92f;
    if (c > 0.0031308f) {
        S = 1.055f * pow(c, 1.0f / 2.4f) - 0.055f;
    }

    return (S);
}

fn v3f exact_linear_to_sRGB(v3f col)
{
    v3f result;
    result.r = exact_linear_to_sRGB(col.r);
    result.g = exact_linear_to_sRGB(col.g);
    result.b = exact_linear_to_sRGB(col.b);

    return result;
}

fn f32 rand_uni()
{
    // TODO: replace with better rand function
    return (f32)rand() / (f32)RAND_MAX;
}

fn f32 rand_bi()
{
    return 2.0f * rand_uni();
}

fn v3f ray_cast(World *world, v3f ray_orig, v3f ray_dir)
{
    v3f result       = {};
    f32 min_hit_dist = 0.001f;
    v3f attenuation  = { 1, 1, 1 };

    for (u32 bounce = 0; bounce < 8; ++bounce) {
        f32 hit_dist  = F32_MAX;
        u32 hit_mat_i = 0;
        v3f next_nrm  = {};

        ++world->bounce_cnt;

        // plane
        for (u32 plane_i = 0; plane_i < world->plane_cnt; ++plane_i) {
            Plane plane = world->planes[plane_i];
            f32 denom   = vec_dot(plane.n, ray_dir);
            if (abs_f32(denom) > EPS_F32) {
                f32 t = (-plane.d - vec_dot(plane.n, ray_orig)) / denom;
                if (t > min_hit_dist && t < hit_dist) {
                    hit_dist  = t;
                    hit_mat_i = plane.mat_i;

                    next_nrm = plane.n;
                }
            }
        }

        // sphere
        for (u32 sphere_i = 0; sphere_i < world->sphere_cnt; ++sphere_i) {
            Sphere sphere = world->spheres[sphere_i];
            v3f oc        = ray_orig - sphere.p;
            f32 a         = vec_length_sq(ray_dir);
            f32 b         = vec_dot(oc, ray_dir);  // half b
            f32 c         = vec_length_sq(oc) - square(sphere.r);
            f32 discrim   = square(b) - a * c;

            if (discrim > EPS_F32) {
                f32 tp = (-b + sqrt(discrim)) / a;
                f32 tn = (-b - sqrt(discrim)) / a;

                f32 t = tp;
                if (tn > min_hit_dist && tn < tp) t = tn;
                if (t > min_hit_dist && t < hit_dist) {
                    hit_dist  = t;
                    hit_mat_i = sphere.mat_i;

                    next_nrm = vec_noz(t * ray_dir + oc);
                }
            }
        }

        if (hit_mat_i) {
            Material mat = world->mats[hit_mat_i];
            result += vec_hadamard(attenuation, mat.emit_col);
#if 0
            f32 cos_atten = 1.0f;
#else
            f32 cos_atten = max(vec_dot(-ray_dir, next_nrm), 0.0f);
#endif
            attenuation = vec_hadamard(attenuation, cos_atten * mat.reflect_col);

            ray_orig += hit_dist * ray_dir;
            v3f pure_bounce = vec_reflect(ray_dir, next_nrm);
            v3f rand_bounce = vec_noz(next_nrm + v3f { rand_bi(), rand_bi(), rand_bi() });
            ray_dir         = lerp(rand_bounce, pure_bounce, mat.scatter);

        } else {
            Material mat = world->mats[hit_mat_i];
            result += vec_hadamard(attenuation, mat.emit_col);

            break;
        }
    }

    return result;
}

fn void ray_render_tile(RayWorkOrder *wo)
{
    f32 film_dist = 1.0f;
    v3f film_cen  = wo->world->cam.p - film_dist * wo->world->cam.z;
    v2f film_dims = { 1.0f, 1.0f };
    if (wo->image->width > wo->image->height) {
        film_dims.h = film_dims.w * (f32)wo->image->height / (f32)wo->image->width;
    } else if (wo->image->height > wo->image->width) {
        film_dims.w = film_dims.h * (f32)wo->image->width / (f32)wo->image->height;
    }
    v2f half_film_dims = 0.5f * film_dims;
    s64 start_counter  = get_time();
    u32 rays_per_pix   = 16;

    Color_u32 clr_col = { 0xfff00000 };
    auto buf_ptr      = (u32 *)wo->image->buf;
    for (s32 y = wo->tile.y0; y < wo->tile.y1; ++y) {
        f32 fy = -1.0f + 2.0f * ((f32)y / (f32)wo->image->height);
        // fy = -fy; // flip the image
        for (s32 x = wo->tile.x0; x < wo->tile.x1; ++x) {
            f32 fx     = -1.0f + 2.0f * ((f32)x / (f32)wo->image->width);
            v3f film_p = film_cen + fx * half_film_dims.w * wo->world->cam.x +
                         fy * half_film_dims.h * wo->world->cam.y;
            v3f ray_orig    = wo->world->cam.p;
            v3f ray_dir     = vec_noz(film_p - wo->world->cam.p);
            f32 ray_contrib = 1.0f / (f32)rays_per_pix;
            v3f col         = {};
            for (u32 ray_i = 0; ray_i < rays_per_pix; ++ray_i) {
                ++wo->world->ray_cnt;
                col += ray_contrib * ray_cast(wo->world, ray_orig, ray_dir);
            }
            col = exact_linear_to_sRGB(col);

            buf_ptr[y * wo->image->width + x] = v3f_to_color_u32(col).rgba;
        }
    }
}

fn void ray_init(AppState *app)
{
    RayState *ray = &app->ray;

    ray->materials[0].emit_col    = { 0.2f, 0.4f, 0.8f };
    ray->materials[1].reflect_col = { 0.2f, 0.6f, 0.2f };
    ray->materials[2].emit_col    = { 4.0f, 0.0f, 0.0f };
    ray->materials[2].reflect_col = { 1.0f, 0.0f, 0.0f };
    ray->materials[2].scatter     = 0.2f;
    ray->materials[3].reflect_col = { 0.9f, 0.9f, 0.9f };
    ray->materials[3].scatter     = 0.7f;
    ray->materials[4].reflect_col = { 0.0f, 1.0f, 0.0f };
    ray->materials[4].scatter     = 1.0f;
    ray->materials[5].reflect_col = { 0.0f, 0.0f, 1.0f };
    ray->materials[5].scatter     = 1.0f;

    ray->plane.n     = { 0.0f, 0.0f, 1.0f };
    ray->plane.d     = 0;
    ray->plane.mat_i = 1;

    ray->spheres[0].r     = 1.5f;
    ray->spheres[0].mat_i = 3;
    ray->spheres[1].p     = { -3, 1, 1 };
    ray->spheres[1].r     = 1.0f;
    ray->spheres[1].mat_i = 2;
    ray->spheres[2].p     = { 3, 1, 2 };
    ray->spheres[2].r     = 1.2f;
    ray->spheres[2].mat_i = 4;
    ray->spheres[3].p     = { 0.3f, 1, 2 };
    ray->spheres[3].r     = 0.9f;
    ray->spheres[3].mat_i = 5;

    ray->world.mat_cnt    = CountOf(ray->materials);
    ray->world.mats       = ray->materials;
    ray->world.plane_cnt  = 1;
    ray->world.planes     = &ray->plane;
    ray->world.sphere_cnt = CountOf(ray->spheres);
    ray->world.spheres    = ray->spheres;

    ray->world.up = { 0.0f, 0.0f, 1.0f };

    ray->world.cam.p = { 0, -10, 1 };
    ray->world.cam.z = vec_noz(ray->world.cam.p);
    ray->world.cam.x = vec_noz(vec_cross(ray->world.cam.z, ray->world.up));
    ray->world.cam.y = vec_noz(vec_cross(ray->world.cam.z, ray->world.cam.x));

    ray->cpu_core_cnt = 8;
    ray->tile_w       = (app->back_buffer.width - 1) / ray->cpu_core_cnt;
    ray->tile_h       = ray->tile_w;
    ray->tile_cnt_x   = (app->back_buffer.width + ray->tile_w - 1) / ray->tile_w;
    ray->tile_cnt_y   = (app->back_buffer.height + ray->tile_h - 1) / ray->tile_h;

    ray->work_queue.work_order_cnt   = ray->tile_cnt_x * ray->tile_cnt_y;
    ray->work_queue.work_orders      = plat_malloc<RayWorkOrder>(ray->work_queue.work_order_cnt);
    ray->work_queue.next_work_order_i = 0;

    for (u32 tile_y = 0; tile_y < ray->tile_cnt_y; ++tile_y) {
        for (u32 tile_x = 0; tile_x < ray->tile_cnt_x; ++tile_x) {
            RayWorkOrder *wo = &ray->work_queue.work_orders[tile_y * ray->tile_cnt_x + tile_x];
            wo->world        = &ray->world;
            wo->image        = &app->back_buffer;

            wo->tile.x0 = tile_x * ray->tile_w;
            wo->tile.y0 = tile_y * ray->tile_h;
            wo->tile.x1 = min((u32)app->back_buffer.width, wo->tile.x0 + ray->tile_w);
            wo->tile.y1 = min((u32)app->back_buffer.height, wo->tile.y0 + ray->tile_w);
        }
    }
}

}  // namespace tom