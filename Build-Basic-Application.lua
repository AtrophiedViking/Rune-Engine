-- premake5.lua
workspace "Basic-Application"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Client"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
   buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
NetworkingBinDir = "vendor/GameNetworkingSockets/bin/%{cfg.system}/%{cfg.buildcfg}"

group "App"
   include "Core/Build-Core.lua"
   include "Client/Build-Client.lua"
group ""
