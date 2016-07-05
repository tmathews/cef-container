set(TARGET_NAME cef-container)

set(SRCS
	WinMain.cpp
	BrowserApp.h
	BrowserApp.cpp
	BrowserHandler.h
	BrowserHandler.cpp
	BrowserWindowDelegate.h
	BrowserWindowDelegate.cpp
	RendererApp.h
	RendererApp.cpp
	Messages.h
	Messages.cpp
	Content.h
	Content.cpp
	AppConfig.h
	PlatformUtils.h
	PlatformUtils.cpp
	)

add_executable(${TARGET_NAME} WIN32 MACOSX_BUNDLE ${SRCS})

# Link the CEF import library and the DLL wrapper
target_link_libraries(${TARGET_NAME} ${CEF_LIB_RELEASE})
target_link_libraries(${TARGET_NAME} libcef_dll_wrapper)

# Copy CEF resources
if(WIN32)
	COPY_FILES("${TARGET_NAME}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR_RELEASE}" "$<TARGET_FILE_DIR:${TARGET_NAME}>")
	COPY_FILES("${TARGET_NAME}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "$<TARGET_FILE_DIR:${TARGET_NAME}>")
endif()

# Copy content
add_custom_command(
	TARGET ${TARGET_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/content $<TARGET_FILE_DIR:${TARGET_NAME}>/content
	)
