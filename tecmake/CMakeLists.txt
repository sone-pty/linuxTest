cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Tutorial VERSION 1.0)

# add the executable
add_executable(Tutorial tutorial.cpp)
configure_file(TutorialConfig.h.in TutorialConfig.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_MYMATH "Use tutorial provided math implementation" ON)
if(USE_MYMATH)
  add_subdirectory(Math)
  list(APPEND EXTRA_LIBS "MathLib")
  list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/Math")
endif()

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
						   "${EXTRA_INCLUDES}"
                           )
