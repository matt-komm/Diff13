CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (pxlmodules)
ADD_DEFINITIONS(-std=c++0x)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
FIND_PACKAGE(PXL REQUIRED)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${PXL_INCLUDE_DIR})


add_subdirectory(selection)
add_subdirectory(utils)
add_subdirectory(reconstruction)
add_subdirectory(internal)

