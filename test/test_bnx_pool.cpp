#include <CppUTest/CommandLineTestRunner.h>
#include <iostream>
#include "core/bnx_pool.h"

TEST_GROUP(BnxAlignPtr)
{
    uintptr_t zero     = 0b00000000;

    uintptr_t one      = 0b00000001;
    uintptr_t two      = 0b00000010;
    uintptr_t three    = 0b00000011;
    uintptr_t four     = 0b00000100;
    uintptr_t five     = 0b00000101;
    uintptr_t six      = 0b00000110;
    uintptr_t seven    = 0b00000111;
    uintptr_t eight    = 0b00001000;

    uintptr_t nine     = 0b00001001;
    uintptr_t ten      = 0b00001010;
    uintptr_t eleven   = 0b00001011;
    uintptr_t twelve   = 0b00001100;
    uintptr_t thirteen = 0b00001101;
    uintptr_t fourteen = 0b00001110;
    uintptr_t fifteen  = 0b00001111;
    uintptr_t sixteen  = 0b00010000;

    uintptr_t seventeen   = 0b00010001;
    uintptr_t eighteen    = 0b00010010;
    uintptr_t nineteen    = 0b00010011;
    uintptr_t twenty      = 0b00010100;
    uintptr_t twentyone   = 0b00010101;
    uintptr_t twentytwo   = 0b00010110;
    uintptr_t twentythree = 0b00010111;
    uintptr_t twentyfour  = 0b00011000;
};

TEST(BnxAlignPtr, execute_with_returning_zero)
{
    int pointer_block = 8;
    unsigned char *result = bnx_align_ptr(zero, pointer_block);
    POINTERS_EQUAL(result, zero);
}

TEST(BnxAlignPtr, execute_with_returning_eight)
{
    int pointer_block = 8;
    uintptr_t arr[8] = {one, two, three, four, five, six, seven, eight};
    for (int i = 0; i < 8; ++i) {
        unsigned char *result = bnx_align_ptr(arr[i], pointer_block);
        POINTERS_EQUAL(result, eight);
    }
}

TEST(BnxAlignPtr, execute_with_returning_sixteen)
{
    int pointer_block = 8;
    uintptr_t arr[8] = {nine, ten, eleven, twelve, thirteen, fourteen, fifteen, sixteen};
    for (int i = 0; i < 8; ++i) {
        unsigned char *result = bnx_align_ptr(arr[i], pointer_block);
        POINTERS_EQUAL(result, sixteen);
    }
}

TEST(BnxAlignPtr, execute_with_returning_twenty_four)
{
    int pointer_block = 8;
    uintptr_t arr[8] = {seventeen, eighteen, nineteen, twenty, twentyone, twentytwo, twentythree, twentyfour};
    for (int i = 0; i < 8; ++i) {
        unsigned char *result = bnx_align_ptr(arr[i], pointer_block);
        POINTERS_EQUAL(result, twentyfour);
    }
}


TEST_GROUP(BnxPoolInit) {};

TEST(BnxPoolInit, init_success)
{
    bnx_pool_t pool;
    bnx_code_e result = bnx_pool_init(&pool, 10);
    
    CHECK_EQUAL(BNX_OK, result);
    CHECK_EQUAL(10, pool.size);
    POINTERS_EQUAL(pool.start, pool.current);
    POINTERS_EQUAL(pool.end, (unsigned char *)pool.start + 10);
    POINTERS_EQUAL(pool.end, (unsigned char *)pool.buf + 10);
    POINTERS_EQUAL(NULL, pool.next);

    bnx_pool_destroy(&pool);
}

TEST(BnxPoolInit, init_with_null)
{
    bnx_code_e result = bnx_pool_init(NULL, 10);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}

TEST(BnxPoolInit, init_with_size_zero)
{
    bnx_pool_t *pool = NULL;
    bnx_code_e result = bnx_pool_init(pool, 0);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
    bnx_pool_destroy(pool);
}


TEST_GROUP(BnxPoolDestroy) {};

