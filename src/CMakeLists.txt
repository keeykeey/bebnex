# excutable file
add_library(bebnex_lib
    core/bnx_conf_file.c
    core/bnx_pool.c
)
add_executable(bebnex main.c)


# build env variable
# log-level
if(NOT DEFINED BNX_LOG_LEVEL)
    set(BNX_LOG_LEVEL 3)
endif()
set(BEBNEX_DEFINES
    BNX_LOG_LEVEL=${BNX_LOG_LEVEL}
    BNX_PREFIX=\"${BNX_PREFIX}\"
)
if(DEFINED BNX_DEBUG)
    set(BEBNEX_DEFINES
    ${BEBNEX_DEFINES}
    BNX_DEBUG=1
)
endif()

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
