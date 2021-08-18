workspace "fallout76-afk"
  configurations { "Debug", "Release" }
  location "out"
  systemversion "latest"
  toolset "v141"
  language "C++"
  targetdir "out/%{cfg.buildcfg}"
  objdir "out"
  cppdialect "c++17"
  characterset "MBCS"
  architecture "x64"
  defines { "WIN32", "WIN32_LEAN_AND_MEAN" }
  startproject "xinput1_3"

  filter "configurations:Debug"
    defines { "DEBUG", "_DEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
    symbols "On"

  filter "configurations:Release"
    optimize "On"

project "xinput1_3"
  kind "SharedLib"
  linkoptions { "/DEF:\"../src/xinput1_3.def\"" }
  links { "meow-hook" }
  defines { "NOMINMAX" }

  files {
    "src/**.h",
    "src/**.cc",
    "src/xinput1_3.def",
  }

  includedirs {
    "src",
    "deps/meow-hook/include"
  }

  postbuildcommands {
    "copy /Y \"$(TargetDir)$(ProjectName).dll\" \"D:/Steam/steamapps/common/Fallout76/$(ProjectName).dll\"",
    "Exit 0"
  }

group "deps"
  project "asmjit"
    kind "StaticLib"
    defines "ASMJIT_STATIC"
    files "deps/meow-hook/third_party/asmjit/src/**"
    includedirs "deps/meow-hook/third_party/asmjit/src"

  project "zydis"
    kind "StaticLib"
    defines {
      "ZYCORE_STATIC_DEFINE",
      "ZYDIS_STATIC_DEFINE"
    }

    includedirs {
      "deps/meow-hook/third_party/zydis/src",
      "deps/meow-hook/third_party/zydis/include",
      "deps/meow-hook/third_party/zydis/msvc",
      "deps/meow-hook/third_party/zydis/dependencies/zycore/include"
    }

    files {
      "deps/meow-hook/third_party/zydis/dependencies/zycore/src/**",
      "deps/meow-hook/third_party/zydis/dependencies/zycore/include/**",
      "deps/meow-hook/third_party/zydis/src/**",
      "deps/meow-hook/third_party/zydis/include/Zydis/**",
    }

  project "meow-hook"
    kind "StaticLib"

    links { "asmjit", "zydis" }

    defines {
      "NOMINMAX",
      "ASMJIT_STATIC",
      "ZYCORE_STATIC_DEFINE",
      "ZYDIS_STATIC_DEFINE"
    }

    includedirs {
      "deps/meow-hook/include",
      "deps/meow-hook/third_party/asmjit/src",
      "deps/meow-hook/third_party/zydis/src",
      "deps/meow-hook/third_party/zydis/include",
      "deps/meow-hook/third_party/zydis/msvc",
      "deps/meow-hook/third_party/zydis/dependencies/zycore/include"
    }

    files {
      "deps/meow-hook/src/*.cc",
      "deps/meow-hook/src/*.h",
      "deps/meow-hook/include/meow_hook/*.h",
    }