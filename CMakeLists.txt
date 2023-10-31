cmake_minimum_required(VERSION 3.1.0)

project(sort VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Concurrent Widgets Charts REQUIRED)
find_package(OpenSSL REQUIRED)

find_library(SQLITE3_LIB libsqlite3.a sqlite3)


# Установка пути к библиотеке QCustomPlot
include_directories(/usr/include)

# Генерация заголовочных файлов ui
qt5_wrap_ui(UI_HEADERS mainwindow.ui)

add_executable(main
    main.cpp
    mainwindow.cpp
    ${UI_HEADERS}
)

target_include_directories(main PRIVATE ${OPENSSL_INCLUDE_DIR})

target_link_libraries(main PRIVATE
    Qt5::Widgets
    Qt5::Charts
    Qt5::Concurrent
    qcustomplot
    ${OPENSSL_LIBRARIES}
    OpenSSL::Crypto
    ${SQLITE3_LIB} 
)

