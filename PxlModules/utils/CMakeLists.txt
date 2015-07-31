find_package(ROOT REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${PROJECT_SOURCE_DIR}/internal/TMVA/inc/TMVA
    ${ROOT_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/internal/rapidjson
)

add_library(RootTreeWriter MODULE RootTreeWriter.cpp OutputStore.cpp)
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

add_library(LumiMask MODULE LumiMask.cpp)
target_link_libraries(LumiMask ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
install(
    TARGETS LumiMask
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

add_executable(TMVATraining TMVATraining.cpp)
target_link_libraries(TMVATraining TMVA ${ROOT_LIBRARIES} -lRIO -lHist -lTree -lMLP -lMinuit -lXMLIO)
