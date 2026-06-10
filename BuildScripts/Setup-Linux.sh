#!/bin/bash

pushd ..
Vendor/Binaries/Premake/Linux/premake5 --cc=clang --file=Build-Basic-Application.lua gmake2
Vendor/Binaries/Premake/Linux/premake5 --cc=clang --file=Build-Basic-Server.lua gmake2
popd
