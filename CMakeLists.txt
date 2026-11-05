cmake_minimum_required(VERSION 3.24)
project(Lab2 LANGUAGES C CXX)
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Configuració de compilació" FORCE)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LAB_SOURCES exercises/Matrix3x3.cpp exercises/Quat.cpp
    exercises/Matrix3x3.hpp exercises/Quat.hpp
    support/Geometry.cpp support/Geometry.hpp support/Validation.cpp support/Validation.hpp)
add_library(exercises ${LAB_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${LAB_SOURCES})
target_include_directories(exercises PUBLIC exercises support)
if(MSVC)
    target_compile_options(exercises PUBLIC /utf-8)
    target_compile_options(exercises PRIVATE /W4)
else()
    target_compile_options(exercises PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(LAB2_BUILD_UI "Compila la interfície SDL3 / OpenGL / Dear ImGui" ON)
if(NOT LAB2_BUILD_UI)
    return()
endif()
include(FetchContent)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SDL3 URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-3.2.30.tar.gz
    URL_HASH SHA256=8bfb8f70a72f216dff66cde64b9d7b863d5e8aa46be7f2fd400d7f1f9baafe76
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_Declare(imgui URL https://github.com/ocornut/imgui/archive/refs/tags/v1.92.6.tar.gz
    URL_HASH SHA256=5b17c01f69545bde732b14936d89ce0f508adb83e8b56fa82448371845172bc3
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(SDL3 imgui)
find_package(OpenGL REQUIRED)
add_library(imgui STATIC ${imgui_SOURCE_DIR}/imgui.cpp ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC SDL3::SDL3 OpenGL::GL)
add_executable(lab2 app/main_app.cpp)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT lab2)
target_link_libraries(lab2 PRIVATE exercises imgui)
set_target_properties(lab2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
foreach(config Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${config} upper)
    set_target_properties(lab2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${upper} "${CMAKE_BINARY_DIR}/bin")
endforeach()
install(TARGETS lab2 RUNTIME DESTINATION .)
