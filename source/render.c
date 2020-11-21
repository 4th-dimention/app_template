////////////////////////////////
// NOTE(allen): Internal types

typedef struct R_GL_Vertex R_GL_Vertex;
struct R_GL_Vertex
{
    V2F32 xy;
    V3F32 uvw;
    V4F32 rgba;
};

typedef struct R_GL_State R_GL_State;
struct R_GL_State
{
    V2F32 render_resolution;
    
    GLuint vertex_buffer;
    
    R_GL_Vertex *vertex_memory;
    U64 vertex_count;
    U64 vertex_max;
    
    R_Font *selected_font;
    R_RGBATexture *selected_rgba_texture;
    U32 selected_texture_id;
    
    GLuint program_mono_texture;
    GLuint program_rgba_texture;
    
    RectF32 clip;
};

////////////////////////////////
// NOTE(allen): Shared Shader

typedef struct R_GL_Bind R_GL_Bind;
struct R_GL_Bind{
    char *name;
    B32 is_uniform;
    U32 size;
    GLint v;
    U32 offset;
    U32 total_size;
};

global char shader_header[] = "#version 330\n";

#define R_GL_BindMembers(N,u,size) R_GL_Bind N;
#define R_GL_BindValues(N,u,size) {#N,u,size},

////////////////////////////////
// NOTE(allen): Shader Mono Texture

global char shader_vertex_mono_texture[] = 
"in vec2 vert_xy;\n"
"in vec3 vert_uvw;\n"
"in vec4 vert_c;\n"
"out vec3 frag_uvw;\n"
"out vec4 frag_c;\n"
"uniform vec2 inv_half_dim;\n"
"void main()\n"
"{\n"
" gl_Position.x = (vert_xy.x*inv_half_dim.x) - 1;\n"
" gl_Position.y = 1 - (vert_xy.y*inv_half_dim.y);\n"
" gl_Position.z = 0;\n"
" gl_Position.w = 1;\n"
" frag_uvw = vert_uvw;\n"
" frag_c = vert_c;\n"
"}\n"
;

global char shader_fragment_mono_texture[] = 
"in vec3 frag_uvw;\n"
"in vec4 frag_c;\n"
"out vec4 out_c;\n"
"uniform sampler2DArray tex;\n"
"void main()\n"
"{\n"
" float v = texture(tex, frag_uvw).r;\n"
" out_c = vec4(frag_c.rgb, frag_c.a*v);\n"
"}\n"
;

#define R_GL_MonoTextureBinds(X)\
X(vert_xy , 0, 2)\
X(vert_uvw, 0, 3)\
X(vert_c  , 0, 4)\
X(inv_half_dim, 1, 0)\
X(tex         , 1, 0)

typedef struct R_GL_MonoTexture R_GL_MonoTexture;
struct R_GL_MonoTexture
{
    R_GL_MonoTextureBinds(R_GL_BindMembers)
};
global R_GL_MonoTexture r_gl_mono_texture = {
    R_GL_MonoTextureBinds(R_GL_BindValues)
};

////////////////////////////////
// NOTE(allen): Shader RGBA Texture

global char shader_vertex_rgba_texture[] = 
"in vec2 vert_xy;\n"
"in vec3 vert_uvw;\n"
"in vec4 vert_c;\n"
"out vec3 frag_uvw;\n"
"out vec4 frag_c;\n"
"uniform vec2 inv_half_dim;\n"
"void main()\n"
"{\n"
" gl_Position.x = (vert_xy.x*inv_half_dim.x) - 1;\n"
" gl_Position.y = 1 - (vert_xy.y*inv_half_dim.y);\n"
" gl_Position.z = 0;\n"
" gl_Position.w = 1;\n"
" frag_uvw = vert_uvw;\n"
" frag_c = vert_c;\n"
"}\n"
;

global char shader_fragment_rgba_texture[] = 
"in vec3 frag_uvw;\n"
"in vec4 frag_c;\n"
"out vec4 out_c;\n"
"uniform sampler2DArray tex;\n"
"void main()\n"
"{\n"
" vec4 rgba_sample = texture(tex, frag_uvw);\n"
" out_c = rgba_sample*frag_c;\n"
"}\n"
;

#define R_GL_RGBATextureBinds(X)\
X(vert_xy , 0, 2)\
X(vert_uvw, 0, 3)\
X(vert_c  , 0, 4)\
X(inv_half_dim, 1, 0)\
X(tex         , 1, 0)

