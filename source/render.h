/* date = July 2nd 2020 9:58 pm */

#ifndef RENDER_H
#define RENDER_H

typedef struct R_Glyph_Box R_Glyph_Box;
struct R_Glyph_Box
{
    v2 offset;
    v2 dim;
};

typedef struct R_Font R_Font;
struct R_Font
{
    b32 initialized;
    u32 var[1];
    f32 top_to_baseline;
    f32 baseline_to_next_top;
    R_Glyph_Box glyph[128];
    f32 advance[128];
};

#define R_SP_0 "\x01"
#define R_SP_1 "\x02"
#define R_SP_2 "\x03"
#define R_SP_3 "\x04"
#define R_SP_4 "\x05"
#define R_SP_5 "\x06"
#define R_SP_6 "\x07"
#define R_SP_7 "\x08"
#define R_SP_8 "\x09"
#define R_SP_9 "\x0A"
#define R_SP_10 "\x0B"
#define R_SP_11 "\x0C"
#define R_SP_12 "\x0D"
#define R_SP_13 "\x0E"
#define R_SP_14 "\x0F"
#define R_SP_15 "\x10"
#define R_SP_16 "\x11"
#define R_SP_17 "\x12"
#define R_SP_18 "\x13"
#define R_SP_19 "\x14"
#define R_SP_20 "\x15"
#define R_SP_21 "\x16"
#define R_SP_22 "\x17"
#define R_SP_23 "\x18"
#define R_SP_24 "\x19"
#define R_SP_25 "\x1A"
#define R_SP_26 "\x1B"
#define R_SP_27 "\x1C"
#define R_SP_28 "\x1D"
#define R_SP_29 "\x1E"
#define R_SP_30 "\x1F"
#define R_SP_31 "\x7F"

#define R_SP_INDX_0 0x01
#define R_SP_INDX_1 0x02
#define R_SP_INDX_2 0x03
#define R_SP_INDX_3 0x04
#define R_SP_INDX_4 0x05
#define R_SP_INDX_5 0x06
#define R_SP_INDX_6 0x07
#define R_SP_INDX_7 0x08
#define R_SP_INDX_8 0x09
#define R_SP_INDX_9 0x0A
#define R_SP_INDX_10 0x0B
#define R_SP_INDX_11 0x0C
#define R_SP_INDX_12 0x0D
#define R_SP_INDX_13 0x0E
#define R_SP_INDX_14 0x0F
#define R_SP_INDX_15 0x10
#define R_SP_INDX_16 0x11
#define R_SP_INDX_17 0x12
#define R_SP_INDX_18 0x13
#define R_SP_INDX_19 0x14
#define R_SP_INDX_20 0x15
#define R_SP_INDX_21 0x16
#define R_SP_INDX_22 0x17
#define R_SP_INDX_23 0x18
#define R_SP_INDX_24 0x19
#define R_SP_INDX_25 0x1A
#define R_SP_INDX_26 0x1B
#define R_SP_INDX_27 0x1C
#define R_SP_INDX_28 0x1D
#define R_SP_INDX_29 0x1E
#define R_SP_INDX_30 0x1F
#define R_SP_INDX_31 0x7F

////////////////////////////////

internal void R_Init(M_Arena *arena);

internal void R_Begin(v2 render_size, v3 color);
internal void R_End(void);

internal void R_SetClip(Rect rect);

internal void R_InitFont(R_Font *font, String8 ttf_path, i32 size);
internal void R_InitUserFont(R_Font *font);
internal void R_ReleaseFont(R_Font *font);
internal b32  R_FontSetSlot(R_Font *font, u32 indx, u8 *bitmap, u32 width, u32 height,
                            u32 xoff, u32 yoff, f32 advance);
internal void R_FontUpdateMipmaps(R_Font *font);
internal v2   R_StringDim(f32 scale, String8 string);
internal v2   R_StringDimWithFont(R_Font *font, f32 scale, String8 string);

internal void R_Rect(Rect rect, v3 color, f32 a);
internal void R_RectOutline(Rect rect, f32 thickness, v3 color, f32 a);
internal void R_SelectFont(R_Font *font);
internal v2   R_String(v2 p, f32 scale, String8 string, v3 color, f32 a);
internal v2   R_StringBaseline(v2 p, f32 scale, String8 string, v3 color, f32 a);

#endif //RENDER_H
