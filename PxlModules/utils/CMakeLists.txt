find_package(ROOT REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${PROJECT_SOURCE_DIR}/internal/TMVA/inc/TMVA
    ${ROOT_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/internal/rapidjson
)

add_library(RootTreeWriter MODULE RootTree.cpp RootTreeWriter.cpp OutputStore.cpp)
target_link_libraries(RootTreeWriter ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
install(
    TARGETS RootTreeWriter
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

add_library(EventWeight MODULE EventWeight.cpp)
target_link_libraries(EventWeight ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
install(
    TARGETS EventWeight
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

### still experimental
add_library(IsrTagging MODULE IsrTagging.cpp)
target_link_libraries(IsrTagging ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
#install(
#    TARGETS IsrTagging
#    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
#)

add_library(LumiMask MODULE LumiMask.cpp)
target_link_libraries(LumiMask ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
install(
    TARGETS LumiMask
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

add_library(Splitter MODULE Splitter.cpp)
target_link_libraries(Splitter ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
install(
    TARGETS Splitter
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

add_library(TMVAEvaluation MODULE TMVAEvaluation.cpp)
target_link_libraries(TMVAEvaluation ${PXL_LIBRARIES} ${ROOT_LIBRARIES} TMVA)
install(
    TARGETS TMVAEvaluation
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

add_library(PileUpReweighting MODULE PileUpReweighting.cpp)
target_link_libraries(PileUpReweighting ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
install(
    TARGETS PileUpReweighting
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

