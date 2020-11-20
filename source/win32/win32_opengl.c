global HGLRC global_opengl_render_context;

function void *
W32_LoadOpenGLProcedure(char *name)
{
    void *p = (void *)wglGetProcAddress(name);
    if(!p || p == (void *)0x1 || p == (void *)0x2 || p == (void *)0x3 || p == (void *)-1)
    {
        return 0;
    }
    else
    {
        return p;
    }
}

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

function void
W32_LoadWGLFunctions(HINSTANCE h_instance)
{
    wglChoosePixelFormatARB    = (PFNWGLCHOOSEPIXELFORMATARBPROC)    W32_LoadOpenGLProcedure("wglChoosePixelFormatARB");
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) W32_LoadOpenGLProcedure("wglCreateContextAttribsARB");
    wglMakeContextCurrentARB   = (PFNWGLMAKECONTEXTCURRENTARBPROC)   W32_LoadOpenGLProcedure("wglMakeContextCurrentARB");
    wglSwapIntervalEXT         = (PFNWGLSWAPINTERVALEXTPROC)         W32_LoadOpenGLProcedure("wglSwapIntervalEXT");
}

function B32
W32_InitOpenGL(HDC *device_context, HINSTANCE h_instance)
{
    B32 result = 0;
    
    // NOTE(rjf): Set up pixel format for dummy context
    int pixel_format = 0;
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    
    pixel_format = ChoosePixelFormat(*device_context, &pfd);
    
    if(pixel_format)
    {
        SetPixelFormat(*device_context, pixel_format, &pfd);
        HGLRC gl_dummy_render_context = wglCreateContext(*device_context);
        wglMakeCurrent(*device_context, gl_dummy_render_context);
        
        W32_LoadWGLFunctions(h_instance);
        
        // NOTE(rjf): Setup real pixel format
        {
            int pf_attribs_i[] =
            {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB, 32,
                WGL_DEPTH_BITS_ARB, 24,
                WGL_STENCIL_BITS_ARB, 8,
                0
            };
            
            UINT num_formats = 0;
            wglChoosePixelFormatARB(*device_context,
                                    pf_attribs_i,
                                    0,
                                    1,
                                    &pixel_format,
                                    &num_formats);
        }
        
        if(pixel_format)
        {
            const int context_attribs[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                0
            };
            
            global_opengl_render_context = wglCreateContextAttribsARB(*device_context,
                                                                      gl_dummy_render_context,
                                                                      context_attribs);
            if(global_opengl_render_context)
            {
                wglMakeCurrent(*device_context, 0);
                wglDeleteContext(gl_dummy_render_context);
                wglMakeCurrent(*device_context, global_opengl_render_context);
                wglSwapIntervalEXT(0);
                result = 1;
            }
        }
    }
    
    return result;
}

function void
W32_CleanUpOpenGL(HDC *device_context)
{
    wglMakeCurrent(*device_context, 0);
    wglDeleteContext(global_opengl_render_context);
}

function void
W32_OpenGLRefreshScreen(void)
{
    wglSwapLayerBuffers(global_device_context, WGL_SWAP_MAIN_PLANE);
}
