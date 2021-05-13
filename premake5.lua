workspace "Sanguine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Sanguine/3rdparty/GLFW/include"
IncludeDir["GLad"] = "Sanguine/3rdparty/GLad/include"
IncludeDir["imgui"] = "Sanguine/3rdparty/imgui/"
IncludeDir["glm"] = "Sanguine/3rdparty/glm/"
IncludeDir["stb_image"] = "Sanguine/3rdparty/stb_image/"
IncludeDir["assimp"] = "Sanguine/3rdparty/assimp/include"
IncludeDir["entt"] = "Sanguine/3rdparty/entt/include"
IncludeDir["yaml_cpp"] = "Sanguine/3rdparty/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Sanguine/3rdparty/ImGuizmo"
IncludeDir["asio"] = "Sanguine/3rdparty/asio/include"

include "Sanguine/3rdparty/GLFW"
include "Sanguine/3rdparty/GLad"
include "Sanguine/3rdparty/imgui"
include "Sanguine/3rdParty/assimp"
include "Sanguine/3rdparty/yaml-cpp"

project "Sanguine"
	location "Sanguine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	pchheader "sgpch.h"
	pchsource "Sanguine/src/sgpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		
		"%{prj.name}/3rdparty/glm/glm/**.hpp",
		"%{prj.name}/3rdparty/glm/glm/**.inl",

		"%{prj.name}/3rdparty/stb_image/**.h",
		"%{prj.name}/3rdparty/stb_image/**.cpp",

		"%{prj.name}/3rdparty/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/3rdparty/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/3rdparty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.asio}"
	}

	links
	{
		"GLFW",
		"GLad",
		"ImGui",
		"opengl32.lib",
		"assimp",
		"yaml-cpp"
	}

	filter "files:3rdparty/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SG_PLATFORM_WINDOWS",
			"SG_BUILD_DLL",
			"SG_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "SG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SG_RELEASE"
		runtime "Release"
		optimize "On"

project "SanguineEditor"
	location "SanguineEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Sanguine/src",
		"Sanguine/3rdparty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.asio}"
	}

	links
	{
		"Sanguine",
		"GLFW",
		"GLad",
		"ImGui",
		"opengl32.lib",
		"assimp",
		"yaml-cpp"
	}

	configuration "windows"
		postbuildcommands { "robocopy /E /mir ../assets/ ../bin/Debug-windows-x86_64/SanguineEditor/assets/" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SG_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "SB_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SB_RELEASE"
		runtime "Release"
		optimize "On"

project "GameClient"
	location "GameClient"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Sanguine/src",
		"Sanguine/3rdparty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.asio}"
	}
	
	links
	{
		"Sanguine",
		"GLFW",
		"GLad",
		"ImGui",
		"opengl32.lib",
		"assimp",
		"yaml-cpp"
	}

	configuration "windows"
		postbuildcommands { "robocopy /E /mir ../assets/ ../bin/Debug-windows-x86_64/SanguineEditor/assets/" }
	
	filter "system:windows"
		systemversion "latest"
	
		defines
		{
			"SG_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "SB_DEBUG"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines "SB_RELEASE"
		runtime "Release"
		optimize "On"

		project "GameServer"
		location "GameServer"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
		
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")
		
		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}
		
		includedirs
		{
			"Sanguine/src",
			"Sanguine/3rdparty/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.GLad}",
			"%{IncludeDir.imgui}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.assimp}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.yaml_cpp}",
			"%{IncludeDir.ImGuizmo}",
			"%{IncludeDir.asio}"
		}
		
		links
		{
			"Sanguine",
			"GLFW",
			"GLad",
			"ImGui",
			"opengl32.lib",
			"assimp",
			"yaml-cpp"
		}
		
		filter "system:windows"
			systemversion "latest"
		
			defines
			{
				"SG_PLATFORM_WINDOWS"
			}
		
		filter "configurations:Debug"
			defines "SB_DEBUG"
			runtime "Debug"
			symbols "On"
		
		filter "configurations:Release"
			defines "SB_RELEASE"
			runtime "Release"
			optimize "On"