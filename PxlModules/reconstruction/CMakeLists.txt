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
include_directories(${ROOT_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/internal/vdt/include)
add_library(Chi2TopSolver MODULE Chi2TopSolver.cpp)
target_link_libraries(Chi2TopSolver ${PXL_LIBRARIES} ${ROOT_LIBRARIES} ${vdt})
install(
    TARGETS Chi2TopSolver
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)

