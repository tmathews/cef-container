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

if(WIN32)
	set(SRCS ${SRCS} WinMain.cpp)
endif()

add_executable(${CEF_TARGET} WIN32 MACOSX_BUNDLE ${SRCS})

target_link_libraries(${CEF_TARGET} RapidJSON)

# Link the CEF import library and the DLL wrapper
add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
target_link_libraries(${CEF_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

# Copy CEF resources
if(WIN32)
	ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET}" "exe")

	COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
	COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
endif()

# Copy content
add_custom_command(
	TARGET ${CEF_TARGET} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/content $<TARGET_FILE_DIR:${CEF_TARGET}>/content
	)
