enable_testing()

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