TEST(BnxPoolDestroy, destroy_pool_success)
{
    bnx_pool_t pool;
    bnx_pool_init(&pool, 24);
    bnx_code_e result = bnx_pool_destroy(&pool);
    CHECK_EQUAL(BNX_OK, result);
    POINTERS_EQUAL(NULL, pool.buf);
    CHECK_EQUAL(0, pool.size);
    POINTERS_EQUAL(NULL, pool.start);
    POINTERS_EQUAL(NULL, pool.end);
    POINTERS_EQUAL(NULL, pool.current);
    POINTERS_EQUAL(NULL, pool.next);
}

TEST(BnxPoolDestroy, execute_fail_with_invalid_argument)
{
    bnx_code_e result = bnx_pool_destroy(NULL);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}


TEST_GROUP(BnxPoolReset) {};

TEST(BnxPoolReset, reset_success)
{
    bnx_pool_t pool;
    bnx_pool_init(&pool, 20);
    bnx_code_e result = bnx_pool_reset(&pool);

    CHECK_EQUAL(BNX_OK, result);
    POINTERS_EQUAL(pool.current, pool.start);
    bnx_pool_destroy(&pool);
}

TEST(BnxPoolReset, execute_with_invalid_argument)
{
    bnx_code_e result = bnx_pool_reset(NULL);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}


TEST_GROUP(BnxPcalloc)
{
    size_t MEMORY_BLOCK_SIZE = sizeof(uintptr_t);
    int get_memory_block_distance(int p, int memory_block_size)
    {
        int next_start = (p + memory_block_size - 1) & ~(memory_block_size - 1);
        return next_start - p;
    }
};

TEST(BnxPcalloc, allocate_success)
{
    int align_size = 4;

    bnx_pool_t large_pool;
    bnx_pool_init(&large_pool, 16);

    bnx_pool_t new_pool;
    bnx_code_e result = bnx_pcalloc(&new_pool, &large_pool, align_size);
    
    CHECK_EQUAL(4, new_pool.size);
    POINTERS_EQUAL(new_pool.start, new_pool.current);
    POINTERS_EQUAL(new_pool.end, (unsigned char *)new_pool.start + align_size - 1);
    POINTERS_EQUAL(new_pool.end, (unsigned char *)new_pool.buf + align_size - 1);
    POINTERS_EQUAL(NULL, new_pool.next);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPcalloc, check_memory_allocated_sequencely_with_uintptr_size)
{
    int align_size = MEMORY_BLOCK_SIZE;

    bnx_pool_t large_pool;
    bnx_pool_init(&large_pool, 64);

    bnx_pool_t new_pool;
    bnx_pool_t another_new_pool;

    bnx_pcalloc(&new_pool, &large_pool, align_size);
    bnx_code_e result = bnx_pcalloc(&another_new_pool, &large_pool, align_size);
    
    CHECK_EQUAL(BNX_OK, result);
    CHECK_EQUAL(align_size, another_new_pool.size);
    POINTERS_EQUAL(another_new_pool.start, another_new_pool.current);
    POINTERS_EQUAL(another_new_pool.buf, (unsigned char *)another_new_pool.start);
    POINTERS_EQUAL(another_new_pool.end, (unsigned char *)another_new_pool.start + align_size - 1);
    POINTERS_EQUAL(NULL, another_new_pool.next);

    POINTERS_EQUAL(new_pool.end + 1, another_new_pool.start);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPcalloc, check_memory_allocated_sequencely_with_not_uintptr_size)
{
    int align_size = 10;

    bnx_pool_t large_pool;
    bnx_pool_init(&large_pool, 64);

    bnx_pool_t new_pool;
    bnx_pool_t another_new_pool;

    bnx_pcalloc(&new_pool, &large_pool, align_size);
    bnx_code_e result = bnx_pcalloc(&another_new_pool, &large_pool, align_size);
    
    CHECK_EQUAL(BNX_OK, result);
    CHECK_EQUAL(align_size, another_new_pool.size);
    POINTERS_EQUAL(another_new_pool.start, another_new_pool.current);
    POINTERS_EQUAL(another_new_pool.buf, (unsigned char *)another_new_pool.start);
    POINTERS_EQUAL(another_new_pool.end, (unsigned char *)another_new_pool.start + align_size - 1);
    POINTERS_EQUAL(NULL, another_new_pool.next);

    int memory_block_distance = get_memory_block_distance(align_size, MEMORY_BLOCK_SIZE);
    POINTERS_EQUAL(new_pool.end + 1 + memory_block_distance, another_new_pool.start);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPcalloc, execute_fail_with_null_new_pool)
{
    bnx_pool_t large_pool;
    bnx_code_e result = bnx_pcalloc(NULL, &large_pool, 16);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}

