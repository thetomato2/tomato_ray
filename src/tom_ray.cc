#define PBSTR   "############################################################"
#define PBWIDTH 60

namespace tom
{

fn void print_progress(f32 percentage)
{
    i32 val  = (int)(percentage * 100);
    i32 lpad = (int)(percentage * PBWIDTH);
    i32 rpad = PBWIDTH - lpad;
    printf("\rRendering: %3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

fn void print_finished()
{
    print_progress(1.0f);
    printf("\nFinished!");
}

fn f32 exact_linear_to_sRGB(f32 L)
{
    if (L < 0.0f) {
        L = 0.0f;
    }

    if (L > 1.0f) {
        L = 1.0f;
    }

    f32 S = L * 12.92f;
    if (L > 0.0031308f) {
        S = 1.055f * pow(L, 1.0f / 2.4f) - 0.055f;
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

    for (u32 ray_cnt = 0; ray_cnt < 8; ++ray_cnt) {
        f32 hit_dist  = F32_MAX;
        u32 hit_mat_i = 0;
        v3f next_nrm  = {};

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

fn void ray_render(AppState *app)
{
    v3f up = { 0.0f, 0.0f, 1.0f };

    Material materials[6]    = {};
    materials[0].emit_col    = { 0.2f, 0.4f, 0.8f };
    materials[1].reflect_col = { 0.2f, 0.6f, 0.2f };
    materials[2].emit_col    = { 4.0f, 0.0f, 0.0f };
    materials[2].reflect_col = { 1.0f, 0.0f, 0.0f };
    materials[2].scatter     = 0.2f;
    materials[3].reflect_col = { 0.9f, 0.9f, 0.9f };
    materials[3].scatter     = 0.7f;
    materials[4].reflect_col = { 0.0f, 1.0f, 0.0f };
    materials[4].scatter     = 1.0f;
    materials[5].reflect_col = { 0.0f, 0.0f, 1.0f };
    materials[5].scatter     = 1.0f;

    Plane plane = {};
    plane.n     = { 0.0f, 0.0f, 1.0f };
    plane.d     = 0;
    plane.mat_i = 1;

    Sphere spheres[4] = {};
    spheres[0].r      = 1.5f;
    spheres[0].mat_i  = 3;
    spheres[1].p      = { -3, 1, 1 };
    spheres[1].r      = 1.0f;
    spheres[1].mat_i  = 2;
    spheres[2].p      = { 3, 1, 2 };
    spheres[2].r      = 1.2f;
    spheres[2].mat_i  = 4;
    spheres[3].p      = { 0.3f, 1, 2 };
    spheres[3].r      = 0.9f;
    spheres[3].mat_i  = 5;

    World world      = {};
    world.mat_cnt    = CountOf(materials);
    world.mats       = materials;
    world.plane_cnt  = 1;
    world.planes     = &plane;
    world.sphere_cnt = CountOf(spheres);
    world.spheres    = spheres;

    Camera cam;
    cam.p = { 0, -10, 1 };
    cam.z = vec_noz(cam.p);
    cam.x = vec_noz(vec_cross(cam.z, up));
    cam.y = vec_noz(vec_cross(cam.z, cam.x));

    f32 film_dist = 1.0f;
    v3f film_cen  = cam.p - film_dist * cam.z;
    v2f film_dims = { 1.0f, 1.0f };
    if (app->back_buffer.width > app->back_buffer.height) {
        film_dims.h = film_dims.w * (f32)app->back_buffer.height / (f32)app->back_buffer.width;
    } else if (app->back_buffer.height > app->back_buffer.width) {
        film_dims.w = film_dims.h * (f32)app->back_buffer.width / (f32)app->back_buffer.height;
    }
    v2f half_film_dims = 0.5f * film_dims;
    i64 start_counter  = get_time();
    u32 rays_per_pix   = 16;

    Color_u32 clr_col = { 0xfff00000 };
    auto buf_ptr      = (u32 *)app->back_buffer.buf;
    for (i32 y = 0; y < app->back_buffer.height; ++y) {
        f32 fy = -1.0f + 2.0f * ((f32)y / (f32)app->back_buffer.height);
        // fy = -fy; // flip the image
        for (i32 x = 0; x < app->back_buffer.width; ++x) {
            f32 fx       = -1.0f + 2.0f * ((f32)x / (f32)app->back_buffer.width);
            v3f film_p   = film_cen + fx * half_film_dims.w * cam.x + fy * half_film_dims.h * cam.y;
            v3f ray_orig = cam.p;
            v3f ray_dir  = vec_noz(film_p - cam.p);
            f32 ray_contrib = 1.0f / (f32)rays_per_pix;
            v3f col         = {};
            for (u32 ray_i = 0; ray_i < rays_per_pix; ++ray_i) {
                col += ray_contrib * ray_cast(&world, ray_orig, ray_dir);
            }
            col = exact_linear_to_sRGB(col);

            buf_ptr[y * app->back_buffer.width + x] = v3f_to_color_u32(col).rgba;
        }
        if (y % 64 == 0) print_progress((f32)y / app->back_buffer.height);
    }
    i64 end_counter = get_time();
    f32 seconds_elapsed =
        get_seconds_elapsed(start_counter, end_counter, app->performance_counter_frequency);
    print_finished();
    printf(" - %f seconds\n", seconds_elapsed);
}
}  // namespace tom