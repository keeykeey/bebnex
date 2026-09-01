#include "CppUTest/TestHarness_c.h"
#include "CppUTest/UtestMacros.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <cassert>
#include <cstdio>
#include <vector>
extern "C" {
    #include "core/bnx_pool.h"
    #include "core/bnx_log.h"
    #include "core/bnx_string.h"
}

void mockLogWriter(bnx_log_t *log, const char *buf, size_t size) { return; }

TEST_GROUP(BnxAlignPtr)
{
    void setup() override{}
    void teardown() override{}

    typedef struct tests_s {
        uintptr_t pointer;
        int align;
        uintptr_t expect;
    } tests_t;
};


TEST(BnxAlignPtr, PositiveBoundary)
{
    tests_t tests[] = {
        { .pointer = 0, .align = 1, .expect = 0 },
        { .pointer = 1, .align = 1, .expect = 1 },
        { .pointer = 0, .align = 16, .expect = 0 },
        { .pointer = 1, .align = 16, .expect = 16 },
        { .pointer = 15, .align = 16, .expect = 16 },
        { .pointer = 16, .align = 16, .expect = 16 },
        { .pointer = 17, .align = 16, .expect = 32 },
        { .pointer = 32, .align = 16, .expect = 32 },
    };

    for (auto& tt: tests) {
        unsigned char *result = bnx_align_ptr(tt.pointer, tt.align);
        POINTERS_EQUAL((unsigned char *)tt.expect, result);
    }

}


TEST_GROUP(BnxCreatePool)
{
    void setup() override{}
    void teardown() override{}
    bnx_string_t log_file_path = bnx_str_literal("foo");
    bnx_log_t log = {
        .log_level=BNX_LOG_LEVEL_INFO,
        .fd=3,
        .fpath=&log_file_path,
        .writer=mockLogWriter,
        .next=NULL,
    };
};


TEST(BnxCreatePool, create_success)
{
    size_t pool_sizes[2] = {sizeof(bnx_pool_t), sizeof(bnx_pool_t) + 1};

    for (size_t size : pool_sizes) {
        bnx_pool_t *pool = bnx_create_pool(size, &log);
        assert(pool);

        assert(pool->d.last);
        assert(pool->d.end);
        assert(pool->d.next == NULL);
        assert(pool->dsize >= 0);
        CHECK_EQUAL_C_INT(size - sizeof(bnx_pool_t), pool->dsize);
        assert(pool->large == NULL);
        CHECK_EQUAL_C_POINTER(&(pool->d), pool->current);

        bnx_pool_destroy(&pool);
    }
}


TEST(BnxCreatePool, create_fail)
{
    size_t pool_sizes[3] = {0, 1, sizeof(bnx_pool_t) - 1};

    for (size_t size : pool_sizes) {
        bnx_pool_t *pool = bnx_create_pool(size, &log);
        assert(pool == NULL);
    }
}


TEST_GROUP(BnxPoolDestroy)
{
    void setup() override{}
    void teardown() override{}
    bnx_string_t log_file_path = bnx_str_literal("foo");
    bnx_log_t log = {
        .log_level=BNX_LOG_LEVEL_INFO,
        .fd=3,
        .fpath=&log_file_path,
        .writer=mockLogWriter,
        .next=NULL,
    };
};


TEST(BnxPoolDestroy, Positive)
{
    bnx_pool_t *pool = bnx_create_pool(1024, &log);
    assert(pool);
    bnx_return_t result = bnx_pool_destroy(&pool);
    CHECK_EQUAL(BNX_OK, result.code);
    POINTERS_EQUAL(NULL, pool);
}


TEST(BnxPoolDestroy, Negative)
{
    bnx_return_t result = bnx_pool_destroy(NULL);
    CHECK_EQUAL(BNX_ERROR, result.code);
}


TEST_GROUP(BnxPoolReset)
{
    void setup() override{}
    void teardown() override{}
    bnx_string_t log_file_path = bnx_str_literal("foo");
    bnx_log_t log = {
        .log_level=BNX_LOG_LEVEL_INFO,
        .fd=3,
        .fpath=&log_file_path,
        .writer=mockLogWriter,
        .next=NULL,
    };
};


TEST(BnxPoolReset, reset_success)
{
    size_t size = 128;
    bnx_pool_t *p = bnx_create_pool(size, &log);
    bnx_return_t result = bnx_pool_reset(p);
    CHECK_EQUAL(BNX_OK, result.code);
    CHECK_EQUAL_C_INT(size - sizeof(bnx_pool_t), p->dsize);
    assert(p->large == NULL);
    POINTERS_EQUAL(&(p->d), p->current);
    bnx_pool_destroy(&p);
}


TEST(BnxPoolReset, execute_with_invalid_argument)
{
    bnx_return_t result = bnx_pool_reset(NULL);
    CHECK_EQUAL(BNX_ERROR, result.code);
}


TEST_GROUP(BnxPmalloc)
{
    void setup() override{}
    void teardown() override{}
    bnx_string_t log_file_path = bnx_str_literal("foo");
    bnx_log_t log = {
        .log_level=BNX_LOG_LEVEL_INFO,
        .fd=3,
        .fpath=&log_file_path,
        .writer=mockLogWriter,
        .next=NULL,
    };
};


