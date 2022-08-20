cmake_minimum_required(VERSION 3.1.0)

project(demo LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Core Gui Qml Quick REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(ffmpeg REQUIRED IMPORTED_TARGET 
  libavdevice
  libavformat
  libavfilter
  libavcodec
  libswresample
  libswscale
  libavutil
)

# include_directories(videoplayer, ffmpeg)
# include_directories(${PROJECT_SOURCE_DIR}/ffmpeg)




add_executable(demo
  main.cpp
  mainwindow.ui
  mainwindow.cpp
  videoplayer.cpp)

target_link_libraries(demo
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
  Qt5::Qml
  Qt5::Quick
  PkgConfig::ffmpeg
)


