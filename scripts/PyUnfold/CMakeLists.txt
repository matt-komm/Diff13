project(PyUnfold)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_MODULE_PATH}")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

#Tunfold needs to be included BEFORE root to shadow the old classes
include_directories($ENV{TUNFOLDDIR})
message(STATUS "including TUnfold from: $ENV{TUNFOLDDIR}")

find_package(ROOT)

set(CMAKE_CXX_FLAGS "-Wextra -Wall -pedantic ${ROOT_CFLAGS} -Wno-long-long")
include_directories(${ROOT_INCLUDE_DIR})

set(CLASSHEADERS 
    PyUnfold.hpp
)

set_source_files_properties(${PROJECT_BINARY_DIR}/dict.C PROPERTIES GENERATED TRUE)
add_custom_target(generatedicts ALL ${ROOT_DICT_EXECUTABLE} 
    -f ${PROJECT_BINARY_DIR}/dict.C 
    -c ${CLASSHEADERS} LinkDef.h 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


add_library(pyUnfold MODULE 
    PyUnfold.cpp 
    dict.C
)
target_link_libraries(pyUnfold ${ROOT_LIBRARIES} -unfold)
add_dependencies(pyUnfold generatedicts)

add_executable(printversion
    printversion.cpp
)
target_link_libraries(printversion ${ROOT_LIBRARIES} -unfold)
