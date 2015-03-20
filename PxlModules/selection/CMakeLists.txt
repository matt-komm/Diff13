
add_library(MuonSelection SHARED MODULE MuonSelection.cpp)

target_link_libraries(MuonSelection ${PXL_LIBRARIES})

install(
    TARGETS MuonSelection
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

