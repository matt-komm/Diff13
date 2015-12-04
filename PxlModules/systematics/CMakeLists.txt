find_package(ROOT REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${ROOT_INCLUDE_DIR}
)

add_library(BTagReweighting MODULE BTagReweighting.cpp BTagCalibrationStandalone.cc)
target_link_libraries(BTagReweighting ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
install(
    TARGETS BTagReweighting
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

add_library(JetMETVariations MODULE JetMETVariations.cpp)
target_link_libraries(JetMETVariations ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
install(
    TARGETS JetMETVariations
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)
