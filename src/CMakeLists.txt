set(CEF_TARGET cef-container)

ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

SET_CEF_TARGET_OUT_DIR()

set(SRCS
	platform/PlatformUtils.h
	content/ContentHandlerFactory.h
	content/ContentHandlerFactory.cpp
	content/ContentResourceHandler.h
	content/ContentResourceHandler.cpp
	content/io/Archive.h
	content/io/FileStream.h
	content/io/StandardIoArchive.h
	content/io/StandardIoArchive.cpp
	content/io/StandardIoFileStream.h
	content/io/StandardIoFileStream.cpp
	core/BrowserHandler.h
	core/BrowserHandler.cpp
	core/BrowserWindowDelegate.h
	core/BrowserWindowDelegate.cpp
	core/IconDownloadCallback.h
	core/IconDownloadCallback.cpp
	core/AppConfig.h
	core/AppConfig.cpp
	core/App.h
	core/App.cpp
	)

if(OS_WINDOWS)
	set(SRCS ${SRCS}
		MainWindows.cpp
		platform/PlatformUtilsWindows.cpp
		content/io/WindowsArchive.h
		)
elseif(OS_LINUX)
	set(SRCS ${SRCS}
		MainLinux.cpp
		platform/PlatformUtilsLinux.cpp
		content/io/LinuxArchive.h
		)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

add_executable(${CEF_TARGET} WIN32 MACOSX_BUNDLE ${SRCS})

target_include_directories(${CEF_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link RapidJSON
target_link_libraries(${CEF_TARGET} RapidJSON)

if(OS_LINUX)
	# Link X11
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

if(OS_WINDOWS)
	# Add manifest
	ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET}" "exe")
endif()

if(OS_LINUX)
	# Set rpath so that libraries can be placed next to the executable.
	set_target_properties(${CEF_TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
	set_target_properties(${CEF_TARGET} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)

	# Set output path
	set_target_properties(${CEF_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
endif()

# Copy content
add_custom_command(
	TARGET ${CEF_TARGET} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/content $<TARGET_FILE_DIR:${CEF_TARGET}>/content
	)
add_custom_command(
	TARGET ${CEF_TARGET} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/config.default.json $<TARGET_FILE_DIR:${CEF_TARGET}>
	)
