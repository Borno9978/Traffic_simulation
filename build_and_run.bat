@echo off
set PATH=C:\msys64\mingw64\bin;%PATH%
echo Compiling...
gcc main.c environment.c road.c traffic.c vehicle.c -o program.exe -lopengl32 -lglu32 -lfreeglut
if %ERRORLEVEL% == 0 (
    echo Build successful. Running...
    program.exe
) else (
    echo Build failed.
    pause
)
