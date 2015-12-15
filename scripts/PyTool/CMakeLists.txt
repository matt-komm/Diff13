project(PyUnfold)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_MODULE_PATH}")


macro(copy_file_if_changed in_file out_file target)  
    	add_custom_command (
    		TARGET     ${target}
    		POST_BUILD
    		COMMAND    ${CMAKE_COMMAND}
    		ARGS       -E copy_if_different ${in_file} ${out_file}
    		COMMENT "Copying file: ${in_file} to: ${out_file}"
    	)
endmacro(copy_file_if_changed)


include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

#Tunfold needs to be included BEFORE root to shadow the old classes
if ($ENV{TUNFOLDDIR} STREQUAL "")
    message(FATAL_ERROR "TUnfold not found. You need to specify environment variable $TUNFOLDDIR")
else ($ENV{TUNFOLDDIR} STREQUAL "")
    message(STATUS "including TUnfold from: $ENV{TUNFOLDDIR}")
endif ($ENV{TUNFOLDDIR} STREQUAL "")
include_directories($ENV{TUNFOLDDIR})


find_package(ROOT)

set(CMAKE_CXX_FLAGS "-Wextra -Wall -pedantic ${ROOT_CFLAGS} -Wno-long-long -std=c++11 -Wno-unused-parameter")
include_directories(${ROOT_INCLUDE_DIR})

set(CLASSHEADERS 
    PyUnfold.hpp
    PyUtils.hpp
)

set_source_files_properties(${PROJECT_BINARY_DIR}/dict.C PROPERTIES GENERATED TRUE)
add_custom_target(generatedicts ALL ${ROOT_DICT_EXECUTABLE} 
    -f ${PROJECT_BINARY_DIR}/dict.C 
    -c ${CLASSHEADERS} LinkDef.h 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


add_library(pyUnfold MODULE 
    PyUnfold.cpp 
    PyUtils.cpp 
    dict.C
)
target_link_libraries(pyUnfold $ENV{TUNFOLDDIR}/libunfold.so ${ROOT_LIBRARIES})
add_dependencies(pyUnfold generatedicts)

add_executable(printversion
    printversion.cpp
)
target_link_libraries(printversion $ENV{TUNFOLDDIR}/libunfold.so ${ROOT_LIBRARIES} )

copy_file_if_changed(${CMAKE_CURRENT_SOURCE_DIR}/test.py ${CMAKE_CURRENT_BINARY_DIR}/test.py pyUnfold)
