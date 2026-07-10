@echo off
pushd %~dp0\..\..\
call BuildOptions\Windows\premake5.exe vs2026
popd
PAUSE
