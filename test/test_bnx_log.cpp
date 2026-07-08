#include <CppUTest/CommandLineTestRunner.h>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <CppUTest/TestHarness_c.h>
#include <CppUTest/UtestMacros.h>
extern "C" {
    #include "core/bnx_log.h"
    #include "core/bnx_string.h"
}

void mock(bnx_log_t *log, const char *buf, size_t size){
    return;
}

void mock_writer(bnx_log_t *log, const char *msg, size_t size)
{
    if (log == NULL || log->fd < 0 || msg == NULL) {
        return;
    }
    write(log->fd, msg, size);
}

TEST_GROUP(BnxInitLog)
{
    void setup() override{}
    void teardown() override{}

    typedef struct input_s {
        bnx_log_level_t    log_level;
        bnx_string_t       fpath;
        bnx_log_writer_pt  writer;
    } input_t;

    typedef struct expect_s {
        bnx_log_level_t    level;
        int                fd;
        char              *fname_data;
        size_t             fname_len;
        bnx_log_writer_pt  writer;
        bnx_log_t         *next;
    } expect_t;

    typedef struct tests_s {
        std::string     label;
        input_t         input;
        bnx_return_t    expect_return;
        bnx_log_t       expect_log;
    } test_t;
};

TEST(BnxInitLog, PositiveBoundary)
{
    bnx_string_t fpath = bnx_str_literal("/foo/bar.log");
    std::vector<test_t> tests = {
        {
            .label = "Case-1",
            .input = (input_t){.log_level=BNX_LOG_LEVEL_INFO, .fpath=fpath, .writer=mock},
            .expect_return = bnx_success(BNX_OK),
            .expect_log = (bnx_log_t){.log_level=BNX_LOG_LEVEL_INFO, .fd=-1, .fpath=&fpath, .writer=mock, .next=NULL}
        }
    };
    for (auto& tt : tests) {
        bnx_log_t log;
        memset(&log, 0, sizeof(log));
        bnx_return_t r = bnx_init_log(&log, tt.input.log_level, &(tt.input.fpath), tt.input.writer);

        std::string msg;
        msg = tt.label + ": check equal return code";
        CHECK_EQUAL_TEXT(tt.expect_return.code, r.code, msg.c_str());

        msg = tt.label + ": check equal log level";
        CHECK_EQUAL_C_INT_TEXT(tt.expect_log.log_level, log.log_level, msg.c_str());

        msg = tt.label + ": check equal fd";
        CHECK_EQUAL_C_INT_TEXT(tt.expect_log.fd, log.fd, msg.c_str());

        msg = tt.label + ": check equal file path";
        CHECK_EQUAL_C_STRING_TEXT((tt.expect_log.fpath)->data, (log.fpath)->data, msg.c_str());

        msg = tt.label + ": check equal writer";
        CHECK_EQUAL_TEXT(tt.expect_log.writer, log.writer, msg.c_str());
    }
}

TEST(BnxInitLog, NegativeBoundary_InvalidArgument)
{
    // Case 1 (argument log is NULL)
    {
        bnx_string_t fpath = bnx_str_literal("/prefix/foo/access.log");
        bnx_return_t r = bnx_init_log(NULL, BNX_LOG_LEVEL_INFO, &fpath, mock);

        std::string msg = "NULL is given to a 'log' argument";
        CHECK_EQUAL_TEXT(BNX_ERROR, r.code, msg.c_str());
    }

    // Case 2 (argument fpath is NULL)
    {
        bnx_log_t log;
        memset(&log, 0, sizeof(bnx_log_t));
        bnx_return_t r = bnx_init_log(&log, BNX_LOG_LEVEL_INFO, NULL, mock);

        std::string msg = "NULL is given to a 'fpath' argument";
        CHECK_EQUAL_TEXT(BNX_ERROR, r.code, msg.c_str());
    }

    // Case 3 (argument writer is NULL)
    {
        bnx_string_t fpath = bnx_str_literal("/prefix/foo/access.log");
        bnx_log_t log;
        memset(&log, 0, sizeof(bnx_log_t));
        bnx_return_t r = bnx_init_log(&log, BNX_LOG_LEVEL_INFO, &fpath, NULL);

        std::string msg = "NULL is given to a 'writer' argument";
        CHECK_EQUAL_TEXT(BNX_ERROR, r.code, msg.c_str());
    }
}

TEST_GROUP(BnxLogFile)
{
    void setup() override{}
    void teardown() override{}
};

