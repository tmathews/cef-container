set(CEF_TARGET cef-container)

ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

SET_CEF_TARGET_OUT_DIR()

set(SRCS
	App.h
	App.cpp
	BrowserHandler.h
	BrowserHandler.cpp
	BrowserWindowDelegate.h
	BrowserWindowDelegate.cpp
	Messages.h
	Messages.cpp
	Content.h
	Content.cpp
	AppConfig.h
	PlatformUtils.h
	PlatformUtils.cpp
	)

if(OS_WINDOWS)
	set(SRCS ${SRCS} WinMain.cpp)
elseif(OS_LINUX)
	set(SRCS ${SRCS} LinuxMain.cpp)
endif()

add_executable(${CEF_TARGET} WIN32 MACOSX_BUNDLE ${SRCS})

# Link RapidJSON
target_link_libraries(${CEF_TARGET} RapidJSON)

# Link X11
if(OS_LINUX)
	target_include_directories(${CEF_TARGET} PRIVATE ${X11_INCLUDE_DIR})
	target_link_libraries(${CEF_TARGET} ${X11_LIBRARIES})
endif()

# Link the CEF import library and the DLL wrapper
SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
target_link_libraries(${CEF_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

# Copy CEF resources
COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")

# Add manifest
if(OS_WINDOWS)
	ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET}" "exe")
endif()

# Set rpath so that libraries can be placed next to the executable.
if(OS_LINUX)
	set_target_properties(${CEF_TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
	set_target_properties(${CEF_TARGET} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
	set_target_properties(${CEF_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
endif()

# Copy content
add_custom_command(
	TARGET ${CEF_TARGET} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/content $<TARGET_FILE_DIR:${CEF_TARGET}>/content
	)
