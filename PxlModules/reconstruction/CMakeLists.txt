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

