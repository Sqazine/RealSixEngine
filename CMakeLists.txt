cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME RealSix)
set(EDITOR_NAME "${PROJECT_NAME}Editor")

project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

configure_file(Core/Version.hpp.in generated/Version.hpp @ONLY)
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED OFF)

if(MSVC OR ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(REALSIX_LIB_NAME libRealSix CACHE INTERNAL "RealSix library name")
    set(REALSIX_EDITOR_LIB_NAME libRealSixEditor CACHE INTERNAL "RealSix editor name")
else()
    set(REALSIX_LIB_NAME RealSix CACHE INTERNAL "RealSix library name")
    set(REALSIX_EDITOR_LIB_NAME RealSixEditor CACHE INTERNAL "RealSix editor name")
endif()

set(REALSIX_BINARY_DIR ${CMAKE_BINARY_DIR}/Binary)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${REALSIX_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${REALSIX_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${REALSIX_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${REALSIX_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${REALSIX_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${REALSIX_BINARY_DIR})

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "current platform: Linux")
    set(CURRENT_PLATFORM PLATFORM_LINUX)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "current platform: Windows")
    set(CURRENT_PLATFORM PLATFORM_WINDOWS)
elseif (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    message(STATUS "current platform: FreeBSD")
    set(CURRENT_PLATFORM PLATFORM_FREEBSD)
else ()
    message(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
    set(CURRENT_PLATFORM PLATFORM_OTHER)
endif ()

message(STATUS "CURRENT_PLATFORM=${CURRENT_PLATFORM}")

option(REALSIX_BUILD_TEST "build test example" ON)
option(REALSIX_BUILD_STATIC "build libRealSix static library" ON)

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/3rd")
set(CGLTF_INC_DIR "${THIRD_PARTY_DIR}/cgltf")
set(SDL_INC_DIR "${THIRD_PARTY_DIR}/SDL/include")
set(STB_INC_DIR "${THIRD_PARTY_DIR}/stb")
set(IMGUI_INC_DIR "${THIRD_PARTY_DIR}/imgui")
set(IMGUI_BACKEND_INC_DIR "${THIRD_PARTY_DIR}/imgui/backends")
set(SPIRV_REFLECT_INC_DIR "${THIRD_PARTY_DIR}/SPIRV-Reflect")

set(SPIRV_REFLECT_STATIC_LIB ON)
if(REALSIX_BUILD_STATIC)
    set(SDL_STATIC ON)
    set(SDL_STATIC_PIC ON)
    set(SDL_SHARED OFF)
else()
    set(SDL_STATIC OFF)
    set(SDL_STATIC_PIC OFF)
    set(SDL_SHARED ON)
endif()

find_package(Vulkan REQUIRED)
add_subdirectory(${THIRD_PARTY_DIR}/SDL EXCLUDE_FROM_ALL)
add_subdirectory(${THIRD_PARTY_DIR}/SPIRV-Reflect EXCLUDE_FROM_ALL)

if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    set_property(TARGET SDL_uclibc PROPERTY FOLDER 3rd/SDL3)
    
    if(REALSIX_BUILD_STATIC)
        set_property(TARGET SDL3-static PROPERTY FOLDER 3rd/SDL3)
    else()
        set_property(TARGET SDL3-shared PROPERTY FOLDER 3rd/SDL3)
    endif()

    set_property(TARGET SDL3_test PROPERTY FOLDER 3rd/SDL3)

    set_property(TARGET spirv-reflect-static PROPERTY FOLDER 3rd/SPIRV-Reflect)
endif()

file(GLOB GENERATED_SRC "${GENERATED_DIR}/*.cpp" "${GENERATED_DIR}/*.h" "${GENERATED_DIR}/*.hpp"  "${GENERATED_DIR}/*.inl")
file(GLOB ANIMATION_SRC "${CMAKE_SOURCE_DIR}/Animation/*.cpp" "${CMAKE_SOURCE_DIR}/Animation/*.h" "${CMAKE_SOURCE_DIR}/Animation/*.hpp"  "${CMAKE_SOURCE_DIR}/Animation/*.inl")
file(GLOB ANIMATION_IK_SRC "${CMAKE_SOURCE_DIR}/Animation/IK/*.cpp" "${CMAKE_SOURCE_DIR}/Animation/IK/*.h"  "${CMAKE_SOURCE_DIR}/Animation/IK/*.hpp"  "${CMAKE_SOURCE_DIR}/Animation/IK/*.inl")
file(GLOB CONFIG_SRC "${CMAKE_SOURCE_DIR}/Config/*.cpp" "${CMAKE_SOURCE_DIR}/Config/*.h" "${CMAKE_SOURCE_DIR}/Config/*.hpp" "${CMAKE_SOURCE_DIR}/Config/*.inl")
file(GLOB CORE_SRC "${CMAKE_SOURCE_DIR}/Core/*.cpp" "${CMAKE_SOURCE_DIR}/Core/*.h" "${CMAKE_SOURCE_DIR}/Core/*.hpp" "${CMAKE_SOURCE_DIR}/Core/*.inl")
file(GLOB EDITOR_SRC "${CMAKE_SOURCE_DIR}/Editor/*.cpp" "${CMAKE_SOURCE_DIR}/Editor/*.h" "${CMAKE_SOURCE_DIR}/Editor/*.hpp" "${CMAKE_SOURCE_DIR}/Editor/*.inl")
file(GLOB EDITOR_UI_PASS_SRC "${CMAKE_SOURCE_DIR}/Editor/EditorUIPass/*.cpp" "${CMAKE_SOURCE_DIR}/Editor/EditorUIPass/*.h" "${CMAKE_SOURCE_DIR}/Editor/EditorUIPass/*.hpp" "${CMAKE_SOURCE_DIR}/Editor/EditorUIPass/*.inl")
file(GLOB GFX_SRC "${CMAKE_SOURCE_DIR}/Gfx/*.cpp" "${CMAKE_SOURCE_DIR}/Gfx/*.h" "${CMAKE_SOURCE_DIR}/Gfx/*.hpp" "${CMAKE_SOURCE_DIR}/Gfx/*.inl")
file(GLOB GFX_VULKAN_SRC "${CMAKE_SOURCE_DIR}/Gfx/VK/*.cpp" "${CMAKE_SOURCE_DIR}/Gfx/VK/*.h" "${CMAKE_SOURCE_DIR}/Gfx/VK/*.hpp" "${CMAKE_SOURCE_DIR}/Gfx/VK/*.inl")
file(GLOB MATH_SRC "${CMAKE_SOURCE_DIR}/Math/*.cpp" "${CMAKE_SOURCE_DIR}/Math/*.h" "${CMAKE_SOURCE_DIR}/Math/*.hpp" "${CMAKE_SOURCE_DIR}/Math/*.inl")
file(GLOB PLATFORM_SRC "${CMAKE_SOURCE_DIR}/Platform/*.cpp" "${CMAKE_SOURCE_DIR}/Platform/*.h" "${CMAKE_SOURCE_DIR}/Platform/*.hpp" "${CMAKE_SOURCE_DIR}/Platform/*.inl")
file(GLOB PLATFORM_SDL3_SRC "${CMAKE_SOURCE_DIR}/Platform/SDL3/*.cpp" "${CMAKE_SOURCE_DIR}/Platform/SDL3/*.h" "${CMAKE_SOURCE_DIR}/Platform/SDL3/*.hpp" "${CMAKE_SOURCE_DIR}/Platform/SDL3/*.inl")
file(GLOB RENDER_SRC "${CMAKE_SOURCE_DIR}/Render/*.cpp" "${CMAKE_SOURCE_DIR}/Render/*.h" "${CMAKE_SOURCE_DIR}/Render/*.hpp" "${CMAKE_SOURCE_DIR}/Render/*.inl")
file(GLOB SCRIPT_SRC "${CMAKE_SOURCE_DIR}/Script/*.cpp" "${CMAKE_SOURCE_DIR}/Script/*.h" "${CMAKE_SOURCE_DIR}/Script/*.hpp" "${CMAKE_SOURCE_DIR}/Script/*.inl")
file(GLOB FRAMEGRAPH_SRC "${CMAKE_SOURCE_DIR}/Render/FrameGraph/*.cpp" "${CMAKE_SOURCE_DIR}/Render/FrameGraph/*.h" "${CMAKE_SOURCE_DIR}/Render/FrameGraph/*.hpp" "${CMAKE_SOURCE_DIR}/Render/FrameGraph/*.inl")
file(GLOB RESOURCE_SRC "${CMAKE_SOURCE_DIR}/Resource/*.cpp" "${CMAKE_SOURCE_DIR}/Resource/*.h" "${CMAKE_SOURCE_DIR}/Resource/*.hpp" "${CMAKE_SOURCE_DIR}/Resource/*.inl")
file(GLOB STB_SRC "${CMAKE_SOURCE_DIR}/3rd/stb/*.cpp" "${CMAKE_SOURCE_DIR}/3rd/stb/*.h" "${CMAKE_SOURCE_DIR}/3rd/stb/*.hpp" "${CMAKE_SOURCE_DIR}/3rd/stb/*.inl")
file(GLOB IMGUI_SRC "${CMAKE_SOURCE_DIR}/3rd/imgui/*.cpp" "${CMAKE_SOURCE_DIR}/3rd/imgui/*.h" "${CMAKE_SOURCE_DIR}/3rd/imgui/*.hpp" "${CMAKE_SOURCE_DIR}/3rd/imgui/*.inl")
file(GLOB IMGUI_BACKEND_SRC "${IMGUI_BACKEND_INC_DIR}/imgui_impl_sdl3.h" "${IMGUI_BACKEND_INC_DIR}/imgui_impl_sdl3.hpp" "${IMGUI_BACKEND_INC_DIR}/imgui_impl_sdl3.cpp" "${IMGUI_BACKEND_INC_DIR}/imgui_impl_vulkan.cpp" "${IMGUI_BACKEND_INC_DIR}/imgui_impl_vulkan.h")

source_group("generated" FILES ${GENERATED_SRC})
source_group("Animation" FILES ${ANIMATION_SRC})
source_group("Animation/IK" FILES ${ANIMATION_IK_SRC})
source_group("Config" FILES ${CONFIG_SRC})
source_group("Core" FILES ${CORE_SRC})
source_group("Editor" FILES ${EDITOR_SRC})
source_group("Editor/EditorUIPass" FILES ${EDITOR_UI_PASS_SRC})
source_group("Gfx" FILES ${GFX_SRC})
source_group("Gfx/VK" FILES ${GFX_VULKAN_SRC})
source_group("Math" FILES ${MATH_SRC})
source_group("Platform" FILES ${PLATFORM_SRC})
source_group("Platform/SDL3" FILES ${PLATFORM_SDL3_SRC})
source_group("Render" FILES ${RENDER_SRC})
source_group("Render/FrameGraph" FILES ${FRAMEGRAPH_SRC})
source_group("Resource" FILES ${RESOURCE_SRC})
source_group("Script" FILES ${SCRIPT_SRC})
source_group("External/stb" FILES ${STB_SRC})
source_group("External/imgui" FILES ${IMGUI_SRC})
source_group("External/imgui/backends" FILES ${IMGUI_BACKEND_SRC})

set(REALSIX_SRC
    ${GENERATED_SRC}
    ${ANIMATION_SRC}
    ${ANIMATION_IK_SRC}
    ${CONFIG_SRC}
    ${CORE_SRC}
    ${GFX_SRC}
    ${GFX_VULKAN_SRC}
    ${LOGGER_SRC}
    ${MATH_SRC}
    ${PLATFORM_SRC}
    ${PLATFORM_SDL3_SRC}
    ${RENDER_SRC}
    ${FRAMEGRAPH_SRC}
    ${RESOURCE_SRC}
    ${SCRIPT_SRC}
    ${STB_SRC}
)

set(REALSIX_INC_DIRS 
    ${CMAKE_SOURCE_DIR}
    ${GENERATED_DIR}
    ${CMAKE_SOURCE_DIR}/Animation
    ${CMAKE_SOURCE_DIR}/Animation/IK
    ${CMAKE_SOURCE_DIR}/Core
    ${CMAKE_SOURCE_DIR}/Editor
    ${CMAKE_SOURCE_DIR}/Editor/EditorUIPass
    ${CMAKE_SOURCE_DIR}/Gfx
    ${CMAKE_SOURCE_DIR}/Gfx/VK
    ${CMAKE_SOURCE_DIR}/Math
    ${CMAKE_SOURCE_DIR}/Platform
    ${CMAKE_SOURCE_DIR}/Platform/SDL3
    ${CMAKE_SOURCE_DIR}/Render
    ${CMAKE_SOURCE_DIR}/Render/FrameGraph
    ${CMAKE_SOURCE_DIR}/Resource
    ${CMAKE_SOURCE_DIR}/Script

    ${Vulkan_INCLUDE_DIRS}

    ${CGLTF_INC_DIR}
    ${SDL_INC_DIR}
    ${STB_INC_DIR}
    ${SPIRV_REFLECT_INC_DIR}
)

set(THIRDPARTY_LIB 
    SDL3::SDL3
    Vulkan::Vulkan
    spirv-reflect-static
)

set(COMPILE_DEFINITIONS
    ${CURRENT_PLATFORM} 
    ${VULKAN_HPP_DISPATCH_LOADER_DYNAMIC} 
    ${VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL}
)

if(REALSIX_BUILD_STATIC)
    list(APPEND COMPILE_DEFINITIONS REALSIX_BUILD_STATIC)
endif()

add_library(${REALSIX_LIB_NAME} ${REALSIX_SRC})
target_include_directories(${REALSIX_LIB_NAME}  PUBLIC ${REALSIX_INC_DIRS})
target_link_libraries(${REALSIX_LIB_NAME} PRIVATE ${THIRDPARTY_LIB})
target_compile_definitions(${REALSIX_LIB_NAME} PUBLIC ${COMPILE_DEFINITIONS})

list(APPEND EDITOR_SRC  
                        ${IMGUI_SRC}
                        ${IMGUI_BACKEND_SRC}
)

list(APPEND REALSIX_INC_DIRS ${IMGUI_INC_DIR}
                           ${IMGUI_BACKEND_INC_DIR}
)

set(REALSIX_EDITOR_SRC
    ${EDITOR_SRC}
    ${EDITOR_UI_PASS_SRC}
)

add_library(${REALSIX_EDITOR_LIB_NAME} ${REALSIX_EDITOR_SRC})
target_include_directories(${REALSIX_EDITOR_LIB_NAME} PUBLIC ${REALSIX_INC_DIRS} ${CMAKE_SOURCE_DIR}/Editor)
target_link_libraries(${REALSIX_EDITOR_LIB_NAME} PRIVATE ${REALSIX_LIB_NAME})
target_compile_definitions(${REALSIX_EDITOR_LIB_NAME} PUBLIC ${COMPILE_DEFINITIONS})

add_executable(${EDITOR_NAME} ${CMAKE_SOURCE_DIR}/Editor/EditorMainEntry.cc)
target_include_directories(${EDITOR_NAME} PUBLIC ${REALSIX_INC_DIRS} ${CMAKE_SOURCE_DIR}/Editor)
target_link_libraries(${EDITOR_NAME} PRIVATE ${REALSIX_EDITOR_LIB_NAME})
target_compile_definitions(${EDITOR_NAME} PUBLIC ${COMPILE_DEFINITIONS})

if(REALSIX_BUILD_TEST)
    add_subdirectory(Test)
endif()

if(${CURRENT_PLATFORM} STREQUAL "PLATFORM_WINDOWS")
    target_compile_definitions(${EDITOR_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    if(MSVC)
        target_compile_options(${EDITOR_NAME} PRIVATE "/wd4251;" "/wd4819" "/bigobj;")
    endif()
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EDITOR_NAME})
endif()