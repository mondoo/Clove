--GLOBALS
outputdir = "%{cfg.platform}/%{cfg.buildcfg}"

targetdir_clv = "Built/bin/"..outputdir.."/%{prj.name}"
objdir_clv = "Built/intermediate/"..outputdir.."/%{prj.name}"

targetdir_vendor = "Built/bin/"..outputdir.."/vendor/%{prj.name}"
objdir_vendor = "Built/intermediate/"..outputdir.."/vendor/%{prj.name}"

hlslout_dir = "../Sandbox/"
hlslout_com = hlslout_dir.."%{file.basename}"..".cso"

--Workspace Settings
workspace "Clove"
	startproject "Sandbox"

	systemversion "latest"
	cppdialect "C++17"

	configurations{
		"Debug",
		"Development",
		"Profiling",
		"Release"
	}

	platforms{
		"Win32",
		"Win64",
		"Linux",
	}

	defines{
		"_ENABLE_EXTENDED_ALIGNED_STORAGE"
	}

	--Platform filters
	filter "platforms:Win32"
		system "windows"
		architecture "x86"

	filter "platforms:Win64"
		system "windows"
		architecture "x86_64"

	filter "platforms:Linux"
		system "linux"
		architecture "x86_64"

	--System filters
	filter "system:windows"
		defines{
			"CLV_PLATFORM_WINDOWS=1"
		}

	filter "system:linux"
		defines{
			"CLV_PLATFORM_LINUX=1"
		}

	--Configuration filters
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"
		warnings "Extra"

		defines {
			"CLV_DEBUG=1"
		}
		
	filter "configurations:Development"
		runtime "Debug"
		symbols "On"
		optimize "Debug"
		warnings "Default"

		defines{
			"CLV_DEVELOPMENT=1"
		}
		
	filter "configurations:Profiling"
		runtime "Release"
		symbols "Off"
		optimize "On"
		warnings "Off"

		defines{
			"CLV_PROFILING=1"
		}

	filter "configurations:Release"
		runtime "Release"
		symbols "Off"
		optimize "Full"
		warnings "Off"

		defines{
			"CLV_RELEASE=1"
		}
	
	filter ""

--Clove Dependencies
group "Dependencies"
--GLAD
project "Glad"
	location "Clove/vendor/Glad"
    kind "StaticLib"
	staticruntime "on"
	language "C"
    
	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	files{
        "%{prj.location}/include/**.h",
        "%{prj.location}/src/**.c"
    }

	includedirs{
		"%{prj.location}/include"
	}

--dxerr
project "dxerr"
	location "Clove/vendor/dxerr"
	kind "StaticLib"
	staticruntime "on"
	language "C++"

	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	files{
		"%{prj.location}/**.h",
		"%{prj.location}/**.inl",
		"%{prj.location}/**.cpp",
	}
	
--stb
project "stb"
	location "Clove/vendor/stb"
	kind "StaticLib"
	staticruntime "on"
	language "C++"
	
	targetdir(targetdir_vendor)
	objdir(objdir_vendor)
	
	files{
		"%{prj.location}/**.h",
		"%{prj.location}/**.cpp",
	}

