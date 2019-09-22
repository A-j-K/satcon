
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SRCS
    mainwindow.cpp
)

set(INCS
     mainwindow.h
)

add_library(mainwindow STATIC ${SRCS} ${INCS})

target_link_libraries(mainwindow PRIVATE Qt5::Widgets sgp4)
