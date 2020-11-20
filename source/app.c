#include "base.h"
#include "app_memory.h"
#include "os.h"
#include "opengl.h"
#include "render.h"
#include "app.h"

#include "base.c"
#include "app_memory.c"
#include "os.c"
#include "render.c"

////////////////////////////////

global V3F32 cl_black = {0.f, 0.f, 0.f};
global V3F32 cl_white = {1.f, 1.f, 1.f};

global V3F32 cl_red    = {1.f, 0.f, 0.f};
global V3F32 cl_yellow = {1.f, 1.f, 0.f};
global V3F32 cl_green  = {0.f, 1.f, 0.f};
global V3F32 cl_cyan   = {0.f, 1.f, 1.f};
global V3F32 cl_blue   = {0.f, 0.f, 1.f};
global V3F32 cl_awful  = {1.f, 0.f, 1.f};

////////////////////////////////

global APP_Variables *vars = 0;

APP_PERMANENT_LOAD
{
    os = os_;
    
    {
        M_Arena arena_ = M_ArenaInitialize();
        vars = PushArray(&arena_, APP_Variables, 1);
        MemoryCopyStruct(&vars->arena_, &arena_);
    }
    M_Arena *arena = vars->arena = &vars->arena_;
    
    R_Init(arena);
    R_InitFont(&vars->test_font, str8_lit("liberation-mono.ttf"), 48);
    R_InitFont(&vars->test_font_small, str8_lit("liberation-mono.ttf"), 24);
}

APP_HOT_LOAD
{
    os = os_;
}

APP_HOT_UNLOAD {}

APP_UPDATE
{
    for (OS_Event *event = 0;
         OS_GetNextEvent(&event);)
    {
        if (event->kind == OS_EventKind_WindowClose)
        {
            os->Quit();
        }
    }
    
    V2F32 window_size = v2F32(os->window_size.x, os->window_size.y);
    R_Begin(window_size, cl_black);
    
    R_Rect(MakeRect(70, 70, 530, 530), cl_blue, 1.f);
    R_RectOutline(MakeRect(100, 100, 560, 500), 4.f, cl_red, 1.f);
    
    String8 string = str8_lit("(exclaim `(hello world))");
    
    R_SelectFont(&vars->test_font);
    
    V2F32 p = v2F32(130, 130);
    V2F32 dim = R_String(p, 1.f, string, cl_yellow, 1.f);
    p.y += dim.y;
    
    dim = R_String(p, 1.f, string, cl_green, 0.5f);
    p.y += dim.y*0.5f;
    dim = R_String(p, 1.f, string, cl_green, 0.5f);
    p.y += dim.y*0.5f;
    dim = R_String(p, 1.f, string, cl_green, 0.5f);
    p.y += dim.y;
    
    dim = R_String(p, 0.7f, string, cl_white, 1.f);
    p.y += dim.y;
    dim = R_String(p, 0.6f, string, cl_white, 1.f);
    p.y += dim.y;
    
    R_SelectFont(&vars->test_font_small);
    dim = R_String(p, 1.f, string, cl_white, 1.f);
    p.y += dim.y;
    dim = R_String(p, 0.8f, string, cl_white, 1.f);
    p.y += dim.y;
    
    R_End();
}

////////////////////////////////

#include "win32/win32_main.c"
