cmake_minimum_required(VERSION 3.5)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build all libs static" FORCE)

project(FFMPEG_video_player)
set(CMAKE_CXX_STANDARD 23)

set(SDL_SHARED OFF CACHE BOOL "Build SDL shared" FORCE)
set(SDL_STATIC ON  CACHE BOOL "Build SDL static" FORCE)

add_subdirectory("thirdparty/SDL" EXCLUDE_FROM_ALL)
set(FFMPEG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ffmpeg-static-build" CACHE PATH "Root directory for static FFmpeg build")

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(FFMPEG_video_player "${MY_SOURCES}")

target_include_directories(FFMPEG_video_player PRIVATE
        "${FFMPEG_ROOT}/include"
)

if (WIN32)
    set(FFMPEG_LIBS
            "${FFMPEG_ROOT}/lib/libavformat.a"
            "${FFMPEG_ROOT}/lib/libavcodec.a"
            "${FFMPEG_ROOT}/lib/libswscale.a"
            "${FFMPEG_ROOT}/lib/libavutil.a"

            ws2_32
            advapi32
            shell32
            user32
            bcrypt
    )
elseif (UNIX OR APPLE)
    set(FFMPEG_LIBS
            "${FFMPEG_ROOT}/lib/libavformat.a"
            "${FFMPEG_ROOT}/lib/libavcodec.a"
            "${FFMPEG_ROOT}/lib/libswscale.a"
            "${FFMPEG_ROOT}/lib/libavutil.a"

            -lz
            -lm
            -pthread
    )
else ()
    message(FATAL_ERROR "Unsupported operating system for static FFmpeg dependencies.")
endif ()

target_link_libraries(FFMPEG_video_player PRIVATE
    SDL3::SDL3-static
    ${FFMPEG_LIBS}
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(FFMPEG_video_player PUBLIC RESOURCES_PATH="./resources/")
else()
    # For IDE development
    target_compile_definitions(FFMPEG_video_player PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
endif()
