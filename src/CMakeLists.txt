# excutable file
add_library(bebnex_lib
    core/bnx_log.c
    core/bnx_conf_file.c
    core/bnx_pool.c
    core/bnx_connection.c
)
add_executable(bebnex main.c)


# build env variable
set(BEBNEX_DEFINES
    BNX_PREFIX=\"${BNX_PREFIX}\"
)

target_compile_definitions(bebnex PUBLIC
    ${BEBNEX_DEFINES}
)
target_compile_definitions(bebnex_lib PUBLIC
    ${BEBNEX_DEFINES}
)

target_include_directories(bebnex_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(bebnex PRIVATE bebnex_lib)