typedef struct R_GL_RGBATexture R_GL_RGBATexture;
struct R_GL_RGBATexture
{
    R_GL_RGBATextureBinds(R_GL_BindMembers)
};
global R_GL_RGBATexture r_gl_rgba_texture = {
    R_GL_RGBATextureBinds(R_GL_BindValues)
};

////////////////////////////////
// NOTE(allen): Main frame setup

global R_GL_State *global_render = 0;

function GLuint
_R_CompileShader(GLenum shader_kind, char *source)
{
    GLuint shader = glCreateShader(shader_kind);
    GLchar *sources[] = {shader_header, source};
    glShaderSource(shader, 2, sources, 0);
    glCompileShader(shader);
    
    S32 log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    char error[1024] = {0};
    log_length = ClampTop(log_length, sizeof(error));
    glGetShaderInfoLog(shader, log_length, 0, error);
    Assert(log_length == 0);
    
    return(shader);
}

function GLuint
_R_LinkProgram(GLuint vertex_shader, GLuint fragment_shader,
               R_GL_Bind *binds, U64 bind_count){
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    S32 log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    char error[1024] = {0};
    log_length = ClampTop(log_length, sizeof(error));
    glGetProgramInfoLog(program, log_length, 0, error);
    Assert(log_length == 0);
    
    U32 total_size = 0;
    R_GL_Bind *bind = binds;
    for (U64 i = 0; i < bind_count; i += 1, bind += 1){
        if (!bind->is_uniform){
            total_size += bind->size*sizeof(F32);
        }
    }
    
    U64 offset = 0;
    bind = binds;
    for (U64 i = 0; i < bind_count; i += 1, bind += 1){
        if (bind->is_uniform){
            bind->v = glGetUniformLocation(program, bind->name);
        }
        else{
            bind->v = glGetAttribLocation(program, bind->name);
            bind->offset = offset;
            bind->total_size = total_size;
            offset += bind->size*sizeof(F32);
        }
    }
    
    return(program);
}

function void
R_Init(M_Arena *arena)
{
    global_render = PushArrayZero(arena, R_GL_State, 1);
    LoadAllOpenGLProcedures();
    
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    // NOTE(allen): vertex array
    GLint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // NOTE(allen): shader
    {
        GLuint vertex_shader   = _R_CompileShader(GL_VERTEX_SHADER  , shader_vertex_mono_texture  );
        GLuint fragment_shader = _R_CompileShader(GL_FRAGMENT_SHADER, shader_fragment_mono_texture);
        GLuint program = _R_LinkProgram(vertex_shader, fragment_shader,
                                        (R_GL_Bind*)&r_gl_mono_texture,
                                        sizeof(r_gl_mono_texture)/sizeof(R_GL_Bind));
        global_render->program_mono_texture = program;
    }
    
    {
        GLuint vertex_shader   = _R_CompileShader(GL_VERTEX_SHADER  , shader_vertex_rgba_texture  );
        GLuint fragment_shader = _R_CompileShader(GL_FRAGMENT_SHADER, shader_fragment_rgba_texture);
        GLuint program = _R_LinkProgram(vertex_shader, fragment_shader,
                                        (R_GL_Bind*)&r_gl_rgba_texture,
                                        sizeof(r_gl_rgba_texture)/sizeof(R_GL_Bind));
        global_render->program_rgba_texture = program;
    }
    
    glUseProgram(global_render->program_mono_texture);
    glUniform1i(r_gl_mono_texture.tex.v, 0);
    
    
    // NOTE(allen): vertex buffer
    glGenBuffers(1, &global_render->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, global_render->vertex_buffer);
    
    
    // NOTE(allen): increase this number for bigger batches; keep multiple of 3
    global_render->vertex_max = 4095;
    global_render->vertex_memory = PushArray(arena, R_GL_Vertex, global_render->vertex_max);
    glBufferData(GL_ARRAY_BUFFER, global_render->vertex_max*sizeof(R_GL_Vertex), 0, GL_DYNAMIC_DRAW);
    
    
    // NOTE(allen): vertex attributes
    {
        U64 bind_count = sizeof(r_gl_mono_texture)/sizeof(R_GL_Bind);
        R_GL_Bind *bind = (R_GL_Bind*)&r_gl_mono_texture;
        for (U64 i = 0; i < bind_count; i += 1, bind += 1)
        {
            if (!bind->is_uniform)
            {
                glEnableVertexAttribArray(bind->v);
                glVertexAttribPointer(bind->v, bind->size, GL_FLOAT, GL_FALSE, bind->total_size, PtrFromInt(bind->offset));
            }
        }
    }
    
    // NOTE(allen): setup texture download alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // NOTE(allen): select null font
    global_render->selected_font = 0;
    global_render->selected_rgba_texture = 0;
    global_render->selected_texture_id = 0;
}