TEST(BnxLogFile, OpenAndClose_Positive)
{
    bnx_log_t log;
    bnx_string_t path = bnx_str_literal("./BnxLogFile_OpenAndClose_Positive.log");
    bnx_init_log(&log, BNX_LOG_LEVEL_INFO, &path, mock);

    bnx_return_t r;
    r = bnx_open_log_file(&log);
    CHECK_EQUAL_C_INT_TEXT(BNX_OK, r.code, "test if bnx_open_log_file succeed");
    CHECK_TEXT(log.fd > 0, "test if file descriptor is set to log.fd");

    r = bnx_close_log_file(&log);
    CHECK_EQUAL_C_INT_TEXT(BNX_OK, r.code, "test if bnx_close_log_file succeed");
    CHECK_EQUAL_C_INT_TEXT(-1, log.fd, "test if -1 is set to log.fd after closing file descriptor");

    remove("./BnxLogFile_OpenAndClose_Positive.log");
}

TEST(BnxLogFile, OpenAndClose_Negative_InvalidArgument)
{
    // case when 'log' is NULL
    {
        bnx_return_t r;
        r = bnx_open_log_file(NULL);
        CHECK_EQUAL_C_INT_TEXT(BNX_ERROR, r.code, "case when 'log' is NULL");
    }

    // case when log.fpath is NULL
    {
        bnx_log_t log;
        bnx_return_t r;

        bnx_string_t path = bnx_str_literal("foo");
        bnx_init_log(&log, BNX_LOG_LEVEL_INFO, &path, mock);

        // set log->fpath->data NULL for testing
        log.fpath = NULL;
        r = bnx_open_log_file(&log);
        CHECK_EQUAL_C_INT_TEXT(BNX_ERROR, r.code, "case when 'log.fpath' is NULL");
    }
 
    // case when log.fpath->data is NULL
    {
        bnx_log_t log;
        bnx_return_t r;
        
        bnx_string_t path = bnx_str_literal("bar");
        bnx_init_log(&log, BNX_LOG_LEVEL_INFO, &path, mock);

        // set log.fpath->data NULL for testing
        log.fpath->data = NULL;
        log.fpath->len = 0;
        r = bnx_open_log_file(&log);
        CHECK_EQUAL_C_INT_TEXT(BNX_ERROR, r.code, "case when 'log.fpath->data' is NULL");
    }

    // case when NULL is given to bnx_close_log_file
    {
        bnx_return_t r = bnx_close_log_file(NULL);
        CHECK_EQUAL_C_INT_TEXT(BNX_ERROR, r.code, "case when NULL is given to 'log'");
    }

    // case when argument 'log' has negative number as fd
    {
        bnx_log_t log;
        bnx_string_t path = bnx_str_literal("bar");
        
        bnx_init_log(&log, BNX_LOG_LEVEL_INFO, &path, mock);

        // replace log.fd to -1 (for negative case testing)
        log.fd = -1;
        bnx_return_t r = bnx_close_log_file(&log);
        CHECK_EQUAL_C_INT_TEXT(BNX_ERROR, r.code, "case when log.fd has negative number");
    }
}

TEST(BnxLogFile, OpenAndClose_Negative_ReopenFile)
{
    bnx_log_t log;
    bnx_string_t path = bnx_str_literal("./BnxLogFile_OpenAndClose_Negative_ReopenFile.log");
    bnx_init_log(&log, BNX_LOG_LEVEL_INFO, &path, mock);

    bnx_return_t r;
    bnx_open_log_file(&log);
    // expect reopening to be failed
    r = bnx_open_log_file(&log);
    
    CHECK_EQUAL_C_INT_TEXT(BNX_ERROR, r.code, "test if reopen log file(bnx_open_log_file) fails");

    bnx_close_log_file(&log);

    remove("./BnxLogFile_OpenAndClose_Negative_ReopenFile.log");
}

TEST_GROUP(BnxLogWriter)
{
    void setup() override{}
    void teardown() override{}

    typedef struct test_s {
        const char *text;
        size_t      size;
        const char *expect;
        const char *label;
    } test_t;
};

TEST(BnxLogWriter, PositiveBoundary)
{
    bnx_log_t log;
    memset(&log, 0, sizeof(log));

    bnx_string_t fpath = bnx_str_literal("./foo.log");
    bnx_init_log(&log, BNX_LOG_LEVEL_INFO, &fpath, bnx_log_writer);

    // create test seeds
    test_t tests[] = {
        {.text = "hello world", .size=0, .expect="", .label="case when 0 is given to a size argument"},
        {.text = "hello world", .size=sizeof("hello world") - 2, .expect="hello worl", .label="case when size is lesser than text length"},
        {.text = "hello world", .size=sizeof("hello world") - 1, .expect="hello world", .label="case when size is equivalent to text length(without \0)"},
        {.text = "hello world", .size=sizeof("hello world"), .expect="hello world", .label="case when size is equivalent to (text + \0) length"},
        // Giving sizeof("hello world") + 1 to size will result in memory bug. UT does not test this case.
    };

    for (auto &tt : tests) {
        // mock
        // replace log->fd to fileno(tmpfile())
        FILE *fp = tmpfile();
        CHECK(fp);
        int tmpfd = fileno(fp);
        log.fd = tmpfd;

        // execute
        log.writer(&log, tt.text, tt.size);
 
        // assertion
        rewind(fp);
        char written[16] = {0};
        int read = fread(written, sizeof(char), 16, fp);
        STRCMP_EQUAL_TEXT(tt.expect, written, tt.label);
        
        // free
        fclose(fp);
    }
}

