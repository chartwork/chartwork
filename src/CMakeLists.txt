set(target chartwork)

file(GLOB core_sources "chartwork/*.cpp")
file(GLOB core_headers "${PROJECT_SOURCE_DIR}/include/chartwork/*.h")

file(GLOB elements_sources "chartwork/elements/*.cpp")
file(GLOB elements_headers "${PROJECT_SOURCE_DIR}/include/chartwork/elements/*.h")

file(GLOB exception_sources "chartwork/exceptions/*.cpp")
file(GLOB exception_headers "${PROJECT_SOURCE_DIR}/include/chartwork/exceptions/*.h")

file(GLOB plugins_sources "chartwork/plugins/*.cpp")
file(GLOB plugins_headers "${PROJECT_SOURCE_DIR}/include/chartwork/plugins/*.h")

file(GLOB resources "chartwork/*.qrc")
qt5_add_resources(resourcesRCC ${resources})

include_directories(
	${QT_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
)

set(sources
	${core_sources}
	${core_headers}

	${elements_sources}
	${elements_headers}

	${exception_sources}
	${exception_headers}

	${plugins_sources}
	${plugins_headers}

	${resourcesRCC}
)

set(libraries
	${QT_LIBRARIES}
)

add_library(${target} SHARED ${sources})
target_link_libraries(${target} ${libraries})

set_target_properties(${target} PROPERTIES
	AUTOMOC TRUE
	DEFINE_SYMBOL "QDESIGNER_EXPORT_WIDGETS"
	DEBUG_POSTFIX "d"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)

qt5_use_modules(${target} Widgets Designer)
