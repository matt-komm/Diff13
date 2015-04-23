# Auto generate the code for the vector signatures and if needed preload

# Generate the code for the lib
if (PRELOAD)
set (SIGGENOPTS  " -p")
else()
set (SIGGENOPTS  " ")
endif()
EXEC_PROGRAM ("cd ${CMAKE_CURRENT_SOURCE_DIR};python signatures_generator.py ${SIGGENOPTS};cd -")

#generare Vc wrapper and config file
if(USE_VC)
  EXEC_PROGRAM ("cd ${CMAKE_CURRENT_SOURCE_DIR};python vc_wrapper_generator.py;cd -")
endif(USE_VC)
message(${VDT_INC_DIR})
configure_file( ${VDT_INC_DIR}/externalLibcfg.h.cmake ${VDT_INC_DIR}/externalLibcfg.h)
