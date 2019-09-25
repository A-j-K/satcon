
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

set(SRCS
    capturewindow.cpp
)

set(INCS
     capturewindow.h
)

add_library(capturewindow STATIC ${SRCS} ${INCS})

target_link_libraries(capturewindow PRIVATE 
	sgp4
	Qt5::Core
	Qt5::Widgets 
	Qt5::Network 
	Qt5::Multimedia
	Qt5::MultimediaWidgets
)
