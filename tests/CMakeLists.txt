include(CTest)

add_executable(
  invoice_tests "invoice_tests.c")

target_link_libraries(
  invoice_tests
  ${GLIB_LIBRARIES}
  ${GIO_LIBRARIES})

add_test(
  NAME invoice_tests
  COMMAND invoice_tests
  CONFIGURATIONS Debug;Release)
