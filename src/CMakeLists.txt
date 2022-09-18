set(
  src_HEADERS
  Model/Model.h
  Model/Invoice.h
  Model/InvoiceLine.h
  Database.h
  CsvImport.h)

set(
  src_SOURCES
  Model/Invoice.c
  Model/InvoiceLine.c
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
