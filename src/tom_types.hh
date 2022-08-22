#include <cstdint>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef size_t szt;
typedef unsigned char byt;
// using byt  u8; // TODO: is this different?

typedef int32_t b32;
typedef wchar_t wchar;

typedef uintptr_t umm;
typedef intptr_t smm;

#define U8_MIN 0u
#define U8_MAX 0xffu
#define I8_MIN (-0x7f - 1)
#define I8_MAX 0x7f

#define U16_MIN 0u
#define U16_MAX 0xffffu
#define I16_MIN (-0x7fff - 1)
#define I16_MAX 0x7fff

#define U32_MIN 0u
#define U32_MAX 0xffffffffu
#define I32_MIN (-0x7fffffff - 1)
#define I32_MAX 0x7fffffff

#define U64_MIN 0ull
#define U64_MAX 0xffffffffffffffffull
#define I64_MIN (-0x7fffffffffffffffll - 1)
#define I64_MAX 0x7fffffffffffffffll

#define F32_MIN 1.175494351e-38f
#define F32_MAX 3.402823466e+38f
#define F64_MIN 2.2250738585072014e-308
#define F64_MAX 1.7976931348623158e+308

#define Bit(x) (1 << x)

#define Kilobytes(val) ((val)*1024)
#define Megabytes(val) (Kilobytes(val) * 1024)
#define Gigabytes(val) (Megabytes(val) * 1024)
#define Terabytes(val) (Gigabytes(val) * 1024)

template<typename T>
union v2
{
    struct
    {
        T x, y;
    };
    struct
    {
        T u, v;
    };
    struct
    {
        T w, h;
    };
    T e[2];
};

typedef v2<f32> v2f;
typedef v2<s32> v2i;
typedef v2<u32> v2u;

template<typename T>
union v3
{
    T e[3];
    struct
    {
        T x, y, z;
    };
    struct
    {
        T r, g, b;
    };
    v2<T> xy;
};

typedef v3<f32> v3f;
typedef v3<s32> v3i;
typedef v3<u32> v3u;

template<typename T>
union v4
{
    T e[4];
    struct
    {
        T x, y, z, w;
    };
    struct
    {
        T r, g, b, a;
    };
    v3<T> xyz;
    v3<T> rgb;
};

typedef v4<f32> v4f;
typedef v4<s32> v4i;
typedef v4<u32> v4u;

union m3
{
    f32 e[9];
    f32 m[3][3];
    v3f r[3];
};

union m4
{
    f32 e[16];
    f32 m[4][4];
    v4f r[4];
};

template<typename T>
union r2
{
    struct
    {
        T x0, y0, x1, y1;
    };
    struct
    {
        v2<T> min;
        v2<T> max;
    };
};

typedef r2<f32> r2f;
typedef r2<s32> r2s;
typedef r2<u32> r2u;

template<typename T>
union r3
{
    struct
    {
        T x0, y0, z0, x1, y1, z1;
    };
    struct
    {
        v3<T> min;
        v3<T> max;
    };
};

typedef r3<f32> r3f;
typedef r3<s32> r3s;
typedef r3<u32> r3u;

struct Button
{
    s32 half_transition_cnt;
    b32 ended_down;
};

struct Camera
{
    v2f dims;
    v2f pos;
};