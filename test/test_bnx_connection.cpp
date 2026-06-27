#include <CppUTest/CommandLineTestRunner.h>
#include <climits>
#include <vector>
#include "CppUTest/TestHarness_c.h"
#include "CppUTest/UtestMacros.h"
#include "core/bnx_connection.h"

TEST_GROUP(BnxCreateConnection)
{
    void setup() override{}
    void teardown() override{}

    typedef struct tests_s {
        std::string       label;
        int               fd;
    } tests_t;
};


TEST(BnxCreateConnection, PositiveBoundary)
{
    tests_t tests[] = {
        {"If zero is passed to fd.", 0},
        {"If positive value is passed to fd.", 1},
        {"If INT_MAX is passed to fd", INT_MAX},
    };

    for (const auto& tt: tests) {
        bnx_connection_t *c = bnx_create_connection(tt.fd);
        CHECK_TEXT(c != NULL, tt.label.c_str());
        LONGS_EQUAL_TEXT(tt.fd, c->fd, tt.label.c_str());
        LONGS_EQUAL_TEXT(sizeof(c->address), c->addrlen, tt.label.c_str());

        struct sockaddr_storage addr;
        std::memset(&addr, 0, sizeof(addr));
        MEMCMP_EQUAL_TEXT(&addr, &(c->address), sizeof(addr), tt.label.c_str());

        std::vector<char> expected_buf(BNX_REQUEST_BUF, 0);
        MEMCMP_EQUAL(expected_buf.data(), c->rbuf, BNX_REQUEST_BUF);

        LONGS_EQUAL_TEXT(-1, c->recieved, tt.label.c_str());
        POINTERS_EQUAL_TEXT(NULL, c->next, tt.label.c_str());
        POINTERS_EQUAL_TEXT(NULL, c->connection_pool, tt.label.c_str());
        CHECK_EQUAL_C_BOOL_TEXT(false, c->linked, tt.label.c_str());

        bnx_free_connection(&c);
    }
}


TEST(BnxCreateConnection, NegativeBoundary)
{
    tests_t tests[] = {
        {"If negative value is passed to fd.", -1},
        {"If INT_MIN is passed to fd", INT_MIN},
    };

    for (const auto& tt : tests) {
        bnx_connection_t *c = bnx_create_connection(tt.fd);
        CHECK_EQUAL_C_POINTER_TEXT(NULL, c, tt.label.c_str());
    }
}


TEST_GROUP(BnxFreeConnection)
{
    void setup() override{}
    void teardown() override{}

    typedef struct tests_s {
        bnx_connection_t *con;
    } test_t;
};

TEST(BnxFreeConnection, PositiveBoundary)
{
    bnx_connection_t *three = bnx_create_connection(3);
    CHECK(three != NULL);

    test_t tests[] = {
        {three},
        {NULL},
    };

    for (auto& tt : tests) {
        // Detect memory leak by ASan
        bnx_free_connection(&tt.con);
    }
}


TEST_GROUP(BnxPrependConnection)
{
    void setup() override{}
    void teardown() override{}
};


TEST(BnxPrependConnection, PositiveBoundary)
{
    typedef struct tests_s {
        std::string       label;
        bnx_connection_t *chain;
        bnx_connection_t *prepend;
        bnx_connection_t *expect1;
        bnx_connection_t *expect2;
    } tests_t;

    bnx_connection_t *chain = bnx_create_connection(3);
    bnx_connection_t *prepend = bnx_create_connection(4);
    bnx_connection_t *chain2 = bnx_create_connection(3);
    bnx_connection_t *prepend3 = bnx_create_connection(4);
    tests_t tests[] = {
        // label,    chain, prepend, expect1, expect2
        { "Test when 'chain' and 'prepend' argument is not NULL: ", chain, prepend, chain, prepend },
        { "Test when 'prepend' argument is NULL", chain2, NULL, NULL, chain2 },
        { "Test when 'chain' argument is NULL", NULL, prepend3, NULL, prepend3 },
    };

    for (auto& tt : tests) {
        bnx_connection_t *result = bnx_prepend_connection(tt.chain, tt.prepend);

        std::string msg = tt.label + "failed to create connection chain";
        CHECK_TEXT(result, msg.c_str());

        msg = tt.label + "connections are not connected properly";
        CHECK_EQUAL_C_POINTER_TEXT(tt.expect1, result->next, msg.c_str());

        msg = tt.label + "linked connection should be located at the first of the chain";
        CHECK_EQUAL_C_POINTER_TEXT(tt.expect2, result, msg.c_str());

        if (result != NULL) {
            bnx_free_connection(&result);
        }
    }
}


TEST(BnxPrependConnection, PositiveRobustnessTesting)
{
    typedef struct tests_s {
        std::string       label;
        bnx_connection_t *chain;
        bnx_connection_t *prepend;
        bnx_connection_t *expect;
    } tests_t;

    bnx_connection_t *three = bnx_create_connection(3);
    bnx_connection_t *four = bnx_create_connection(4);
    bnx_connection_t *five = bnx_create_connection(5);
    // three->four->five
    bnx_connection_t *chain = bnx_prepend_connection(five, four);
    chain = bnx_prepend_connection(chain, three);

    tests_t tests[] = {
        // label, chain, prepend, expect
        {"test if duplicated connection is not prepended", chain, four, three }
    };

    for (auto& tt : tests) {
        bnx_connection_t *result = bnx_prepend_connection(tt.chain, tt.prepend);
        CHECK_EQUAL_C_POINTER_TEXT(tt.expect, result, tt.label.c_str());
    }

    bnx_free_connection(&chain);
}

