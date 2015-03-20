# - Try to find PXL 
# Once done, this will define
#
#  PXL_FOUND - system has PXL 
#  PXL_INCLUDE_DIR - the PXL include directories
#  PXL_LIBRARIES - link these to use PXL 
#  PXL_PLUGIN_INSTALL_PATH - path to the default plugins

find_program(PXLRUN_PATH pxlrun)

SET(PXL_FOUND FALSE)

if (PXLRUN_PATH)
    message(STATUS "pxlrun command found under: ${PXLRUN_PATH}")
	SET(PXL_FOUND TRUE)
	execute_process (COMMAND ${PXLRUN_PATH} --getUserPluginPath
		OUTPUT_VARIABLE PXL_PLUGIN_INSTALL_PATH
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	MESSAGE(STATUS User\ plugin\ path\ modules\ to \ ${PXL_PLUGIN_INSTALL_PATH}  )

	execute_process (COMMAND ${PXLRUN_PATH} --incdir
		OUTPUT_VARIABLE PXL_INCLUDE_DIR
		OUTPUT_STRIP_TRAILING_WHITESPACE)

	execute_process (COMMAND ${PXLRUN_PATH} --libdir
		OUTPUT_VARIABLE PXL_LIBRARIES_DIR
		OUTPUT_STRIP_TRAILING_WHITESPACE)
		
	find_library(PXL_CORELIB NAMES pxl-core
             HINTS ${PXL_LIBRARIES_DIR} )
	find_library(PXL_MODULESLIB NAMES pxl-modules
         HINTS ${PXL_LIBRARIES_DIR} )
 	find_library(PXL_HEPLIB NAMES pxl-hep
         HINTS ${PXL_LIBRARIES_DIR} )
    set(PXL_LIBRARIES ${PXL_CORELIB} ${PXL_MODULESLIB} ${PXL_HEPLIB})
	
    mark_as_advanced(PXL_INCLUDE_DIR PXL_LIBRARIES)
endif(PXLRUN_PATH)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PXL DEFAULT_MSG PXL_LIBRARIES PXL_INCLUDE_DIR)
                                  


