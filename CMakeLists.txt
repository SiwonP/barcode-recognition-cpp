cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
file(
    GLOB_RECURSE
    src/*
    source_files
)
add_executable(
    DisplayImage
    ${source_files}
    main.cc
)

target_link_libraries( DisplayImage ${OpenCV_LIBS} )
