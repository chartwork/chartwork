cmake_minimum_required(VERSION 3.0)
project(installer CXX)

find_package(Qt5Core 5.3 REQUIRED)
find_package(Qt5Widgets 5.3 REQUIRED)
find_package(Qt5Gui 5.3 REQUIRED)
find_package(Qt5Concurrent 5.3 REQUIRED)

add_definitions(-std=c++11 -lpthread)

add_definitions(${QT_DEFINITIONS})
set(QT_USE_QTOPENGL FALSE)

set(target installer)

file(GLOB core_sources "*.cpp")
file(GLOB core_headers "*.h")
file(GLOB core_resources "*.rc")

message(${core_sources})

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
	MACOSX_BUNDLE
	icon.icns
	${core_sources}
	${core_headers}
	${core_resources}
	${form_headers}
	${resourcesRCC}
)

set(libraries
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::Concurrent
)

set(MACOSX_BUNDLE_ICON_FILE icon.icns)
set_source_files_properties(
	icon.icns
	PROPERTIES
	MACOSX_PACKAGE_LOCATION Resources
)

add_executable(${target} ${sources})
target_link_libraries(${target} ${libraries})