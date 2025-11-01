add_executable(unit_test
    test.cpp
    test_bnx_log.cpp
    test_bnx_conf_file.cpp
    test_bnx_pool.cpp
)

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
