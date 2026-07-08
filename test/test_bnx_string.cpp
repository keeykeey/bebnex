#include <CppUTest/CommandLineTestRunner.h>
#include <cstdlib>
#include <vector>
#include "CppUTest/TestHarness_c.h"
extern "C" {
    #include "core/bnx_string.h"
}

TEST_GROUP(BnxStrLiteral)
{
    void setup() override{}
    void teardown() override{}
    
    struct Tests {
        const char *label;
        bnx_string_t got;
        const char *expect_str;
        int expect_size;
    };
};


TEST(BnxStrLiteral, PositiveBoundary)
{
    // bnx_str_literal() accepts only literal as a input.
    // (it doesn't works fine if char * variable is given as a input)
    std::vector<Tests> tests = {
        {"Test if empty string is given", bnx_str_literal(""), "", 0},
        {"Test if one string is given", bnx_str_literal("a"), "a", 1},
        {"Test if given string has null-string at its tail", bnx_str_literal("a\0"), "a\0", 2},
        {"Test if many string is given", bnx_str_literal("Hello world"), "Hello world", 11},
    };

    std::string msg;
    for (auto& tt : tests) {
        msg = tt.label + std::string("(Check equal string data)");
        CHECK_EQUAL_C_STRING_TEXT(tt.expect_str, tt.got.data, msg.c_str());
        msg = tt.label + std::string("(Check equal string length)");
        CHECK_EQUAL_TEXT(tt.expect_size, tt.got.len, msg.c_str());
    }
}


TEST_GROUP(BnxSetStr)
{
    void setup() override{}
    void teardown() override{}
    
    struct Tests {
        const char *label;
        bnx_string_t str;
        const char *expect_str;
        int expect_size;
    };
};

TEST(BnxSetStr, PositiveBoundary)
{
    bnx_string_t s1 = { 0 }; bnx_set_str(&s1, "");
    bnx_string_t s2 = { 0 }; bnx_set_str(&s2, "Z");
    bnx_string_t s3 = { 0 }; bnx_set_str(&s3, "Z\0");
    bnx_string_t s4 = { 0 }; bnx_set_str(&s4, "Hello world!");
    std::vector<Tests> tests = {
        {"Test ", s1, "", 0},
        {"Test ", s2, "Z", 1},
        {"Test ", s3, "Z\0", 2},
        {"Test ", s4, "Hello world!", 12},
    };
    
    std::string msg;
    for (auto& tt : tests) {
        msg = tt.label + std::string("(Check equal string data)");
        CHECK_EQUAL_C_STRING_TEXT(tt.expect_str, tt.str.data, msg.c_str());
        msg = tt.label + std::string("(Check equal string length)");
        CHECK_EQUAL_TEXT(tt.expect_size, tt.str.len, msg.c_str());
    }
}