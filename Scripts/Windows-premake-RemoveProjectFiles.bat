@echo off
pushd %~dp0\..\
call Premake\bin\premake5.exe clean
popd
PAUSE
