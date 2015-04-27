add_library(TriggerSelection MODULE TriggerSelection.cpp)
target_link_libraries(TriggerSelection ${PXL_LIBRARIES})
install(
    TARGETS TriggerSelection
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)


add_library(MuonSelection MODULE MuonSelection.cpp)
target_link_libraries(MuonSelection ${PXL_LIBRARIES})
install(
    TARGETS MuonSelection
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

add_library(MuonVeto MODULE MuonVeto.cpp)
target_link_libraries(MuonVeto ${PXL_LIBRARIES})
install(
    TARGETS MuonVeto
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)



add_library(ElectronSelection MODULE ElectronSelection.cpp)
target_link_libraries(ElectronSelection ${PXL_LIBRARIES})
install(
    TARGETS ElectronSelection
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

add_library(ElectronVeto MODULE ElectronVeto.cpp)
target_link_libraries(ElectronVeto ${PXL_LIBRARIES})
install(
    TARGETS ElectronVeto
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)


add_library(JetSelection MODULE JetSelection.cpp)
target_link_libraries(JetSelection ${PXL_LIBRARIES})
install(
    TARGETS JetSelection
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

add_library( BTagSelection MODULE  BTagSelection.cpp)
target_link_libraries(BTagSelection ${PXL_LIBRARIES})
install(
    TARGETS BTagSelection
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

