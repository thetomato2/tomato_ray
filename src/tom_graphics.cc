namespace tom
{

fn void draw_clear(Texture *buffer, Color_u32 color)
{
    Assert(buffer->type == Texture::Type::R8G8B8A8);
    u32 pitch = texture_get_pitch(buffer);
    byt *row  = (byt *)buffer->buf;
    for (i32 y = 0; y < buffer->height; ++y) {
        u32 *pixel = (u32 *)row;
        for (i32 x = 0; x < buffer->width; ++x) {
            *pixel++ = color.rgba;
        }
        row += pitch;
    }
}

fn void draw_clear(Texture *buffer, v3f color)
{
    draw_clear(buffer, v3f_to_color_u32(color));
}

fn void draw_rect(Texture *buffer, i32 min_x, i32 min_y, i32 max_x, i32 max_y, Color_u32 color)
{
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x > buffer->width) max_x = buffer->width;
    if (max_y > buffer->height) max_y = buffer->height;

    Assert(buffer->type == Texture::Type::R8G8B8A8);
    u32 pitch = texture_get_pitch(buffer);
    byt *row  = (byt *)buffer->buf + min_x * texture_get_texel_size(buffer->type) + min_y * pitch;

    for (i32 y = min_y; y < max_y; ++y) {
        u32 *pixel = (u32 *)row;
        for (i32 x = min_x; x < max_x; ++x) {
            v4f dest_col  = color_u32_to_v4f(*(u32 *)pixel);
            v4f src_col   = color_u32_to_v4f(color.rgba);
            v4f blend_col = lerp(dest_col, src_col, src_col.a);
            blend_col.a   = 1.0f;
            *pixel++      = v4f_to_color_u32(blend_col).rgba;
        }
        row += pitch;
    }
}

fn void draw_rect(Texture *buffer, i32 min_x, i32 min_y, i32 max_x, i32 max_y, v3f color)
{
    draw_rect(buffer, min_x, min_y, max_x, max_y, v3f_to_color_u32(color));
}

fn void draw_rect(Texture *buffer, r2i rect, Color_u32 color)
{
    draw_rect(buffer, rect.x0, rect.y0, rect.x1, rect.y1, color);
}
fn void draw_rect(Texture *buffer, r2f rect, Color_u32 color)
{
    r2i rect_i32 = rect_f32_to_i32(rect);
    draw_rect(buffer, rect_i32.x0, rect_i32.y0, rect_i32.x1, rect_i32.y1, color);
}

fn void draw_rect(Texture *buffer, r2i rect, v3f color)
{
    draw_rect(buffer, rect.x0, rect.y0, rect.x1, rect.y1, v3f_to_color_u32(color));
}

fn void draw_rect(Texture *buffer, r2u rect, v3f color)
{
    draw_rect(buffer, (i32)rect.x0, (i32)rect.y0, (i32)rect.x1, (i32)rect.y1, v3f_to_color_u32(color));
}

fn void draw_rect(Texture *buffer, r2u rect, Color_u32 color)
{
    draw_rect(buffer, (i32)rect.x0, (i32)rect.y0, (i32)rect.x1, (i32)rect.y1, color);
}

fn void draw_rect(Texture *buffer, r2f rect, v3f color)
{
    r2i rect_i32 = rect_f32_to_i32(rect);
    draw_rect(buffer, rect_i32.x0, rect_i32.y0, rect_i32.x1, rect_i32.y1, v3f_to_color_u32(color));
}

fn void draw_square(Texture *buffer, v2f pos, f32 radius, Color_u32 color)
{
    r2i rc = rect_f32_to_i32(rect_init_square(pos, radius));
    draw_rect(buffer, rc.x0, rc.y0, rc.x1, rc.y1, color);
}

fn void draw_rect_outline(Texture *buffer, i32 min_x, i32 min_y, f32 max_x, f32 max_y,
                          i32 thickness, Color_u32 color)
{
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x > buffer->width) max_x = buffer->width;
    if (max_y > buffer->height) max_y = buffer->height;

    Assert(buffer->type == Texture::Type::R8G8B8A8);
    u32 pitch = texture_get_pitch(buffer);
    byt *row  = (byt *)buffer->buf + min_x * texture_get_texel_size(buffer->type) + min_y * pitch;

    for (i32 y = min_y; y < max_y; ++y) {
        u32 *pixel = (u32 *)row;
        for (i32 x = min_x; x < max_x; ++x) {
            if (x <= min_x + thickness || x >= max_x - thickness - 1 || y <= min_y + thickness ||
                y >= max_y - thickness - 1) {
                *pixel = color.rgba;
            }
            ++pixel;
        }
        row += pitch;
    }
}

fn void draw_rect_outline(Texture *buffer, i32 min_x, i32 min_y, i32 max_x, i32 max_y,
                          i32 thickness, v3f color)
{
    draw_rect_outline(buffer, min_x, min_y, max_x, max_y, thickness, v3f_to_color_u32(color));
}

fn void draw_rect_outline(Texture *buffer, r2i rect, i32 thickness, Color_u32 color)
{
    draw_rect_outline(buffer, rect.x0, rect.y0, rect.x1, rect.y1, thickness, color);
}
fn void draw_rect_outline(Texture *buffer, r2f rect, i32 thickness, Color_u32 color)
{
    r2i rect_i32 = rect_f32_to_i32(rect);
    draw_rect_outline(buffer, rect_i32.x0, rect_i32.y0, rect_i32.x1, rect_i32.y1, thickness, color);
}

fn void draw_rect_outline(Texture *buffer, r2i rect, i32 thickness, v3f color)
{
    draw_rect_outline(buffer, rect.x0, rect.y0, rect.x1, rect.y1, thickness, v3f_to_color_u32(color));
}

fn void draw_rect_outline(Texture *buffer, r2f rect, i32 thickness, v3f color)
{
    r2i rect_i32 = rect_f32_to_i32(rect);
    draw_rect_outline(buffer, rect_i32.x0, rect_i32.y0, rect_i32.x1, rect_i32.y1, thickness,
                      v3f_to_color_u32(color));
}

}  // namespace tom