TEST(BnxLogWriter, NegativeBoundary_InvalidArgument)
{
    bnx_log_t log;
    memset(&log, 0, sizeof(log));

    bnx_string_t fpath = bnx_str_literal("./foo.log");
    bnx_init_log(&log, BNX_LOG_LEVEL_INFO, &fpath, bnx_log_writer);

    // case when NULL is given to a 'log' argument.
    {
        // mock
        // replace log->fd to fileno(tmpfile())
        FILE *fp = tmpfile();
        CHECK(fp);
        int tmpfd = fileno(fp);
        log.fd = tmpfd;

        // execute
        log.writer(NULL, "hello world", sizeof("hello world") -1);
 
        // assertion
        rewind(fp);
        char written[16] = {0};
        int read = fread(written, sizeof(char), 16, fp);
        std::string expect = "";
        STRCMP_EQUAL_TEXT(expect.c_str(), written, "case when NULL is given to a 'log' argument");

        fclose(fp);
    }

    // case when NULL is given to a 'msg' argument.
    {
        // mock
        // replace log->fd to fileno(tmpfile())
        FILE *fp = tmpfile();
        CHECK(fp);
        int tmpfd = fileno(fp);
        log.fd = tmpfd;

        // execute
        log.writer(&log, NULL, sizeof("hello world") -1);
 
        // assertion
        rewind(fp);
        char written[16] = {0};
        int read = fread(written, sizeof(char), 16, fp);
        std::string expect = "";
        STRCMP_EQUAL_TEXT(expect.c_str(), written, "case when NULL is given to a 'msg' argument");

        fclose(fp);
    }
}

TEST(BnxLogWriter, Boundary_LogFD)
{
    bnx_log_t log;
    memset(&log, 0, sizeof(log));

    bnx_string_t fpath = bnx_str_literal("./foo.log");
    bnx_init_log(&log, BNX_LOG_LEVEL_INFO, &fpath, bnx_log_writer);

    // case when log->fd has negative number(INT_MIN)(log file is not open).
    {
        // mock
        // replace log->fd to fileno(tmpfile())
        FILE *fp = tmpfile();
        CHECK(fp);
        int tmpfd = fileno(fp);
        log.fd = INT_MIN;

        // execute
        log.writer(&log, "hello world", sizeof("hello world") -1);
 
        // assertion
        rewind(fp);
        char written[16] = {0};
        int read = fread(written, sizeof(char), 16, fp);
        std::string expect = "";
        char label[64];
        snprintf(label, 64, "test if bnx_log_writer writes log(read %d bytes)", read);
        STRCMP_EQUAL_TEXT(expect.c_str(), written, label);

        fclose(fp);
    }
    
    // case when log->fd has negative number(-1)(log file is not open).
    {
        // mock
        // replace log->fd to fileno(tmpfile())
        FILE *fp = tmpfile();
        CHECK(fp);
        int tmpfd = fileno(fp);
        log.fd = -1;

        // execute
        log.writer(&log, "hello world", sizeof("hello world") -1);
 
        // assertion
        rewind(fp);
        char written[16] = {0};
        int read = fread(written, sizeof(char), 16, fp);
        std::string expect = "";
        char label[64];
        snprintf(label, 64, "test if bnx_log_writer writes log(read %d bytes)", read);
        STRCMP_EQUAL_TEXT(expect.c_str(), written, label);

        fclose(fp);
    }
}


TEST_GROUP(BnxWriteLogs)
{
    void setup() override{}
    void teardown() override{}

    typedef struct test_s {
        std::string label;
        bnx_log_level_t input_log_level;
        const char *expect_debug_log;
        const char *expect_info_log;
        const char *expect_error_log;
        const char *expect_critical_log;
    } test_t;
};

