@echo off
echo Running action 'clean'...
echo Cleaning files...
pushd %~dp0\..\..\
rmdir /s /q build
if exist build rd /s /q build
if exist build rd /s /q build
if exist build echo Help! & pause
popd
