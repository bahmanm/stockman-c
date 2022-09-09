cmake_minimum_required (VERSION 3.24)
set (
  CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/.cmake
  ${CMAKE_MODULE_PATH})

project (stockman-c C)

set (CMAKE_C_STANDARD 17)

if (CMAKE_C_COMPILER_ID EQUAL GNU)
  set (
    CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -Wall -Wextra")
endif ()

add_executable (${PROJECT_NAME} src/main.c)

add_custom_target (
  format ALL
  COMMAND ${CMAKE_COMMAND} -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR} -P${PROJECT_SOURCE_DIR}/.cmake/reformat.cmake)

add_dependencies (${PROJECT_NAME} format)

find_package (GLib)
target_link_libraries (
  ${PROJECT_NAME}
  ${GLIB_LIBRARIES}
  ${GIO_LIBRARIES})
