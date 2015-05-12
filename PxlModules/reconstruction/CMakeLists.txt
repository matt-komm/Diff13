add_library(NeutrinoPz MODULE NeutrinoPz.cpp)
target_link_libraries(NeutrinoPz ${PXL_LIBRARIES})
install(
    TARGETS NeutrinoPz
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

add_library(TopReconstruction MODULE TopReconstruction.cpp)
target_link_libraries(TopReconstruction ${PXL_LIBRARIES})
install(
    TARGETS TopReconstruction
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

find_package(ROOT REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIR})

add_library(EventVariables MODULE EventVariables.cpp EventShapeVariables.cpp)
target_link_libraries(EventVariables ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
install(
    TARGETS EventVariables
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)