function void
_R_Flush(void)
{
    if (global_render->vertex_count > 0)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, global_render->vertex_count*sizeof(R_GL_Vertex), global_render->vertex_memory);
        glDrawArrays(GL_TRIANGLES, 0, global_render->vertex_count);
        global_render->vertex_count = 0;
    }
}

function R_GL_Vertex*
_R_AllocVertices(U64 count)
{
    Assert(count <= global_render->vertex_max);
    if (global_render->vertex_count + count > global_render->vertex_max){
        _R_Flush();
    }
    
    R_GL_Vertex *result = global_render->vertex_memory + global_render->vertex_count;
    global_render->vertex_count += count;
    return(result);
}

function void
R_Begin(V2F32 render_size, V3F32 color)
{
    global_render->render_resolution = render_size;
    R_SetClip(MakeRect(0.f, 0.f, V2Expand(render_size)));
    glViewport(0, 0, render_size.x, render_size.y);
    glClearColor(color.x, color.y, color.z, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform2f(r_gl_mono_texture.inv_half_dim.v, 2.f/render_size.x, 2.f/render_size.y);
}

function void
R_End(void)
{
    _R_Flush();
    OS_RefreshScreen();
}

function RectF32
R_GetClip(void)
{
    return(global_render->clip);
}

function RectF32
R_SetClip(RectF32 rect)
{
    _R_Flush();
    glScissor((GLint)rect.x0, (GLint)(global_render->render_resolution.y - rect.y1),
              (GLint)(rect.x1 - rect.x0), (GLint)(rect.y1 - rect.y0));
    RectF32 result = global_render->clip;
    global_render->clip = rect;
    return(result);
}

function RectF32
R_PushClip(RectF32 rect)
{
    RectF32 intersected = RectIntersect(rect, global_render->clip);
    RectF32 result = R_SetClip(intersected);
    return(result);
}


////////////////////////////////
// NOTE(allen): OpenGL Texture State

function void
_R_RestoreSelectedTexture(void){
    glBindTexture(GL_TEXTURE_2D_ARRAY, global_render->selected_texture_id);
}


////////////////////////////////
// NOTE(allen): Font

#define STB_TRUETYPE_IMPLEMENTATION
#include "ext/stb_truetype.h"

function void
_R_TextureArraySetDataSlice(U32 index, U8 *data, U32 width, U32 height)
{
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                    0, 0, index,
                    width, height, 1,
                    GL_RED, GL_UNSIGNED_BYTE, data);
}

function void
_R_BaseInitFont(R_Font *font)
{
    // NOTE(allen): init
    font->initialized = 1;
    MemoryZeroArray(font->glyph);
    MemoryZeroArray(font->advance);
    
    // NOTE(allen): font metrics
    font->top_to_baseline = 0.f;
    font->baseline_to_next_top = 0.f;
    
    // NOTE(allen): setup texture
    glGenTextures(1, &font->var[0]);
    glBindTexture(GL_TEXTURE_2D_ARRAY, font->var[0]);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 32, 64, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 3);
    
    // NOTE(allen): full white at slice 0
    local U8 temp_buffer[32*64] = {0};
    if (temp_buffer[0] == 0)
    {
        MemorySet(temp_buffer, 0xFF, sizeof(temp_buffer));
    }
    _R_TextureArraySetDataSlice(0, temp_buffer, 32, 64);
}

function void
R_InitUserFont(R_Font *font)
{
    _R_BaseInitFont(font);
    _R_RestoreSelectedTexture();
}

