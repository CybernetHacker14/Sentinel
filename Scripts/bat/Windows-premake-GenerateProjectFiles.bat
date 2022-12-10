@echo off
pushd %~dp0\..\..\
call Premake\bin\premake5.exe vs2022 --gfxapi=DX11
popd
