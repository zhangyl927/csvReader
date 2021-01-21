cmake_minimum_required(VERSION 3.0.0)
project(fast_cpp_csv VERSION 0.1.0)

include(CTest)
enable_testing()

aux_source_directory(. DIR_SRCS)

add_subdirectory (src)

add_executable(fast_cpp_csv ${DIR_SRCS})

target_link_libraries (fast_cpp_csv DateFrame)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
