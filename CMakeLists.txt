cmake_minimum_required(VERSION 3.14)

project(SU-Opgave-Mikkel LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt5Sql REQUIRED)

add_executable(SU-Opgave-Mikkel
  main.cpp Character.h Character.cpp Hero.h Hero.cpp Enemy.h Enemy.cpp Manager.h Manager.cpp Magic.h Magic.cpp
)
target_link_libraries(SU-Opgave-Mikkel Qt${QT_VERSION_MAJOR}::Core Qt5::Sql)
