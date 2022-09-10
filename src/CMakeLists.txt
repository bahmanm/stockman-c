set(
  src_HEADERS
  models.h
  db.h)

set(
  src_SOURCES
  models.c
  db.c)

add_executable(${PROJECT_NAME} ${src_HEADERS} ${src_SOURCES} main.c)
add_library(
  ${PROJECT_NAME}_LIBS
  OBJECT ${src_SOURCES})

target_link_libraries(
  ${PROJECT_NAME}
  ${GLIB_LIBRARIES}
  ${GIO_LIBRARIES})
