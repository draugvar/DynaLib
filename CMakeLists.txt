cmake_minimum_required(VERSION 3.16)
project(DynaLib)

set(CMAKE_CXX_STANDARD 17)

include_directories(examples)
include_directories(include)

add_library(util SHARED examples/util.c)
set_target_properties(util PROPERTIES LINKER_LANGUAGE CXX)

add_executable(DynaLib
        examples/main.cpp
        include/dynalib.h)

set_target_properties(DynaLib PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(DynaLib PUBLIC cxx_std_20)
target_link_libraries(DynaLib PUBLIC ${CMAKE_DL_LIBS})