function void
R_ReleaseFont(R_Font *font)
{
    font->initialized = 0;
    glDeleteTextures(1, &font->var[0]);
    if (global_render->selected_font == font)
    {
        global_render->selected_font = 0;
        global_render->selected_texture_id = 0;
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
}

function void
R_InitFont(R_Font *font, String8 ttf_path, S32 size)
{
    font->initialized = 0;
    
    M_Arena *scratch = OS_GetScratch();
    
    void *data = 0;
    U64 data_len = 0;
    OS_LoadEntireFile(scratch, ttf_path, &data, &data_len);
    
    if (data_len > 0)
    {
        stbtt_fontinfo stb_font;
        if (stbtt_InitFont(&stb_font, data, stbtt_GetFontOffsetForIndex(data, 0)))
        {
            // NOTE(allen): init
            _R_BaseInitFont(font);
            
            // NOTE(allen): font metrics
            F32 scale = stbtt_ScaleForPixelHeight(&stb_font, size);
            F32 em_scale = stbtt_ScaleForMappingEmToPixels(&stb_font, size);
            
            S32 ascent;
            S32 descent;
            S32 gap;
            stbtt_GetFontVMetrics(&stb_font, &ascent, &descent, &gap);
            
            font->top_to_baseline = ascent*em_scale;
            font->baseline_to_next_top = (gap - descent)*em_scale;
            
            
            // NOTE(allen): characters between [32,126]
            {
                R_GlyphBox *glyph_ptr = font->glyph + 32;
                F32 *advance_ptr = font->advance + 32;
                for (U32 i = 32; i <= 126; i += 1, advance_ptr += 1, glyph_ptr += 1)
                {
                    S32 w;
                    S32 h;
                    S32 xoff;
                    S32 yoff;
                    
                    U8 *bitmap = stbtt_GetCodepointBitmap(&stb_font, 0, scale, i, &w, &h, &xoff, &yoff);
                    Assert(w <= 32);
                    Assert(h <= 64);
                    _R_TextureArraySetDataSlice(i, bitmap, w, h);
                    stbtt_FreeBitmap(bitmap, 0);
                    
                    S32 advance;
                    S32 lsb;
                    stbtt_GetCodepointHMetrics(&stb_font, i, &advance, &lsb);
                    
                    glyph_ptr->offset.x = xoff;
                    glyph_ptr->offset.y = yoff;
                    glyph_ptr->dim.x = w;
                    glyph_ptr->dim.y = h;
                    *advance_ptr = advance*em_scale;
                }
            }
            
            // NOTE(allen): update mipmaps
            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
            
            _R_RestoreSelectedTexture();
        }
    }
    
    OS_ReleaseScratch(scratch);
}

function B32
R_FontSetSlot(R_Font *font, U32 indx, U8 *bitmap, U32 width, U32 height,
              U32 xoff, U32 yoff, F32 advance)
{
    B32 result = 0;
    if (font->initialized)
    {
        if (0 < indx && indx <= 0x7F)
        {
            if (width <= 32 && height <= 64)
            {
                glBindTexture(GL_TEXTURE_2D_ARRAY, font->var[0]);
                _R_TextureArraySetDataSlice(indx, bitmap, width, height);
                
                font->glyph[indx].offset.x = xoff;
                font->glyph[indx].offset.y = yoff;
                font->glyph[indx].dim.x = width;
                font->glyph[indx].dim.y = height;
                font->advance[indx] = advance;
                
                _R_RestoreSelectedTexture();
            }
        }
    }
    return(result);
}

function void
R_FontUpdateMipmaps(R_Font *font)
{
    if (font != global_render->selected_font)
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, font->var[0]);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        _R_RestoreSelectedTexture();
    }
    else
    {
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }
}

function V2F32
R_StringDimWithFont(R_Font *font, F32 scale, String8 string)
{
    V2F32 result = {0};
    U8 *ptr = string.str;
    U8 *end = ptr + string.size;
    F32 *advances = font->advance;
    for (;ptr < end; ptr += 1)
    {
        if (*ptr <= 0x7F)
        {
            result.x += F32Ceil(advances[*ptr]*scale);
        }
    }
    result.y = F32Ceil((font->top_to_baseline + font->baseline_to_next_top)*scale);
    return(result);
}

function V2F32
R_StringDim(F32 scale, String8 string)
{
    return(R_StringDimWithFont(global_render->selected_font, scale, string));
}


////////////////////////////////
// NOTE(allen): RGBA Texture

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"