TEST_GROUP(BnxGetConnection)
{
    void setup() override{}

    void teardown() override{}

    typedef struct tests_s {
        bnx_connection_t  *arg_chain;
        int               arg_fd;
        bnx_connection_t  *expected;
        std::string       label;
    } tests_t;
};


TEST(BnxGetConnection, PositiveEquivalencePartitioning)
{
    // five->four->three
    bnx_connection_t *three = bnx_create_connection(3);
    bnx_connection_t *four = bnx_create_connection(4);
    bnx_connection_t *five = bnx_create_connection(5);
    bnx_connection_t *start = bnx_prepend_connection(three, four);
    start = bnx_prepend_connection(start, five);

    CHECK_EQUAL(start->fd, 5);
    CHECK_EQUAL(start->next->fd, 4);
    CHECK_EQUAL(start->next->next->fd, 3);

    tests_t tests[] = {
        // { arg_chain, arg_fd, expected connection, label }
        { start, three->fd, three, "case 1: find the tail of the connection chain"  },
        { start, four->fd, four, "case 2: find the middle of the connection chain" },
        { start, five->fd, five, "case 3: find the head of the connection chain" },
        { NULL, three->fd, NULL, "case 4: test if return value is NULL when chain is NULL" },
    };

    for (auto& tt : tests) {
        bnx_connection_t *got = bnx_get_connection(tt.arg_chain, tt.arg_fd);
        CHECK_EQUAL_C_POINTER(tt.expected, got);
    }
    bnx_free_connection(&start);
}

TEST(BnxGetConnection, PositiveBoundary_GivenFileDescriptor)
{
    // zero -> one -> two -> three -> four -> int_max
    bnx_connection_t *zero = bnx_create_connection(0);
    bnx_connection_t *one = bnx_create_connection(1);
    bnx_connection_t *two = bnx_create_connection(2);
    bnx_connection_t *three = bnx_create_connection(3);
    bnx_connection_t *four = bnx_create_connection(4);
    bnx_connection_t *int_max = bnx_create_connection(INT_MAX);
    bnx_connection_t *start = bnx_prepend_connection(int_max, four);
    start = bnx_prepend_connection(start, three);
    start = bnx_prepend_connection(start, two);
    start = bnx_prepend_connection(start, one);
    start = bnx_prepend_connection(start, zero);

    CHECK_EQUAL(start->fd, 0);
    CHECK_EQUAL(start->next->fd, 1);
    CHECK_EQUAL(start->next->next->fd, 2);
    CHECK_EQUAL(start->next->next->next->fd, 3);
    CHECK_EQUAL(start->next->next->next->next->fd, 4);
    CHECK_EQUAL(start->next->next->next->next->next->fd, INT_MAX);

    tests_t tests[] = {
        // { arg_chain, arg_fd, expected connection, label }
        { start, -1, NULL, "case 1: test if negative value is given as a file descriptor" },
        { start, 0, zero, "case 2: test if 0 is given as a file descriptor" },
        { start, INT_MAX, int_max, "case 3: test if INT_MAX is given as a file descriptor" },
    };

    for (auto& tt : tests) {
        bnx_connection_t *got = bnx_get_connection(tt.arg_chain, tt.arg_fd);
        CHECK_EQUAL_C_POINTER(tt.expected, got);
    }
    bnx_free_connection(&start);
}

TEST(BnxGetConnection, Negative_ConnectionChainIsCirculated)
{
    // zero -> one -> two -> three -> four -> one -> five
    bnx_connection_t *zero = bnx_create_connection(0);
    bnx_connection_t *one = bnx_create_connection(1);
    bnx_connection_t *two = bnx_create_connection(2);
    bnx_connection_t *three = bnx_create_connection(3);
    bnx_connection_t *four = bnx_create_connection(4);
    bnx_connection_t *five = bnx_create_connection(5);
    bnx_connection_t *start = bnx_prepend_connection(five, one);
    start = bnx_prepend_connection(start, four);
    start = bnx_prepend_connection(start, three);
    start = bnx_prepend_connection(start, two);
    // connect one to chain manually. (bnx_prepend_connection won't prepend duplicate connection)
    one->next = two;
    one->linked = true;
    start->linked = true;
    start = one;
    start = bnx_prepend_connection(start, zero);

    CHECK_EQUAL(0, start->fd);
    CHECK_EQUAL(1, start->next->fd);
    CHECK_EQUAL(2, start->next->next->fd);
    CHECK_EQUAL(3, start->next->next->next->fd);
    CHECK_EQUAL(4, start->next->next->next->next->fd);
    CHECK_EQUAL(1, start->next->next->next->next->next->fd);
    CHECK_EQUAL(2, start->next->next->next->next->next->next->fd);

    tests_t tests[] = {
        // { arg_chain, arg_fd, expected connection, label }
        { start, 1, one, "case 1: test if target is found" },
        { start, 3, three, "case 2: test if target is found before circulation" },
        { start, 5, NULL, "case 3: test if bnx_create_connection detects circulation and returns NULL" },
    };

    for (auto& tt : tests) {
        bnx_connection_t *got = bnx_get_connection(tt.arg_chain, tt.arg_fd);

        CHECK_EQUAL_C_POINTER(tt.expected, got);
    }

    // Tear down.
    // manually free memory because bnx_free_connection can't free circulating connection chain.
    one->next = NULL;
    bnx_free_connection(&zero);
    // after one is freed. set four->next from one to NULL.
    four->next = NULL;
    bnx_free_connection(&two);
    bnx_free_connection(&five);
}
