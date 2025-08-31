add_executable(unit_test
    test.cpp
    test_bnx_log.cpp
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

# enable_testing()
add_test(NAME unit_test COMMAND $<TARGET_FILE:unit_test>)