function void
R_InitRGBATexture(R_RGBATexture *texture, String8 img_path){
    texture->initialized = 0;
    
    M_Arena *scratch = OS_GetScratch();
    
    void *data = 0;
    U64 data_len = 0;
    OS_LoadEntireFile(scratch, img_path, &data, &data_len);
    
    if (data_len > 0){
        S32 w;
        S32 h;
        S32 channels;
        stbi_uc *img_data = stbi_load_from_memory((stbi_uc*)data, data_len, &w, &h, &channels, 4);
        if (img_data != 0){
            if (w > 0 && h > 0){
                // NOTE(allen): init
                texture->initialized = 1;
                texture->dim = v2F32(w, h);
                
                // NOTE(allen): setup texture
                glGenTextures(1, &texture->var[0]);
                glBindTexture(GL_TEXTURE_2D_ARRAY, texture->var[0]);
                glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, w, h, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                
                // NOTE(allen): rgba slice
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                                0, 0, 1,
                                w, h, 1,
                                GL_RED, GL_UNSIGNED_BYTE, img_data);
                
                // NOTE(allen): white slice
                stbi_uc *opl = img_data + w*h*4;
                for (stbi_uc *ptr = img_data; ptr < opl; ptr += 1){
                    *ptr = 0xFF;
                }
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                                0, 0, 0,
                                w, h, 1,
                                GL_RED, GL_UNSIGNED_BYTE, img_data);
                
                _R_RestoreSelectedTexture();
            }
            
            stbi_image_free(img_data);
        }
    }
    
    OS_ReleaseScratch(scratch);
}

////////////////////////////////
// NOTE(allen): Draw

function void
R_SelectFont(R_Font *font){
    if (global_render->selected_font != font){
        _R_Flush();
        global_render->selected_font = font;
        global_render->selected_rgba_texture = 0;
        global_render->selected_texture_id = font->var[0];
        _R_RestoreSelectedTexture();
    }
}

function void
R_SelectRGBATexture(R_RGBATexture *texture){
    if (global_render->selected_rgba_texture != texture){
        _R_Flush();
        global_render->selected_font = 0;
        global_render->selected_rgba_texture = texture;
        global_render->selected_texture_id = texture->var[0];
        _R_RestoreSelectedTexture();
    }
}

#define DupVert(n) vertices[n].xy = vertices[n - 2].xy
#define DupVertUVW(n) vertices[n].uvw = vertices[n - 2].uvw
#define DupVertC(n) vertices[n].c = vertices[n - 2].c

function void
R_Rect(RectF32 rect, V3F32 color, F32 a)
{
    R_GL_Vertex *vertices = _R_AllocVertices(6);
    vertices[0].xy = rect.p0;
    vertices[1].xy = v2F32(rect.x1, rect.y0);
    vertices[2].xy = v2F32(rect.x0, rect.y1);
    
    DupVert(3); DupVert(4);
    vertices[5].xy = rect.p1;
    
    R_GL_Vertex *v = vertices;
    for (U64 i = 0; i < 6; i += 1, v += 1)
    {
        v->uvw = v3F32(0.f, 0.f, 0.f);
        v->rgba = v4F32(color.x, color.y, color.z, a);
    }
}

function void
R_RectOutline(RectF32 rect, F32 thickness, V3F32 color, F32 a)
{
    RectF32 outer = rect;
    RectF32 inner = RectShrink(rect, thickness);
    
    R_GL_Vertex *vertices = _R_AllocVertices(24);
    vertices[0].xy = v2F32(outer.x0, outer.y0);
    vertices[1].xy = v2F32(inner.x0, inner.y0);
    vertices[2].xy = v2F32(outer.x1, outer.y0);
    
    DupVert(3); DupVert(4);
    vertices[5].xy = v2F32(inner.x1, inner.y0);
    
    DupVert(6); DupVert(7);
    vertices[8].xy = v2F32(outer.x1, outer.y1);
    
    DupVert(9); DupVert(10);
    vertices[11].xy = v2F32(inner.x1, inner.y1);
    
    DupVert(12); DupVert(13);
    vertices[14].xy = v2F32(outer.x0, outer.y1);
    
    DupVert(15); DupVert(16);
    vertices[17].xy = v2F32(inner.x0, inner.y1);
    
    DupVert(18); DupVert(19);
    vertices[20].xy = v2F32(outer.x0, outer.y0);
    
    DupVert(21); DupVert(22);
    vertices[23].xy = v2F32(inner.x0, inner.y0);
    
    R_GL_Vertex *v = vertices;
    for (U64 i = 0; i < 24; i += 1, v += 1){
        v->uvw = v3F32(0.f, 0.f, 0.f);
        v->rgba = v4F32(color.x, color.y, color.z, a);
    }
}

