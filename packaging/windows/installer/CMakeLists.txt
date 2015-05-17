cmake_minimum_required(VERSION 3.0)
project(installer CXX)

find_package(Qt5Core 5.3 REQUIRED)
find_package(Qt5Widgets 5.3 REQUIRED)
find_package(Qt5Gui 5.3 REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-std=c++11)
endif()

add_definitions(${QT_DEFINITIONS})
set(QT_USE_QTOPENGL FALSE)

set(target installer)

file(GLOB core_sources "*.cpp")
file(GLOB core_headers "*.h")
file(GLOB core_resources "*.rc")

file(GLOB forms "*.ui")
qt5_wrap_ui(form_headers "mainwindow.ui")

file(GLOB resources "*.qrc")
qt5_add_resources(resourcesRCC ${resources})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(
	${QT_INCLUDE_DIRS}
)

set(sources
	${core_sources}
	${core_headers}
	${core_resources}
	${form_headers}
	${resourcesRCC}
)

if(CMAKE_BUILD_TYPE MATCHES Release)
	set(Qt5PlatformSupport_LIBRARY ${_qt5Core_install_prefix}/lib/Qt5PlatformSupport.lib)
else()
	set(Qt5PlatformSupport_LIBRARY ${_qt5Core_install_prefix}/lib/Qt5PlatformSupportd.lib)
endif()

message(STATUS ${Qt5PlatformSupport_LIBRARY})

set(libraries
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::QWindowsIntegrationPlugin
	${Qt5PlatformSupport_LIBRARY}
	imm32.lib
	winmm.lib
	Ws2_32.lib
)

add_executable(${target} ${sources})
target_link_libraries(${target} ${libraries})

set_target_properties(${target} PROPERTIES
	LINK_FLAGS "/MANIFEST:NO /SUBSYSTEM:WINDOWS"
	AUTOMOC TRUE
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)
