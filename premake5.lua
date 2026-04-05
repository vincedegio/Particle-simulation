workspace "ParticleSim"
    architecture "x64"
    configurations { "Debug", "Release" }
    location "build"

-- Force GCC/Clang toolsets
filter "system:windows"
    toolset "gcc"

filter "system:linux"
    toolset "gcc"

filter "system:macosx"
    toolset "clang"

filter {}

-- Auto-clone Raylib
if not os.isdir("raylib") then
    print("Raylib not found. Cloning...")
    os.execute("git clone https://github.com/raysan5/raylib.git")
end

---------------------------------------------------------
-- Raylib static library
---------------------------------------------------------
project "raylib"
    kind "StaticLib"
    language "C"
    cdialect "C11"

    targetdir ("build/raylib/%{cfg.buildcfg}")
    objdir ("build/raylib/obj/%{cfg.buildcfg}")

    includedirs {
        "raylib/src",
        "raylib/src/external/glfw/include"
    }

    -- Core Raylib sources
    files {
        "raylib/src/*.c",
        "raylib/src/*.h"
    }

    -----------------------------------------------------
    -- WINDOWS (MinGW-w64 GCC)
    -----------------------------------------------------
    filter "system:windows"
        defines {
            "PLATFORM_DESKTOP",
            "SUPPORT_FILEFORMAT_QOA=0"   -- QOA broken on MinGW
        }

        -- Only Windows GLFW backend
        files {
            "raylib/src/external/glfw/src/context.c",
            "raylib/src/external/glfw/src/init.c",
            "raylib/src/external/glfw/src/input.c",
            "raylib/src/external/glfw/src/monitor.c",
            "raylib/src/external/glfw/src/platform.c",
            "raylib/src/external/glfw/src/vulkan.c",

            "raylib/src/external/glfw/src/win32_init.c",
            "raylib/src/external/glfw/src/win32_joystick.c",
            "raylib/src/external/glfw/src/win32_module.c",
            "raylib/src/external/glfw/src/win32_monitor.c",
            "raylib/src/external/glfw/src/win32_thread.c",
            "raylib/src/external/glfw/src/win32_time.c",
            "raylib/src/external/glfw/src/win32_window.c",
            "raylib/src/external/glfw/src/wgl_context.c"
        }

        links { "winmm", "gdi32", "opengl32" }

    -----------------------------------------------------
    -- LINUX (GCC)
    -----------------------------------------------------
    filter "system:linux"
        defines {
            "PLATFORM_DESKTOP",
            "SUPPORT_FILEFORMAT_QOA=1"
        }

        files {
            "raylib/src/external/glfw/src/context.c",
            "raylib/src/external/glfw/src/init.c",
            "raylib/src/external/glfw/src/input.c",
            "raylib/src/external/glfw/src/monitor.c",
            "raylib/src/external/glfw/src/platform.c",
            "raylib/src/external/glfw/src/vulkan.c",

            "raylib/src/external/glfw/src/x11_init.c",
            "raylib/src/external/glfw/src/x11_monitor.c",
            "raylib/src/external/glfw/src/x11_window.c",
            "raylib/src/external/glfw/src/xkb_unicode.c",
            "raylib/src/external/glfw/src/posix_time.c",
            "raylib/src/external/glfw/src/posix_thread.c",
            "raylib/src/external/glfw/src/glx_context.c"
        }

        links { "m", "pthread", "dl", "rt", "GL", "X11" }

    -----------------------------------------------------
    -- MACOS (Clang)
    -----------------------------------------------------
    filter "system:macosx"
        defines {
            "PLATFORM_DESKTOP",
            "SUPPORT_FILEFORMAT_QOA=1"
        }

        files {
            "raylib/src/external/glfw/src/context.c",
            "raylib/src/external/glfw/src/init.c",
            "raylib/src/external/glfw/src/input.c",
            "raylib/src/external/glfw/src/monitor.c",
            "raylib/src/external/glfw/src/platform.c",
            "raylib/src/external/glfw/src/vulkan.c",

            "raylib/src/external/glfw/src/cocoa_init.c",
            "raylib/src/external/glfw/src/cocoa_monitor.c",
            "raylib/src/external/glfw/src/cocoa_window.c",
            "raylib/src/external/glfw/src/cocoa_time.c",
            "raylib/src/external/glfw/src/nsgl_context.c",
            "raylib/src/external/glfw/src/posix_thread.c",
            "raylib/src/external/glfw/src/posix_module.c"
        }

        links {
            "m",
            "pthread",
            "dl",
            "objc",
            "OpenGL.framework",
            "Cocoa.framework",
            "IOKit.framework"
        }

    filter {}

---------------------------------------------------------
-- Your C++23 project
---------------------------------------------------------
project "ParticleSim"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"

    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin-int/%{cfg.buildcfg}")

    files {
        "src/**.cpp",
        "include/**.h"
    }

    includedirs {
        "include",
        "raylib/src"
    }

    links {
        "raylib",
        "ws2_32",
        "winmm",
        "gdi32",
        "opengl32"
    }


    filter "system:windows"
        links { "winmm", "gdi32", "opengl32" }

    filter "system:linux"
        links { "m", "pthread", "dl", "rt", "GL", "X11" }

    filter "system:macosx"
        links {
            "m",
            "pthread",
            "dl",
            "objc",
            "OpenGL.framework",
            "Cocoa.framework",
            "IOKit.framework"
        }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"
