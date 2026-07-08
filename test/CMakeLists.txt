add_executable(unit_test
    test.cpp
    test_bnx_log.cpp
    test_bnx_conf_file.cpp
    test_bnx_pool.cpp
    test_bnx_connection.cpp
    test_bnx_string.cpp
)

# Enable ASan to detect memory leak.
target_compile_options(unit_test PRIVATE -fsanitize=address)
target_link_options(unit_test PRIVATE -fsanitize=address)

target_include_directories(unit_test PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/cpputest/include
)

target_link_libraries(unit_test
    PRIVATE     # testだけでリンクする
    bebnex_lib  # src/のライブラリ
    CppUTest
    CppUTestExt
)

add_test(NAME unit_test COMMAND $<TARGET_FILE:unit_test>)
