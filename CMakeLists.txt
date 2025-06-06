cmake_minimum_required(VERSION 3.30)
project(minimalistic-raytracer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/lib/Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/Release")

option(SDL_SHARED       "Build SDL as a shared library" ON)
option(SDL_STATIC       "Build SDL as a static library" OFF)
option(SDL_TEST_LIBRARY "Build the SDL3_test static library" OFF)

add_subdirectory(Vendor/SDL)
add_subdirectory(Vendor/IMGUI)

set(SRC_FILES
	Source/main.cpp
	#App
	Source/Private/App/Window.cpp
	# Image
	Source/Private/Image/Image.cpp
	Source/Private/Image/PPMHandler.cpp
	# Math
	Source/Private/Math/Hittable.cpp
	Source/Private/Math/BoundingSphere.cpp
)

set(INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/Source/Public

	${CMAKE_SOURCE_DIR}/Vendor
	${CMAKE_SOURCE_DIR}/Vendor/SDL/include
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 IMGUI::IMGUI)
target_compile_options(${PROJECT_NAME} PRIVATE -w)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})