TEST(BnxPcalloc, execute_fail_with_null_large_pool)
{
    bnx_pool_t new_pool;
    bnx_code_e result = bnx_pcalloc(&new_pool, NULL, 16);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}

TEST(BnxPcalloc, execute_fail_with_size_zero)
{
    bnx_pool_t new_pool;
    bnx_pool_t large_pool;
    bnx_code_e result = bnx_pcalloc(&new_pool, &large_pool, 0);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}

TEST(BnxPcalloc, execute_with_too_big_size)
{
    bnx_pool_t new_pool;
    bnx_pool_t large_pool;
    bnx_pool_init(&large_pool, 16);

    bnx_code_e result = bnx_pcalloc(&new_pool, &large_pool, 20);

    CHECK_EQUAL(BNX_MEMORY_ERROR, result);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPcalloc, execute_with_just_size)
{
    bnx_pool_t new_pool;
    bnx_pool_t large_pool;
    bnx_pool_init(&large_pool, 16);

    bnx_code_e result = bnx_pcalloc(&new_pool, &large_pool, 16);

    CHECK_EQUAL(BNX_OK, result);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPcalloc, execute_fail_with_too_big_size_with_two_allocation)
{
    bnx_pool_t large_pool;
    bnx_pool_t new_pool;
    bnx_pool_t another_new_pool;

    bnx_pool_init(&large_pool, 16);
    bnx_pcalloc(&new_pool, &large_pool, 8);
    bnx_code_e result = bnx_pcalloc(&another_new_pool, &large_pool, 16);
    CHECK_EQUAL(BNX_MEMORY_ERROR, result);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPcalloc, check_aligned_memory_buffer_is_zero_cleared)
{
    bnx_pool_t large_pool;
    bnx_pool_t new_pool;
    bnx_pool_init(&large_pool, 8);
    bnx_pcalloc(&new_pool, &large_pool, 2);
    CHECK_EQUAL(*(new_pool.start), 0);
    CHECK_EQUAL(*(new_pool.start + 1), 0);

    bnx_pool_destroy(&large_pool);
}


TEST_GROUP(BnxPmalloc)
{
    size_t MEMORY_BLOCK_SIZE = sizeof(uintptr_t);
    int get_memory_block_distance(int p, int memory_block_size)
    {
        int next_start = (p + memory_block_size - 1) & ~(memory_block_size - 1);
        return next_start - p;
    }
};

