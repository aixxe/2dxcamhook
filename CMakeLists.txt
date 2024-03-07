cmake_minimum_required(VERSION 3.26)
project(2dxcamhook VERSION 0.3.0.0 LANGUAGES CXX)

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

file(GLOB_RECURSE VERSIONS "versions/*.cmake")
file(READ cmake/VersionOffsets.in VERSION_OFFSET_TEMPLATE)

foreach (VERSION_PATH ${VERSIONS})
    message(STATUS "Including game support file: ${VERSION_PATH}...")

    include(${VERSION_PATH})

    string(CONFIGURE ${VERSION_OFFSET_TEMPLATE} VERSION_OFFSET_TEXT @ONLY)
    string(APPEND VERSION_ALL_OFFSETS "${VERSION_OFFSET_TEXT}")
endforeach ()

configure_file(cmake/VersionHeader.in macros/versions.h @ONLY)

add_library(${PROJECT_NAME} SHARED
    src/main.cc
    src/receiver.cc
)

target_link_directories(${PROJECT_NAME} PRIVATE lib)
target_link_libraries(${PROJECT_NAME} safetyhook SpoutDX_static avs2-core)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/macros
    ${CPM_PACKAGE_spout2_SOURCE_DIR}/SPOUTSDK/SpoutDirectX
)