TEST(BnxWriteLogs, Boundary_GiveDifferentLogLevel)
{
    bnx_string_t fpath = bnx_str_literal("foo");

    bnx_log_t *head;
    bnx_log_t debug, info, error, critical;
    memset(&debug, 0, sizeof(debug));
    memset(&info, 0, sizeof(info));
    memset(&error, 0, sizeof(error));
    memset(&critical, 0, sizeof(critical));

    bnx_init_log(&debug, BNX_LOG_LEVEL_DEBUG, &fpath, mock_writer);
    bnx_init_log(&info, BNX_LOG_LEVEL_INFO, &fpath, mock_writer);
    bnx_init_log(&error, BNX_LOG_LEVEL_ERROR, &fpath, mock_writer);
    bnx_init_log(&critical, BNX_LOG_LEVEL_CRIT, &fpath, mock_writer);
 
    // connect logs
    debug.next = &info;
    info.next = &error;
    error.next = &critical;
    head = &debug;

    test_t tests[] = {
        {
            .label="case when log-level is CRIT",
            .input_log_level=BNX_LOG_LEVEL_CRIT,
            .expect_debug_log="hello world !",
            .expect_info_log="hello world !",
            .expect_error_log="hello world !",
            .expect_critical_log="hello world !",
        },
        {
            .label="case when log-level is ERROR",
            .input_log_level=BNX_LOG_LEVEL_ERROR,
            .expect_debug_log="hello world !",
            .expect_info_log="hello world !",
            .expect_error_log="hello world !",
            .expect_critical_log="",
        },
        {
            .label="case when log-level is INFO",
            .input_log_level=BNX_LOG_LEVEL_INFO,
            .expect_debug_log="hello world !",
            .expect_info_log="hello world !",
            .expect_error_log="",
            .expect_critical_log="",
        },
        {
            .label="case when log-level is DEBUG",
            .input_log_level=BNX_LOG_LEVEL_DEBUG,
            .expect_debug_log="hello world !",
            .expect_info_log="",
            .expect_error_log="",
            .expect_critical_log="",
        },
    };

    for (auto& tt : tests) {
        // replace bnx_log_t's file descriptor (for unit test)
        FILE *fp_debug = tmpfile(); int fd_debug = fileno(fp_debug); debug.fd = fd_debug;
        FILE *fp_info = tmpfile(); int fd_info = fileno(fp_info); info.fd = fd_info;
        FILE *fp_error = tmpfile(); int fd_error = fileno(fp_error); error.fd = fd_error;
        FILE *fp_critical = tmpfile(); int fd_critical = fileno(fp_critical); critical.fd = fd_critical;

        // execute
        bnx_return_t result = bnx_write_logs(head, tt.input_log_level, 16, "hello %s %s", "world", "!");
        CHECK_EQUAL_C_INT(BNX_OK, result.code);

        // assertion
        rewind(fp_debug);
        rewind(fp_info);
        rewind(fp_error);
        rewind(fp_critical);
        char read_debug[16] = {0};
        char read_info[16] = {0};
        char read_error[16] = {0};
        char read_critical[16] = {0};
        fread(read_debug, sizeof(char), 16, fp_debug);
        fread(read_info, sizeof(char), 16, fp_info);
        fread(read_error, sizeof(char), 16, fp_error);
        fread(read_critical, sizeof(char), 16, fp_critical);

        std::string label;

        label = tt.label + ": test debug log output";
        STRCMP_EQUAL_TEXT(tt.expect_debug_log, read_debug, label.c_str());
        label = tt.label + ": test info loga output";
        STRCMP_EQUAL_TEXT(tt.expect_info_log, read_info, label.c_str());
        label = tt.label + ": test error log output";
        STRCMP_EQUAL_TEXT(tt.expect_error_log, read_error, label.c_str());
        label = tt.label + ": test critical log output";
        STRCMP_EQUAL_TEXT(tt.expect_critical_log, read_critical, label.c_str());

        // free
        fclose(fp_debug);
        fclose(fp_info);
        fclose(fp_error);
        fclose(fp_critical);
    }
}

TEST(BnxWriteLogs, Boudary_NullableArgument)
{
    // case when NULL is given to bnx_log_t *log
    {
        bnx_return_t result = bnx_write_logs(NULL, BNX_LOG_LEVEL_INFO, 16, "foo %s", "bar");
        CHECK_EQUAL(BNX_ERROR, result.code);
    }

    // case when NULL is given to char *fmt
    {
        bnx_log_t log;
        memset(&log, 0, sizeof(log));
        bnx_string_t fpath = bnx_str_literal("foo");
        bnx_init_log(&log, BNX_LOG_LEVEL_INFO, &fpath, mock);

        bnx_return_t result;
        result = bnx_write_logs(&log, BNX_LOG_LEVEL_ERROR, 16, NULL, "world");
        CHECK_EQUAL(BNX_ERROR, result.code);
        result = bnx_write_logs(&log, BNX_LOG_LEVEL_ERROR, 16, NULL, NULL);
        CHECK_EQUAL(BNX_ERROR, result.code);
        result = bnx_write_logs(&log, BNX_LOG_LEVEL_ERROR, 16, "hello %s", NULL);
        CHECK_EQUAL(BNX_OK, result.code);
    }
}