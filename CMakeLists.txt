cmake_minimum_required(VERSION 3.0)
project(cef-container)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

#----------------------------------------------------------------------------
# CEF setup, this is basically the same as what's in the CMakeLists.txt in
# the vendor/cef folder.
#----------------------------------------------------------------------------
if(MSVC)
	set(CEF_ROOT ${VENDOR_DIR}/cef_win)
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")
	find_package(CEF REQUIRED)
	add_subdirectory(${CEF_ROOT}/libcef_dll ${BIN_DIR}/libcef_dll_wrapper)
	add_library(libcef INTERFACE)
	target_include_directories(libcef INTERFACE ${CEF_ROOT})
	PRINT_CEF_CONFIG()
endif()

add_subdirectory(src ${BIN_DIR}/cef-container)
