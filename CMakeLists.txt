cmake_minimum_required(VERSION 3.8)
project(global_assistant)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(NOT ROOT)
  set(ROOT ${PROJECT_SOURCE_DIR}/)
endif()

add_definitions(-DROOT="${ROOT}")

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories("/usr/include/eigen3")

add_executable(global_assistant src/global_assistant_rechecker.cpp)
# add_executable(platform_assistant src/platform_assistant.cpp)

target_link_libraries(global_assistant ${OpenCV_LIBS})
# target_link_libraries(platform_assistant ${OpenCV_LIBS})