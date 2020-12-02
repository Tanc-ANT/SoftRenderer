workspace "SoftRenderer"
	architecture "x64"
	startproject "SoftRenderer"

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
	location "build"

outputdir = "%{cfg.buildcfg}"

project "SoftRenderer"
	location "build"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir (outputdir)
	objdir ("bin-int/" .. outputdir)
	
	includedirs
	{	
		"SoftRenderer/"
	}

	files
	{
		"SoftRenderer/**.h",
		"SoftRenderer/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "off"
	
		includedirs
		{
	
		}
	
		defines
		{
	
		}
	
		links
		{
			"gdi32.lib",
			"user32.lib",
			"winmm.lib"
		}
	
	filter "configurations:Debug"
		defines "_DEBUG"
		runtime	"Debug"
		symbols	"on"
	
	filter "configurations:Release"
		defines "_RELEASE"
		runtime	"Release"
		symbols	"on"
		optimize "Speed"