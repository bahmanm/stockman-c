set(
  src_HEADERS
  model/model.h
  model/invoice.h
  model/invoiceline.h
  Database.h
  CsvImport.h)

set(
  src_SOURCES
  model/invoice.c
  model/invoiceline.c
  Database.c
  CsvImport.c)

add_executable(${PROJECT_NAME} ${src_HEADERS} ${src_SOURCES} main.c)
add_library(
  ${PROJECT_NAME}_LIBS
  OBJECT ${src_SOURCES})

target_link_libraries(
  ${PROJECT_NAME}
  ${GLIB_LIBRARIES}
  ${GIO_LIBRARIES})
