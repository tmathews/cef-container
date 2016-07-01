set(TARGET_NAME cef-container)

set(SRCS
	WinMain.cpp
	App.h
	App.cpp
	Handler.h
	Handler.cpp
	WindowDelegate.h
	WindowDelegate.cpp
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