project "PortAudio"
	location "Clove/vendor/portaudio"
	kind "StaticLib"
	staticruntime "on"
	language "C"

	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	--converted from CMakeLists.txt in "Clove/vendor/portaudio"
	files{
		"%{prj.location}/include/portaudio.h",

		"%{prj.location}/src/common/pa_allocation.h",
		"%{prj.location}/src/common/pa_allocation.c",
  		"%{prj.location}/src/common/pa_converters.h",
		"%{prj.location}/src/common/pa_converters.c",
  		"%{prj.location}/src/common/pa_cpuload.h",
		"%{prj.location}/src/common/pa_cpuload.c",
  		"%{prj.location}/src/common/pa_debugprint.h",
		"%{prj.location}/src/common/pa_debugprint.c",
  		"%{prj.location}/src/common/pa_dither.h",
		"%{prj.location}/src/common/pa_dither.c",
		"%{prj.location}/src/common/pa_process.h",
		"%{prj.location}/src/common/pa_process.c",
		"%{prj.location}/src/common/pa_ringbuffer.h",
		"%{prj.location}/src/common/pa_ringbuffer.c",
		"%{prj.location}/src/common/pa_stream.h",
		"%{prj.location}/src/common/pa_stream.c",
		"%{prj.location}/src/common/pa_trace.h",
		"%{prj.location}/src/common/pa_trace.c",
		
		"%{prj.location}/src/common/pa_endianness.h",
		"%{prj.location}/src/common/pa_hostapi.h",
		"%{prj.location}/src/common/pa_memorybarrier.h",
  		"%{prj.location}/src/common/pa_types.h",
		"%{prj.location}/src/common/pa_util.h",
		  
		"%{prj.location}/src/common/pa_front.c",
	}

	includedirs{
		"%{prj.location}/include/",
		"%{prj.location}/src/common",
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
	}

	filter "system:windows"
		files{
			"%{prj.location}/include/pa_win_ds.h",
			"%{prj.location}/include/pa_win_waveformat.h",

			"%{prj.location}/src/os/win/pa_win_wdmks_utils.h",
			"%{prj.location}/src/os/win/pa_win_wdmks_utils.c",
			"%{prj.location}/src/os/win/pa_win_coinitialize.h",
			"%{prj.location}/src/os/win/pa_win_coinitialize.c",
			"%{prj.location}/src/hostapi/dsound/pa_win_ds_dynlink.h",
			"%{prj.location}/src/hostapi/dsound/pa_win_ds_dynlink.c",

			"%{prj.location}/src/os/win/pa_win_hostapis.c",
			"%{prj.location}/src/os/win/pa_win_waveformat.c",
			"%{prj.location}/src/os/win/pa_win_util.c",
			"%{prj.location}/src/hostapi/dsound/pa_win_ds.c"
		}

		includedirs{
			"%{prj.location}/src/os/win"
		}

		defines{
			"PA_USE_DS",
			"PAWIN_USE_DIRECTSOUNDFULLDUPLEXCREATE"
		}
	
	filter "system:linux"
		files{
			"%{prj.location}/src/os/unix/pa_unix_hostapis.c",
			"%{prj.location}/src/os/unix/pa_unix_util.c",
			"%{prj.location}/include/pa_linux_alsa.h",
			"%{prj.location}/src/hostapi/alsa/pa_linux_alsa.c",
		}

		includedirs{
			"%{prj.location}/src/os/unix"
		}

		defines{
			"PA_USE_ALSA"
		}

