
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SRCS
    capturewindow.cpp
)

set(INCS
     capturewindow.h
)

add_library(capturewindow STATIC ${SRCS} ${INCS})

target_link_libraries(capturewindow PRIVATE Qt5::Widgets sgp4)
