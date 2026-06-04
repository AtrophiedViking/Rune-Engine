project "Core"
   kind "StaticLib"
   language "C++"
   cppdialect "C++latest"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp", "Platform/Client/**.h", "Platform/Client/**.cpp"}

   includedirs
   {
      "Source",
      "Platform/Client",
	  -- Include
      "../vendor/GameNetworkingSockets/include",
      "../vendor/glm",
      "../vender/GLFW/include",
      "../vendor/spdlog/include",
      "../vendor/imgui"
   }

   links
   {
   }

   defines
   {
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "PLATFORM_WINDOWS" }
       buildoptions { "/utf-8" }

      postbuildcommands 
	  {
	    '{COPY} "../%{NetworkingBinDir}/GameNetworkingSockets.dll" "%{cfg.targetdir}"',
	    '{COPY} "../%{NetworkingBinDir}/libcrypto-3-x64.dll" "%{cfg.targetdir}"',
	    '{COPY} "../%{NetworkingBinDir}/libprotobufd.dll" "%{cfg.targetdir}"',
	  }


   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"