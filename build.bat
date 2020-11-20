@echo off

set exe_name=app
set build_options= -DBUILD_WIN32=1
set compile_flags= -nologo /Zi /FC /I ../source/
set link_flags= gdi32.lib user32.lib winmm.lib Comdlg32.lib opengl32.lib -opt:ref -incremental:no

if not exist build mkdir build
pushd build
cl %build_options% %compile_flags% ../source/app.c /link %link_flags% /out:%exe_name%.exe
copy ..\data\* . >NUL
popd