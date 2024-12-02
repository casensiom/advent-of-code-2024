get_filename_component(CURRENT_TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" CURRENT_TARGET "${CURRENT_TARGET}")

message("Build ${CURRENT_TARGET} start")
add_executable(${CURRENT_TARGET} ${CURRENT_TARGET}.cpp)
# target_link_libraries(${CURRENT_TARGET} common)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/casensiom/toolit.git
    GIT_TAG 71e1791c2edeebae5a1a758e2cddcb7999975b6e
)

# Traer Google Test
FetchContent_MakeAvailable(toolit)

add_custom_target(${CURRENT_TARGET}-copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(${CURRENT_TARGET} ${CURRENT_TARGET}-copy_assets)