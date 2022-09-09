cmake_minimum_required(VERSION 3.5)
project(SLISP CXX)

# EDIT
# add any files you create related to the interpreter here
# excluding unit tests
set(interpreter_src
  tokenize.hpp tokenize.cpp
  expression.hpp expression.cpp
  environment.hpp environment.cpp
  interpreter.hpp interpreter.cpp
  )

# EDIT
# add any files you create related to unit testing here
set(test_src
  catch.hpp
  unittests.cpp
  test_tokenize.cpp
  test_types.cpp
  test_interpreter.cpp
)

# EDIT
# add any files you create related to the slisp program here
set(slisp_src
  ${interpreter_src}
  slisp.cpp
  )

# ------------------------------------------------
# You should not need to edit any files below here
# ------------------------------------------------

# create the slisp executable
add_executable(slisp ${slisp_src})
set_property(TARGET slisp PROPERTY CXX_STANDARD 11)

# setup testing
set(TEST_FILE_DIR "${CMAKE_SOURCE_DIR}/tests")

configure_file(${CMAKE_SOURCE_DIR}/test_config.hpp.in 
  ${CMAKE_BINARY_DIR}/test_config.hpp)

include_directories(${CMAKE_BINARY_DIR})

add_executable(unittests ${interpreter_src} ${test_src})
set_property(TARGET unittests PROPERTY CXX_STANDARD 11)

enable_testing()
add_test(unittests unittests)

################
SET(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

# On Linux, using GCC, to enable coverage on tests -DCOVERAGE=TRUE                                                                                                            
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX AND COVERAGE)
  message("Enabling Test Coverage")
  set_target_properties(unittests PROPERTIES COMPILE_FLAGS ${GCC_COVERAGE_COMPILE_FLAGS} )
  target_link_libraries(unittests gcov)
  add_custom_target(coverage-grading
    COMMAND ${CMAKE_COMMAND} -E env "ROOT=${CMAKE_CURRENT_SOURCE_DIR}"
    ${CMAKE_CURRENT_SOURCE_DIR}/coverage.sh) 
endif()
