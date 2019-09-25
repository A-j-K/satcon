cmake_minimum_required(VERSION 3.5)

project(satcon LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS 
	Core 
	Gui 
	Widgets 
	Network
	Multimedia 
	MultimediaWidgets
REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5multimedia_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(libsgp4)
add_subdirectory(mainwindow)
add_subdirectory(capturewindow)

add_executable(satcon WIN32
	main.cpp
)

target_link_libraries(satcon PRIVATE 
	Qt5::Core
	Qt5::Widgets 
	Qt5::Network 
	Qt5::Multimedia
	Qt5::MultimediaWidgets
	mainwindow 
	capturewindow
)
