@echo off

set core_name=app
set exe_name=appWin32
set build_options= -DBUILD_WIN32=1
set compile_flags= -nologo /Zi /FC /I ../source/
set common_link_flags= opengl32.lib -opt:ref -incremental:no
set platform_link_flags= gdi32.lib user32.lib winmm.lib %common_link_flags%

if not exist build mkdir build
pushd build
start /b /wait "" "cl.exe"  %build_options% %compile_flags% ../source/win32/win32_main.c /link %platform_link_flags% /out:%exe_name%.exe
start /b /wait "" "cl.exe"  %build_options% %compile_flags% ../source/app.c /LD /link %common_link_flags% /out:%core_name%.dll
copy ..\data\* . >NUL
popd