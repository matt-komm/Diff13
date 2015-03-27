add_library(MuonSelection MODULE MuonSelection.cpp)
target_link_libraries(MuonSelection ${PXL_LIBRARIES})
install(
    TARGETS MuonSelection
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)


add_library(ElectronSelection MODULE ElectronSelection.cpp)
target_link_libraries(ElectronSelection ${PXL_LIBRARIES})
install(
    TARGETS ElectronSelection
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)


add_library(JetSelection MODULE JetSelection.cpp)
target_link_libraries(JetSelection ${PXL_LIBRARIES})
install(
    TARGETS JetSelection
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)