TEST(BnxPmalloc, allocate_success)
{
    int align_size = 8;

    bnx_pool_t large_pool;
    bnx_pool_t new_pool;
    bnx_pool_init(&large_pool, 16);
    bnx_pmalloc(&new_pool, &large_pool, align_size);


    CHECK_EQUAL(8, new_pool.size);
    POINTERS_EQUAL(new_pool.start, new_pool.current);
    POINTERS_EQUAL(new_pool.end, (unsigned char *)new_pool.start + align_size - 1);
    POINTERS_EQUAL(new_pool.end, (unsigned char *)new_pool.buf + align_size - 1);
    POINTERS_EQUAL(NULL, new_pool.next);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPmalloc, check_memory_allocated_sequencely_with_uintptr_size)
{
    int align_size = MEMORY_BLOCK_SIZE;

    bnx_pool_t large_pool;
    bnx_pool_init(&large_pool, 64);

    bnx_pool_t new_pool;
    bnx_pool_t another_new_pool;

    bnx_pmalloc(&new_pool, &large_pool, align_size);
    bnx_code_e result = bnx_pmalloc(&another_new_pool, &large_pool, align_size);
    
    CHECK_EQUAL(BNX_OK, result);
    CHECK_EQUAL(align_size, another_new_pool.size);
    POINTERS_EQUAL(another_new_pool.start, another_new_pool.current);
    POINTERS_EQUAL(another_new_pool.buf, (unsigned char *)another_new_pool.start);
    POINTERS_EQUAL(another_new_pool.end, (unsigned char *)another_new_pool.start + align_size - 1);
    POINTERS_EQUAL(NULL, another_new_pool.next);

    POINTERS_EQUAL(new_pool.end + 1, another_new_pool.start);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPmalloc, check_memory_allocated_sequencely_with_not_uintptr_size)
{
    int align_size = 10;

    bnx_pool_t large_pool;
    bnx_pool_init(&large_pool, 64);

    bnx_pool_t new_pool;
    bnx_pool_t another_new_pool;

    bnx_pmalloc(&new_pool, &large_pool, align_size);
    bnx_code_e result = bnx_pmalloc(&another_new_pool, &large_pool, align_size);
    
    CHECK_EQUAL(BNX_OK, result);
    CHECK_EQUAL(align_size, another_new_pool.size);
    POINTERS_EQUAL(another_new_pool.start, another_new_pool.current);
    POINTERS_EQUAL(another_new_pool.buf, (unsigned char *)another_new_pool.start);
    POINTERS_EQUAL(another_new_pool.end, (unsigned char *)another_new_pool.start + align_size - 1);
    POINTERS_EQUAL(NULL, another_new_pool.next);

    int memory_block_distance = get_memory_block_distance(align_size, MEMORY_BLOCK_SIZE);
    POINTERS_EQUAL(new_pool.end + 1 + memory_block_distance, another_new_pool.start);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPmalloc, execute_fail_with_null_new_pool)
{
    bnx_pool_t large_pool;
    bnx_code_e result = bnx_pmalloc(NULL, &large_pool, 16);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}

TEST(BnxPmalloc, execute_fail_with_null_large_pool)
{
    bnx_pool_t new_pool;
    bnx_code_e result = bnx_pmalloc(&new_pool, NULL, 16);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}

TEST(BnxPmalloc, execute_fail_with_size_zero)
{
    bnx_pool_t new_pool;
    bnx_pool_t large_pool;
    bnx_code_e result = bnx_pmalloc(&new_pool, &large_pool, 0);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}

TEST(BnxPmalloc, execute_with_too_big_size)
{
    bnx_pool_t new_pool;
    bnx_pool_t large_pool;
    bnx_pool_init(&large_pool, 16);

    bnx_code_e result = bnx_pmalloc(&new_pool, &large_pool, 20);

    CHECK_EQUAL(BNX_MEMORY_ERROR, result);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPmalloc, execute_with_just_size)
{
    bnx_pool_t new_pool;
    bnx_pool_t large_pool;
    bnx_pool_init(&large_pool, 16);

    bnx_code_e result = bnx_pcalloc(&new_pool, &large_pool, 16);

    CHECK_EQUAL(BNX_OK, result);

    bnx_pool_destroy(&large_pool);
}

TEST(BnxPmalloc, execute_fail_with_too_big_size_with_two_allocation)
{
    bnx_pool_t large_pool;
    bnx_pool_t new_pool;
    bnx_pool_t another_new_pool;

    bnx_pool_init(&large_pool, 16);
    bnx_pcalloc(&new_pool, &large_pool, 8);
    bnx_code_e result = bnx_pcalloc(&another_new_pool, &large_pool, 16);
    CHECK_EQUAL(BNX_MEMORY_ERROR, result);

    bnx_pool_destroy(&large_pool);
}
