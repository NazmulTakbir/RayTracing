@ECHO OFF
del output.exe
del images\*.bmp
del *.bmp
set source_file=%1
set mingw_home=C:\Program Files\CodeBlocks\MinGW\x86_64-w64-mingw32
g++ "%source_file%" -o output.exe -I "%mingw_home%\include" -O3 -L "%mingw_home%\lib" -lopengl32 -lglu32 -lfreeglut
output.exe