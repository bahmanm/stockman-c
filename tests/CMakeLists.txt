enable_testing()

# Model tests
add_executable(
  Model_tests "Model_tests.c")
target_link_libraries(
  Model_tests
  PRIVATE
  ${GLIB_LIBRARIES}
  ${GIO_LIBRARIES}
  ${PROJECT_NAME}_LIBS)
target_include_directories(
  Model_tests
  PRIVATE "${CMAKE_SOURCE_DIR}/src/")
add_test(
  NAME Model_tests
  COMMAND Model_tests
  CONFIGURATIONS Debug;Release)

# Database_tests
add_executable(
  Database_tests "Database_tests.c")
target_include_directories(
  Database_tests
  PRIVATE "${CMAKE_SOURCE_DIR}/src/")
target_link_libraries(
  Database_tests
  PRIVATE
  ${GLIB_LIBRARIES}
  ${GIO_LIBRARIES}
  ${PROJECT_NAME}_LIBS)
add_test(
  NAME Database_tests
  COMMAND Database_tests
  CONFIGURATIONS Debug;Release)

# CsvImport tests
add_executable(
  CsvImport_tests "CsvImport_tests.c")
target_link_libraries(
  CsvImport_tests
  PRIVATE
  ${GLIB_LIBRARIES}
  ${GIO_LIBRARIES}
  ${PROJECT_NAME}_LIBS)
target_include_directories(
  CsvImport_tests
  PRIVATE "${CMAKE_SOURCE_DIR}/src/")
add_test(
  NAME CsvImport_tests
  COMMAND CsvImport_tests
  CONFIGURATIONS Debug;Release)
