@echo off
pushd %~dp0\..\
rmdir /s /q build
if exist build rd /s /q build
if exist build rd /s /q build
if exist build echo Help! & pause
popd
PAUSE
