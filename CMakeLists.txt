cmake_minimum_required(VERSION 3.0.0)
project(cpp_oop VERSION 0.1.0 LANGUAGES C CXX)


include(CTest)
enable_testing()

add_executable(cpp_oop main.cpp)
include_directories()

if(MSVC)
  target_compile_options(cpp_oop PRIVATE /W4 /WX)
else()
  target_compile_options(cpp_oop PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
