
#if BUILD_WIN32
#undef function
#include <windows.h>
#define function static
#include <gl/gl.h>
#include "ext/wglext.h"
#else
#error "OpenGL includes for platform not supported."
#endif
#include "ext/glext.h"

#define GLProc(name, type) PFN##type##PROC name = 0;
#include "opengl_procedure_list.h"

function void
LoadAllOpenGLProcedures(void)
{
#define GLProc(name, type) name = os->LoadOpenGLProcedure(#name);
#include "opengl_procedure_list.h"
}
