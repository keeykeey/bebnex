# excutable file
add_library(bebnex_lib
    core/bnx_log.c
)
add_executable(bebnex main.c)


# build env variable
# log-level
if(NOT DEFINED BNX_LOG_LEVEL)
    set(BNX_LOG_LEVEL 3)
endif()
target_compile_definitions(bebnex PRIVATE 
    BNX_LOG_LEVEL=${BNX_LOG_LEVEL}
)
# debug-mode
if(DEFINED BNX_DEBUG)
    target_compile_definitions(bebnex PRIVATE 
        BNX_DEBUG=1
    )
endif()

target_include_directories(bebnex_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(bebnex PRIVATE bebnex_lib)
