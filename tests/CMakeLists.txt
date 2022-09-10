enable_testing()

# models tests
add_executable(
  models_tests "models_tests.c")
target_link_libraries(
  models_tests
  PRIVATE
  ${GLIB_LIBRARIES}
  ${GIO_LIBRARIES}
  ${PROJECT_NAME}_LIBS)
target_include_directories(
  models_tests
  PRIVATE "${CMAKE_SOURCE_DIR}/src/")
add_test(
  NAME models_tests
  COMMAND models_tests
  CONFIGURATIONS Debug;Release)

# database_tests
add_executable(
  database_tests "database_tests.c")
target_include_directories(
  database_tests
  PRIVATE "${CMAKE_SOURCE_DIR}/src/")
target_link_libraries(
  database_tests
  PRIVATE
  ${GLIB_LIBRARIES}
  ${GIO_LIBRARIES}
  ${PROJECT_NAME}_LIBS)
add_test(
  NAME database_tests
  COMMAND database_tests
  CONFIGURATIONS Debug;Release)
