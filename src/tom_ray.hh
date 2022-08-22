namespace tom
{

struct Material
{
    v3f emit_col;
    v3f reflect_col;
    f32 scatter;  // NOTE: 0 is diffuse, 1 is mirror
                  // TODO: is this not roughness in Blender??
};

struct Plane
{
    v3f n;
    f32 d;
    u32 mat_i;
};

struct Sphere
{
    v3f p;
    f32 r;
    u32 mat_i;
};

struct Camera
{
    v3f p, x, y, z;
};

struct World
{
    v3f up;
    Camera cam;

    u32 mat_cnt;
    Material *mats;

    u32 plane_cnt;
    Plane *planes;

    u32 sphere_cnt;
    Sphere *spheres;

    u64 ray_cnt;
    u64 bounce_cnt;
};

struct RayWorkOrder
{
    World *world;
    Texture *image;
    r2u tile;
};

struct RayWorkQueue
{
    u32 work_order_cnt;
    RayWorkOrder *work_orders;

    volatile u32 next_work_order_i;
};

struct RayState
{
    b32 initialized;
    b32 rendering;
    b32 render_finished;
    Material materials[6];
    Plane plane;
    Sphere spheres[4];
    World world;
    u32 cpu_core_cnt;
    u32 tile_w;
    u32 tile_h;
    u32 tile_cnt_x;
    u32 tile_cnt_y;
    s64 start_counter;
    
    RayWorkQueue work_queue;
};


}  // namespace tom