TEST(BnxPmalloc, Boundary)
{
    typedef struct dtype_s {
        char foo;
        int bar;
        int hoge;
        const char *fuga;
    } dtype_t;

    typedef struct tests_s {
        size_t  size;
        std::vector<dtype_t> datas;
    } tests_t;

    std::string first = "first";
    std::string second = "second";

    tests_t tests[] = {
        {
            .size=256,
            .datas={
                {
                    .foo='f',
                    .bar=1,
                    .hoge=2,
                    .fuga=first.c_str()
                },
                {
                    .foo='s',
                    .bar=10,
                    .hoge=20,
                    .fuga=second.c_str()
                },
            }
        },
        {
            .size=sizeof(bnx_pool_t) + sizeof(dtype_t),
            .datas={
                {
                    .foo='f',
                    .bar=1,
                    .hoge=2,
                    .fuga=first.c_str()
                },
                {
                    .foo='s',
                    .bar=10,
                    .hoge=20,
                    .fuga=second.c_str()
                },
            }
        },
        {
            .size=sizeof(bnx_pool_t) + sizeof(dtype_t) - 1,
            .datas={
                {
                    .foo='f',
                    .bar=1,
                    .hoge=2,
                    .fuga=first.c_str()
                },
                {
                    .foo='s',
                    .bar=10,
                    .hoge=20,
                    .fuga=second.c_str()
                },
            }
        },
    };

    for (auto tt : tests) {
        bnx_pool_t *p = bnx_create_pool(tt.size, &log);
        CHECK_TRUE(p != NULL);

        uintptr_t prev = 0;
        for (auto data : tt.datas) {
            dtype_t *got = (dtype_t *)bnx_pmalloc(p, sizeof(dtype_t));
            CHECK_TRUE(got != NULL);

            uintptr_t curr = (uintptr_t)got;
            if (prev) {
                if (prev < curr) {
                    CHECK_TRUE(prev + sizeof(dtype_t) <= curr);
                } else {
                    CHECK_TRUE(curr + sizeof(dtype_t) <= prev);
                }
            }
            prev = curr;

            got->foo = data.foo;
            got->bar = data.bar;
            got->hoge = data.hoge;
            got->fuga = data.fuga;

            CHECK_EQUAL_C_CHAR(data.foo, got->foo);
            CHECK_EQUAL_C_INT(data.bar, got->bar);
            CHECK_EQUAL_C_INT(data.hoge, got->hoge);
            CHECK_EQUAL_C_STRING(data.fuga, got->fuga);
        }

        bnx_pool_destroy(&p);
    }
}


TEST_GROUP(BnxPcalloc)
{
    void setup() override {};
    void teardown() override {};
    bnx_string_t log_file_path = bnx_str_literal("foo");
    bnx_log_t log = {
        .log_level=BNX_LOG_LEVEL_INFO,
        .fd=3,
        .fpath=&log_file_path,
        .writer=mockLogWriter,
        .next=NULL,
    };
};


TEST(BnxPcalloc, CheckZeroClear)
{
    int sizes[4] = { 1, 1027, 1028, 1209 };
    bnx_pool_t *p = bnx_create_pool(1028, &log);

    for (size_t size : sizes) {
        unsigned char *got = (unsigned char *)bnx_pcalloc(p, size);
        for (size_t i = 0; i < size; ++i) {
            CHECK_EQUAL_C_INT(0, got[i]);
        }
    }
    bnx_pool_destroy(&p);
}


TEST(BnxPcalloc, Boundary)
{
    typedef struct dtype_s {
        char foo;
        int bar;
        int hoge;
        const char *fuga;
    } dtype_t;

    typedef struct tests_s {
        size_t  size;
        std::vector<dtype_t> datas;
    } tests_t;

    std::string first = "first";
    std::string second = "second";

    tests_t tests[] = {
        {
            .size=256,
            .datas={
                {
                    .foo='f',
                    .bar=1,
                    .hoge=2,
                    .fuga=first.c_str()
                },
                {
                    .foo='s',
                    .bar=10,
                    .hoge=20,
                    .fuga=second.c_str()
                },
            }
        },
        {
            .size=sizeof(bnx_pool_t) + sizeof(dtype_t),
            .datas={
                {
                    .foo='f',
                    .bar=1,
                    .hoge=2,
                    .fuga=first.c_str()
                },
                {
                    .foo='s',
                    .bar=10,
                    .hoge=20,
                    .fuga=second.c_str()
                },
            }
        },
        {
            .size=sizeof(bnx_pool_t) + sizeof(dtype_t) - 1,
            .datas={
                {
                    .foo='f',
                    .bar=1,
                    .hoge=2,
                    .fuga=first.c_str()
                },
                {
                    .foo='s',
                    .bar=10,
                    .hoge=20,
                    .fuga=second.c_str()
                },
            }
        },
    };

    for (auto tt : tests) {
        bnx_pool_t *p = bnx_create_pool(tt.size, &log);
        CHECK_TRUE(p != NULL);

        uintptr_t prev = 0;
        for (auto data : tt.datas) {
            dtype_t *got = (dtype_t *)bnx_pmalloc(p, sizeof(dtype_t));
            CHECK_TRUE(got != NULL);

            uintptr_t curr = (uintptr_t)got;
            if (prev) {
                if (prev < curr) {
                    CHECK_TRUE(prev + sizeof(dtype_t) <= curr);
                } else {
                    CHECK_TRUE(curr + sizeof(dtype_t) <= prev);
                }
            }
            prev = curr;

            got->foo = data.foo;
            got->bar = data.bar;
            got->hoge = data.hoge;
            got->fuga = data.fuga;

            CHECK_EQUAL_C_CHAR(data.foo, got->foo);
            CHECK_EQUAL_C_INT(data.bar, got->bar);
            CHECK_EQUAL_C_INT(data.hoge, got->hoge);
            CHECK_EQUAL_C_STRING(data.fuga, got->fuga);
        }

        bnx_pool_destroy(&p);
    }
}
