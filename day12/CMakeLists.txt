get_filename_component(CURRENT_TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" CURRENT_TARGET "${CURRENT_TARGET}")

message("Build ${CURRENT_TARGET} start")
add_executable(${CURRENT_TARGET} ${CURRENT_TARGET}.c)
target_link_libraries(${CURRENT_TARGET} common)


add_custom_target(${CURRENT_TARGET}-copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(${CURRENT_TARGET} ${CURRENT_TARGET}-copy_assets)
