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
    u32 mat_cnt;
    Material *mats;

    u32 plane_cnt;
    Plane *planes;

    u32 sphere_cnt;
    Sphere *spheres;
};

}  // namespace tom