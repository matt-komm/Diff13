# Rules for making the so library with the vector and libm-like signatures

# Define this directory's flags:
SET(CMAKE_CXX_FLAGS  ${LIB_FLAGS})

include_directories ( ${VDT_INC_DIR} )

if(BUILD_SHARED_LIBS)
  message(STATUS "Libraries are configured as: SHARED")
else()
  message(STATUS "Libraries are configured as: STATIC")
endif()

if(PRELOAD)
  message(STATUS "Symbols for the preload requested")
endif()

# The library
ADD_LIBRARY(vdt ${VDT_SRC_DIR}/vdtMath_signatures.cc ${VDT_INC_DIR}/vdtMath.h )

        

