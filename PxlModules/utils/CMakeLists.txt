find_package(ROOT REQUIRED)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIR})

add_library(RootTreeWriter MODULE RootTreeWriter.cpp OutputStore.cpp)
target_link_libraries(RootTreeWriter ${PXL_LIBRARIES} ${ROOT_LIBRARIES})
install(
    TARGETS RootTreeWriter
    LIBRARY DESTINATION ${PXL_PLUGIN_INSTALL_PATH}
)


