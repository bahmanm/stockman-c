##########
# enforce formatting via astyle.
# the style is configured in ${PROJECT_SOURCE_DIR}/.astyle
##########
file(
  GLOB_RECURSE MY_SOURCE_FILES
  LIST_DIRECTORIES false
  ${PROJECT_SOURCE_DIR}/src/*.[ch]
  ${PROJECT_SOURCE_DIR}/tests/*.[ch])

foreach(file ${MY_SOURCE_FILES})
  message(STATUS "Formatting ${file}...")
  execute_process(
    COMMAND astyle --options=${PROJECT_SOURCE_DIR}/.astylerc ${file})
endforeach()
