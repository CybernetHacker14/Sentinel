@echo off
pushd %~dp0\..\
call Premake\bin\premake5.exe vs2019
popd
PAUSE