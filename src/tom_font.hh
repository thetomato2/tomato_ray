// ============================================================================================
// Hard-coded to work with one simple bitmap image
// TODO: all the font stuff
// ============================================================================================

namespace tom
{

#define MAX_GLYPH_CNT 256

struct ttf_GlyphResult
{
    s32 width, height, x_off, y_off;
    void* bitmap;

    void free()
    {
        width  = 0;
        height = 0;
        x_off  = 0;
        y_off  = 0;
        stbtt_FreeBitmap((byt*)bitmap, 0);
    };

    operator bool() const { return bitmap != nullptr; }
};

struct Glyph
{
    s32 x_off, y_off;  // top left corner
    s32 width, height;
};

struct FontSheet
{
    const char* name;
    u32 glyph_cnt;
    s32 width, height;  // pixels
    byt* bitmap;
    Glyph glyphs[MAX_GLYPH_CNT];
    s32 r;
    s32 x_cnt, y_cnt;
};

}  // namespace tom
