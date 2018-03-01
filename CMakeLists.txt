cmake_minimum_required(VERSION 3.8.1)

project(cef-container)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(CEF REQUIRED)
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

if(OS_LINUX)
	find_package(X11 REQUIRED)
endif()

add_library(RapidJSON INTERFACE)
target_include_directories(RapidJSON INTERFACE ${CMAKE_SOURCE_DIR}/vendor/rapidjson/include)

add_subdirectory(src ${CMAKE_SOURCE_DIR}/bin/cef-container)

# Visual Studio IDE configuration
if(MSVC_IDE)
	set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT cef-container)
	set_property(TARGET cef-container PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
endif()
