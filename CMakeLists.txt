cmake_minimum_required(VERSION 2.8)

project(camera_calibration)

find_package(OpenCV 3 REQUIRED)
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
