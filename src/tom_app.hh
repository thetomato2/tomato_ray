
#include "tom_core.hh"
#include "tom_intrinsic.hh"
#include "tom_math.hh"
#include "tom_color.hh"
#include "tom_memory.hh"
#include "tom_string.hh"
#include "tom_time.hh"
#include "tom_file_io.hh"
#include "tom_texture.hh"
#include "tom_input.hh"
#include "tom_win32.hh"
#include "tom_d3d11.hh"
#include "tom_dx_error.hh"
// #include "tom_font.hh"
#include "tom_ray.hh"

namespace tom
{

struct AppMemory
{
    u64 permanent_storage_size;
    void* permanent_storage;  // NOTE: required to be cleared to 0!
    u64 transient_storage_size;
    void* transient_storage;  // NOTE: required to be cleared to 0!
};

struct AppState
{
    AppMemory memory;
    void* memory_block;
    szt total_size;
    Win32State win32;
    D3D11State d3d11;
    Input input;
    Texture back_buffer;
    u32 update_hertz;
    u32 dpi;
    f32 target_secs_per_frame;
    i64 performance_counter_frequency;
    u64 frame_cnt;
    u32 work_ind;
    f32 work_secs[256];
    f32 ms_frame;
    i32 fps;
    i32 target_fps;
    char exe_path[MAX_PATH];
    f32 dt;
    f32 time;
    f32 fov;

    f32 device_changed_delay;
    bool suspend_lost_focus;
    bool draw_frame;

    D3D11ShaderProg main_shader;
};
}  // namespace tom