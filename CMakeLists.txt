cmake_minimum_required(VERSION 3.24)
project(stockman-c C)
set(CMAKE_C_STANDARD 17)

set(
  CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/.cmake
  ${CMAKE_MODULE_PATH})

add_executable(${PROJECT_NAME} src/main.c)

add_custom_target(
  format ALL
  COMMAND ${CMAKE_COMMAND} -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR} -P${PROJECT_SOURCE_DIR}/.cmake/reformat.cmake)

add_dependencies(${PROJECT_NAME} format)

find_package(GLib)
target_link_libraries(${PROJECT_NAME} ${GLIB_LIBRARIES})