project "libsndfile"
	location "Clove/vendor/libsndfile"
	kind "StaticLib"
	staticruntime "on"
	language "C"

	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	--converted from CMakeLists.txt in "Clove/vendor/libsndfile"
	files{
		"%{prj.location}/src/sfconfig.h",
		"%{prj.location}/src/sfendian.h",
		"%{prj.location}/src/sf_unistd.h",
		"%{prj.location}/src/common.h",
		"%{prj.location}/src/common.c",
		"%{prj.location}/src/file_io.c",
		"%{prj.location}/src/command.c",
		"%{prj.location}/src/pcm.c",
		"%{prj.location}/src/ulaw.c",
		"%{prj.location}/src/alaw.c",
		"%{prj.location}/src/float32.c",
		"%{prj.location}/src/double64.c",
		"%{prj.location}/src/ima_adpcm.c",
		"%{prj.location}/src/ms_adpcm.c",
		"%{prj.location}/src/gsm610.c",
		"%{prj.location}/src/dwvw.c",
		"%{prj.location}/src/vox_adpcm.c",
		"%{prj.location}/src/interleave.c",
		"%{prj.location}/src/strings.c",
		"%{prj.location}/src/dither.c",
		"%{prj.location}/src/cart.c",
		"%{prj.location}/src/broadcast.c",
		"%{prj.location}/src/audio_detect.c",
 		"%{prj.location}/src/ima_oki_adpcm.c",
		"%{prj.location}/src/ima_oki_adpcm.h",
		"%{prj.location}/src/alac.c",
		"%{prj.location}/src/chunk.c",
		"%{prj.location}/src/ogg.h",
		"%{prj.location}/src/ogg.c",
		"%{prj.location}/src/chanmap.h",
		"%{prj.location}/src/chanmap.c",
		"%{prj.location}/src/id3.c",
		"%{prj.location}/src/sndfile.c",
		"%{prj.location}/src/aiff.c",
		"%{prj.location}/src/au.c",
		"%{prj.location}/src/avr.c",
		"%{prj.location}/src/caf.c",
		"%{prj.location}/src/dwd.c",
		"%{prj.location}/src/flac.c",
		"%{prj.location}/src/g72x.c",
		"%{prj.location}/src/htk.c",
		"%{prj.location}/src/ircam.c",
		"%{prj.location}/src/macos.c",
		"%{prj.location}/src/mat4.c",
		"%{prj.location}/src/mat5.c",
		"%{prj.location}/src/nist.c",
		"%{prj.location}/src/paf.c",
		"%{prj.location}/src/pvf.c",
		"%{prj.location}/src/raw.c",
		"%{prj.location}/src/rx2.c",
		"%{prj.location}/src/sd2.c",
		"%{prj.location}/src/sds.c",
		"%{prj.location}/src/svx.c",
		"%{prj.location}/src/txw.c",
		"%{prj.location}/src/voc.c",
		"%{prj.location}/src/wve.c",
		"%{prj.location}/src/w64.c",
		"%{prj.location}/src/wavlike.h",
		"%{prj.location}/src/wavlike.c",
		"%{prj.location}/src/wav.c",
		"%{prj.location}/src/xi.c",
		"%{prj.location}/src/mpc2k.c",
		"%{prj.location}/src/rf64.c",
		"%{prj.location}/src/ogg_vorbis.c",
		"%{prj.location}/src/ogg_speex.c",
		"%{prj.location}/src/ogg_pcm.c",
		"%{prj.location}/src/ogg_opus.c",
		"%{prj.location}/src/ogg_vcomment.c",
		"%{prj.location}/src/nms_adpcm.c",
		"%{prj.location}/src/GSM610/config.h",
		"%{prj.location}/src/GSM610/gsm.h",
		"%{prj.location}/src/GSM610/gsm610_priv.h",
		"%{prj.location}/src/GSM610/add.c",
		"%{prj.location}/src/GSM610/code.c",
		"%{prj.location}/src/GSM610/decode.c",
		"%{prj.location}/src/GSM610/gsm_create.c",
		"%{prj.location}/src/GSM610/gsm_decode.c",
		"%{prj.location}/src/GSM610/gsm_destroy.c",
		"%{prj.location}/src/GSM610/gsm_encode.c",
		"%{prj.location}/src/GSM610/gsm_option.c",
		"%{prj.location}/src/GSM610/long_term.c",
		"%{prj.location}/src/GSM610/lpc.c",
		"%{prj.location}/src/GSM610/preprocess.c",
		"%{prj.location}/src/GSM610/rpe.c",
		"%{prj.location}/src/GSM610/short_term.c",
		"%{prj.location}/src/GSM610/table.c",
		"%{prj.location}/src/G72x/g72x.h",
		"%{prj.location}/src/G72x/g72x_priv.h",
		"%{prj.location}/src/G72x/g721.c",
		"%{prj.location}/src/G72x/g723_16.c",
		"%{prj.location}/src/G72x/g723_24.c",
		"%{prj.location}/src/G72x/g723_40.c",
		"%{prj.location}/src/G72x/g72x.c",
		"%{prj.location}/src/ALAC/ALACAudioTypes.h",
		"%{prj.location}/src/ALAC/ALACBitUtilities.h",
		"%{prj.location}/src/ALAC/EndianPortable.h",
		"%{prj.location}/src/ALAC/aglib.h",
		"%{prj.location}/src/ALAC/dplib.h",
		"%{prj.location}/src/ALAC/matrixlib.h",
		"%{prj.location}/src/ALAC/alac_codec.h",
		"%{prj.location}/src/ALAC/shift.h",
		"%{prj.location}/src/ALAC/ALACBitUtilities.c",
		"%{prj.location}/src/ALAC/ag_dec.c",
		"%{prj.location}/src/ALAC/ag_enc.c",
		"%{prj.location}/src/ALAC/dp_dec.c",
		"%{prj.location}/src/ALAC/dp_enc.c",
		"%{prj.location}/src/ALAC/matrix_dec.c",
		"%{prj.location}/src/ALAC/matrix_enc.c",
		"%{prj.location}/src/ALAC/alac_decoder.c",
		"%{prj.location}/src/ALAC/alac_encoder.c",
		"%{prj.location}/src/sndfile.hh",
		"%{prj.location}/src/sndfile.h",
		"%{prj.location}/src/config.h",
	}

	includedirs{
		"%{prj.location}/src"
	}

	defines{
		"_USE_MATH_DEFINES",
		"_CRT_SECURE_NO_WARNINGS"
	}

	postbuildcommands{
		("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
	}

	filter "system:windows"
		files{
			"%{prj.location}/src/windows.c",
		}

--End: Dependencies

--CLOVE
-- Inlcude direction relative to the roof folder (solution directory)
includeDir = {}
includeDir["Glad"]		= "Clove/vendor/Glad/include"
includeDir["glm"]		= "Clove/vendor/glm"
includeDir["stb"]		= "Clove/vendor/stb"
includeDir["dxerr"]		= "Clove/vendor/dxerr"
includeDir["portaudio"] = "Clove/vendor/portaudio/include"
includeDir["libsndfile"] = "Clove/vendor/libsndfile/src"

group ""
project "Clove"
	location "Clove"
	kind "StaticLib"
	staticruntime "on"
	language "C++"

	targetdir(targetdir_clv)
	objdir(objdir_clv)

	pchheader "clvpch.hpp"
	pchsource "Clove/src/clvpch.cpp"

	files{
		"%{prj.location}/src/**.hpp",
		"%{prj.location}/src/**.inl",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/src/**.glsl",
	}

	includedirs{
		--Clove
		"%{prj.location}/src",
		
		--Libs
		"%{includeDir.Glad}",
		"%{includeDir.glm}",
		"%{includeDir.stb}",
		"%{includeDir.dxerr}",
		"%{includeDir.portaudio}",
		"%{includeDir.libsndfile}",

		--Misc vendor
		"%{prj.location}/vendor/spdlog/include",
		"%{prj.location}/vendor/OBJ-Loader/Source",
		"%{prj.location}/vendor/Event-Dispatcher",
	}

	links{
		"Glad",
		"stb",
		"portaudio",
		"libsndfile"
	}

	defines{
		"CLV_ENGINE=1"
	}

	--platform specifics
	filter "system:windows"
		files{
			"%{prj.location}/src/**.hlsl",
		}

		removefiles{
			"%{prj.location}/src/Platform/Linux/**.hpp",
			"%{prj.location}/src/Platform/Linux/**.inl",
			"%{prj.location}/src/Platform/Linux/**.cpp",
			
			"%{prj.location}/src/Graphics/OpenGL-4/GLXContext.*",
		}

		includedirs{
			"%{prj.location}/vendor/wglext",
		}

		links{
			"dxerr",
			"opengl32.lib",
			"d3d11.lib",
			"d3dcompiler.lib",
		}

		postbuildcommands{
			("{COPY} "..hlslout_dir.."**.cso \"../Built/bin/"..outputdir.."/Sandbox/\"")
		}

	filter "system:linux"
		removefiles{
			"%{prj.location}/src/Platform/Windows/**.hpp",
			"%{prj.location}/src/Platform/Windows/**.inl",
			"%{prj.location}/src/Platform/Windows/**.cpp",

			"%{prj.location}/src/Graphics/DirectX-11/**.hpp",
			"%{prj.location}/src/Graphics/DirectX-11/**.inl",
			"%{prj.location}/src/Graphics/DirectX-11/**.cpp",

			"%{prj.location}/src/Graphics/OpenGL-4/WGLContext.*",
		}

		links{
			"GLX",
			"X11"
		}

	--hlsl shaders
	filter "files:**.hlsl"
		shadermodel "5.0"
		shaderobjectfileoutput(hlslout_com)
	filter "files:**-vs.hlsl"
		shadertype "Vertex"
	filter "files:**-ps.hlsl"
		shadertype "Pixel"

	filter ""

--Sandbox
group ""
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	staticruntime "on"
	language "C++"

	targetdir(targetdir_clv)
	objdir(objdir_clv)

	files{
		"%{prj.location}/src/**.hpp",
		"%{prj.location}/src/**.inl",
		"%{prj.location}/src/**.cpp"
	}

	includedirs{
		"Clove/vendor/spdlog/include",

		"%{includeDir.Glad}",
		"%{includeDir.glm}",

		"Clove/src"
	}

	links{
		"Clove",
	}

	--gmake doesn't unpack static dependencies the same way msvc does
	filter "action:gmake2"
		links{
			"Glad",
			"GLX",
			"X11",
			"stb",
			"dl",
			"portaudio",
			"pthread",
			"asound",
			"rt",
			"m"
		}