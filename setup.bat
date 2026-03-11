@echo off
echo === Cellular Automata — Setup ^& Build ===
echo.

where g++ >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: g++ not found.
    echo.
    echo Install MinGW-w64 or MSYS2, then ensure g++ is on your PATH.
    echo   MSYS2: https://www.msys2.org
    echo   After installing MSYS2, run: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-sfml
    echo.
    pause
    exit /b 1
)

echo [1/2] Checking for SFML...
echo If SFML is not installed, download SFML 2.x from https://www.sfml-dev.org/download.php
echo and set SFML_DIR to the extracted folder (e.g., set SFML_DIR=C:\SFML-2.6.1)
echo.

if not defined SFML_DIR (
    echo SFML_DIR is not set. Trying default compile (SFML on system PATH^)...
    echo.

    echo [2/2] Compiling...
    g++ -std=c++17 -O2 -o cellular_automata.exe ^
        Main.cpp Grid.cpp RuleSet.cpp Cell.cpp Menu.cpp Settings.cpp ^
        -lsfml-graphics -lsfml-window -lsfml-system
) else (
    echo Using SFML from: %SFML_DIR%
    echo.

    echo [2/2] Compiling...
    g++ -std=c++17 -O2 -o cellular_automata.exe ^
        Main.cpp Grid.cpp RuleSet.cpp Cell.cpp Menu.cpp Settings.cpp ^
        -I"%SFML_DIR%\include" ^
        -L"%SFML_DIR%\lib" ^
        -lsfml-graphics -lsfml-window -lsfml-system
)

if %errorlevel% neq 0 (
    echo.
    echo Build failed. Make sure SFML 2.x is installed and accessible.
    pause
    exit /b 1
)

echo.
echo Build successful! Run with:
echo   cellular_automata.exe
pause
