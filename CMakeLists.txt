cmake_minimum_required(VERSION 4.0)
project(DBSYS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.10.0/mingw_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets )


qt_standard_project_setup()

add_executable(DBSYS
        main.cpp
        MainWindow.cpp
        FileViewer.cpp
        CreateWindow.cpp
        DeleteWindow.cpp
        OpenWindow.cpp
        AddWindow.cpp
        multifile_functions.h
        LineDeletion.cpp
        editWindow.cpp
        zipWindow.cpp
        zipWindow.h
)

set_target_properties(DBSYS PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)

target_link_libraries(DBSYS PRIVATE
        Qt6::Core
        Qt6::Widgets
)

add_custom_command(TARGET DBSYS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/commands.txt"
        "${CMAKE_CURRENT_BINARY_DIR}/commands.txt"
)