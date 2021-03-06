# Source Files
set(SOURCE_FILES

dllmain.cpp
Pools.cpp
WaterBlocks.cpp
StaticMatrices.cpp
AlphaEntityList.cpp

)


# The current directory shall be used for #include <file>
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Visual Studio specific stuff
if(MSVC)
    set(ASM_EXT "c")	 # Use C with declspec(naked) instead

    # We don't want to deal with Buffer Security Check under "Release" build.
    set(CMAKE_CXX_FLAGS_RELEASE "/GS- ${CMAKE_CXX_FLAGS_RELEASE}")

    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
endif(MSVC)

# MiNGW specific stuff
if(MINGW)

    # Enable ASM files
    enable_language(C CXX ASM)

    set(ASM_EXT "s")  # GAS Assembly extension

    # Compiling C++11 source (with GNU and Microsoft extensions)
    set(CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
    
    # Force static runtime
    # As you may know Windows DLLs and GNU standard library do not work very well together since the GNU librares have different exception models within the same DLL name, that's real bad.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static")

endif(MINGW)


add_library(limit_adjuster_gta3vcsa SHARED ${SOURCE_FILES})
set_target_properties(limit_adjuster_gta3vcsa PROPERTIES SUFFIX ".asi")
install(TARGETS limit_adjuster_gta3vcsa RUNTIME DESTINATION "./")

