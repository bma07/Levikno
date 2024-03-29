cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)

# Levikno
project(Levikno)

# Architecture
set(ARCHITECTURE_TYPE x64)

# Platform
if (WIN32)
	add_definitions(-DLVN_PLATFORM_WINDOWS)
	set(PLATFORM_TYPE windows)

elseif(APPLE)
	add_definitions(-DLVN_PLATFORM_MACOS)
	set(PLATFORM_TYPE macos)

elseif(UNIX AND NOT APPLE)
	add_definitions(-DLVN_PLATFORM_LINUX)
	set(PLATFORM_TYPE linux)

else()
	add_definitions(-DLVN_PLATFORM_UNKNOWN)
endif()

# Output dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${PLATFORM_TYPE}-${ARCHITECTURE_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${PLATFORM_TYPE}-${ARCHITECTURE_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLATFORM_TYPE}-${ARCHITECTURE_TYPE})


# Subdirectories ---------------------------------------- /

# GLFW
# set(GLFW_BUILD_WAYLAND 1)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/Levikno/src/dependencies/glfw)


# Source build files

# Graphics
set(LVN_GRAPHICS_SRC
	# glad
	Levikno/src/dependencies/glad/include/glad/glad.h
	Levikno/src/dependencies/glad/src/glad.c

	# vulkan
	Levikno/src/platform/api/vulkan/lvn_vulkan.cpp
	Levikno/src/platform/api/vulkan/lvn_vulkan.h
	Levikno/src/platform/api/vulkan/lvn_vulkanBackends.h
)

set(LVN_SOURCES
	Levikno/include/levikno/levikno.h
	Levikno/src/levikno.cpp
	Levikno/src/levikno_internal.h

	# glfw
	Levikno/src/platform/window/glfw/lvn_glfw.cpp
	Levikno/src/platform/window/glfw/lvn_glfw.h

	# vma
	Levikno/src/dependencies/vma/include/vk_mem_alloc.cpp
	Levikno/src/dependencies/vma/include/vk_mem_alloc.h

	# stb
	Levikno/src/dependencies/stb/include/stb_image.cpp
	Levikno/src/dependencies/stb/include/stb_image.h

  ${LVN_GRAPHICS_SRC}
)

add_library(levikno STATIC ${LVN_SOURCES})

target_include_directories(levikno
	PRIVATE
		${PROJECT_SOURCE_DIR}/Levikno/src
		${PROJECT_SOURCE_DIR}/Levikno/include/levikno
		${PROJECT_SOURCE_DIR}/Levikno/src/platform/api/vulkan
		${PROJECT_SOURCE_DIR}/Levikno/src/platform/window/glfw
		${PROJECT_SOURCE_DIR}/Levikno/src/dependencies/glfw/include
		${PROJECT_SOURCE_DIR}/Levikno/src/dependencies/glad/include
		${PROJECT_SOURCE_DIR}/Levikno/src/dependencies/vulkan/include
		${PROJECT_SOURCE_DIR}/Levikno/src/dependencies/vma/include
		${PROJECT_SOURCE_DIR}/Levikno/src/dependencies/stb/include
)


# Build definitions
target_compile_definitions(levikno PRIVATE
	$<$<CONFIG:Debug>:LVN_DEBUG>
	$<$<CONFIG:Release>:LVN_RELEASE>
)

# Find VulkanSDK
set(MIN_VULKAN_VERSION 1.3)
find_package(Vulkan ${MIN_VULKAN_VERSION} REQUIRED)

if(Vulkan_FOUND)
    message ("Vulkan found")
else()
    message (FATAL_ERROR "Cannot find Vulkan")
endif()

set(VULKAN_LIB_DIR
	Vulkan::Vulkan
)

target_link_libraries(levikno
	PRIVATE 
		glfw
		${VULKAN_LIB_DIR}
)

# LeviknoEditor
set(LVN_EDITOR_SOURCES
	LeviknoEditor/src/Main.cpp
)

add_executable(leviknoEditor ${LVN_EDITOR_SOURCES})

target_include_directories(leviknoEditor 
		PRIVATE
		${PROJECT_SOURCE_DIR}/LeviknoEditor
		${PROJECT_SOURCE_DIR}/LeviknoEditor/src
		${PROJECT_SOURCE_DIR}/Levikno/include
)

target_link_libraries(leviknoEditor
	PRIVATE
		levikno
)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT leviknoEditor)	
endif()
