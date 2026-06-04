@echo off

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build-Basic-Application.lua vs2022
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build-Basic-Server.lua vs2022
popd
pause