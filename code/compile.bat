@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl /EHsc -Zi ..\code\win32_game_engine.cpp gdi32.lib  user32.lib kernel32.lib Winmm.lib
popd