function V2F32
R_StringBaselineCapped(V2F32 p, F32 max_x, F32 scale, String8 string, V3F32 color, F32 a)
{
    V2F32 result = {0};
    R_Font *font = global_render->selected_font;
    if (font != 0 && font->initialized){
        F32 *advances = font->advance;
        
        U8 tail[3] = {'.', '.', '.', };
        F32 x_tail = F32Ceil(advances['.']*scale)*3.f;
        B32 string_will_fit = 0;
        
        F32 x = p.x;
        if (x + x_tail <= max_x){
            U8 *ptr = string.str;
            U8 *end = ptr + string.size;
            R_GlyphBox *glyphs = font->glyph;
            for (;ptr < end; ptr += 1){
                top:
                if (*ptr <= 0x7F)
                {
                    F32 next_x = x + F32Ceil(advances[*ptr]*scale);
                    
                    // NOTE(allen): If the tail isn't going to fit after this
                    // we look ahead to see if the string fits. If not we switch
                    // to the tail now.
                    if (next_x + x_tail > max_x && !string_will_fit){
                        U8 *look_ahead_ptr = ptr + 1;
                        F32 look_ahead_x = next_x;
                        for (;look_ahead_ptr < end; look_ahead_ptr += 1){
                            look_ahead_x += F32Ceil(advances[*ptr]*scale);
                            if (look_ahead_x > max_x){
                                break;
                            }
                        }
                        if (look_ahead_x > max_x){
                            ptr = tail;
                            end = ptr + 3;
                            goto top;
                        }
                        
                        string_will_fit = 1;
                    }
                    
                    R_GlyphBox *g = &glyphs[*ptr];
                    
                    RectF32 rect;
                    rect.x0 = x + g->offset.x*scale;
                    rect.y0 = p.y + g->offset.y*scale;
                    rect.x1 = rect.x0 + g->dim.x*scale;
                    rect.y1 = rect.y0 + g->dim.y*scale;
                    
                    V2F32 uv;
                    uv.x = g->dim.x*(1.f/32.f);
                    uv.y = g->dim.y*(1.f/64.f);
                    
                    F32 w = (*ptr);
                    
                    R_GL_Vertex *vertices = _R_AllocVertices(6);
                    vertices[0].xy = rect.p0;
                    vertices[0].uvw = v3F32(0.f, 0.f, w);
                    
                    vertices[1].xy = v2F32(rect.x1, rect.y0);
                    vertices[1].uvw = v3F32(uv.x, 0.f, w);
                    
                    vertices[2].xy = v2F32(rect.x0, rect.y1);
                    vertices[2].uvw = v3F32(0.f, uv.y, w);
                    
                    DupVert(3); DupVertUVW(3);
                    DupVert(4); DupVertUVW(4);
                    
                    vertices[5].xy = rect.p1;
                    vertices[5].uvw = v3F32(uv.x, uv.y, w);
                    
                    R_GL_Vertex *v = vertices;
                    for (U64 i = 0; i < 6; i += 1, v += 1)
                    {
                        v->rgba = v4F32(color.x, color.y, color.z, a);
                    }
                    
                    x = next_x;
                }
            }
        }
        result.x = x - p.x;
        result.y = F32Ceil((font->top_to_baseline + font->baseline_to_next_top)*scale);
    }
    return(result);
}

function V2F32
R_StringCapped(V2F32 p, F32 max_x, F32 scale, String8 string, V3F32 color, F32 a)
{
    V2F32 result = {0};
    R_Font *font = global_render->selected_font;
    if (font != 0 && font->initialized)
    {
        p.y += font->top_to_baseline*scale;
        result = R_StringBaselineCapped(p, max_x, scale, string, color, a);
    }
    return(result);
}

function V2F32
R_StringBaseline(V2F32 p, F32 scale, String8 string, V3F32 color, F32 a)
{
    return(R_StringBaselineCapped(p, Inf32(), scale, string, color, a));
}

function V2F32
R_String(V2F32 p, F32 scale, String8 string, V3F32 color, F32 a)
{
    return(R_StringCapped(p, Inf32(), scale, string, color, a));
}

