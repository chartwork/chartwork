cmake_minimum_required(VERSION 2.8)
project(chartwork CXX)

find_package(Qt5Core 5.2 REQUIRED)
find_package(Qt5Widgets 5.2 REQUIRED)
find_package(Qt5Designer 5.2 REQUIRED)

if(MSVC)
	add_definitions(-D_USE_MATH_DEFINES)
else()
	add_definitions(-std=c++11)
endif()

add_definitions(${QT_DEFINITIONS})

add_subdirectory(src)
