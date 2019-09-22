cmake_minimum_required(VERSION 3.5)

project(satcon LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(libsgp4)
add_subdirectory(mainwindow)
add_subdirectory(capturewindow)

add_executable(satcon
	main.cpp
)

target_link_libraries(satcon PRIVATE 
	Qt5::Widgets 
	mainwindow 
	capturewindow
)
