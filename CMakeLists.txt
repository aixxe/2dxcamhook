cmake_minimum_required(VERSION 3.26)
project(2dxcamhook VERSION 0.2.0.0 LANGUAGES CXX)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME               safetyhook
    GIT_TAG            v0.3.0
    GITHUB_REPOSITORY  cursey/safetyhook
    OPTIONS            "SAFETYHOOK_FETCH_ZYDIS ON"
)

CPMAddPackage(
    NAME               spout2
    GIT_TAG            2.007.013
    GITHUB_REPOSITORY  leadedge/Spout2
    OPTIONS            "SPOUT_BUILD_CMT OFF" "SPOUT_BUILD_LIBRARY OFF" "SPOUT_BUILD_SPOUTDX ON"
)

file(GLOB_RECURSE VERSIONS "src/game/*.cc")

foreach (VERSION_PATH ${VERSIONS})
    get_filename_component(VERSION_BASENAME ${VERSION_PATH} NAME_WE)
    set(TARGET_NAME ${PROJECT_NAME}.${VERSION_BASENAME})

    message(STATUS "Adding target for ${VERSION_BASENAME}...")

    add_library(${TARGET_NAME} SHARED ${VERSION_PATH} src/main.cc src/receiver.cc)

    target_link_directories(${TARGET_NAME} PRIVATE lib)
    target_link_libraries(${TARGET_NAME} safetyhook SpoutDX_static avs2-core)
    target_include_directories(${TARGET_NAME} PUBLIC ${CPM_PACKAGE_spout2_SOURCE_DIR}/SPOUTSDK/SpoutDirectX)
endforeach ()