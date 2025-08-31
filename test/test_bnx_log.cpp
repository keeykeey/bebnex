#include <CppUTest/CommandLineTestRunner.h>
#include <cstdio>
#include <unistd.h>
#include "core/bnx_log.h"

TEST_GROUP(BnxLog) {
    int saved_stdout = -1;
    int saved_stderr = -1;
    FILE *caplog = nullptr;

    void setup() override
    {
        // save original fd of stdout
        saved_stdout = dup(fileno(stdout));
        CHECK_TEXT(saved_stdout != -1, "dup() failed");

        // save original fd of stderr
        saved_stderr = dup(fileno(stderr));
        CHECK_TEXT(saved_stderr != -1, "dum() failed");

        // create temporary FILE*
        caplog = tmpfile();
        CHECK_TEXT(caplog != nullptr, "tmpfile() failed");
        int caplog_fd = fileno(caplog);

        // swap stdout and temporary FILE
        int result = dup2(caplog_fd, fileno(stdout));
        CHECK_TEXT(result != -1, "dup2() failed");

        // swap stderr and temporary FILE
        result = dup2(caplog_fd, fileno(stderr));
        CHECK_TEXT(result != -1, "dup2() failed");
    }

    void teardown() override
    {
        if (caplog) {
            fflush(caplog);
        }

        if (saved_stdout != -1) {
            dup2(saved_stdout, fileno(stdout));
            close(saved_stdout);
            saved_stdout = -1;
        }

        if (saved_stderr != -1) {
            dup2(saved_stderr, fileno(stderr));
            close(saved_stderr);
            saved_stderr = -1;
        }

        if (caplog) {
            fclose(caplog);
            caplog = nullptr;
        }

    }

    void clearCaplog()
    {
        int caplog_fd = fileno(caplog);
        ftruncate(caplog_fd, 0);
        fseek(caplog, 0, SEEK_SET);
    }
};

TEST(BnxLog, info)
{
#if BNX_LOG_LEVEL >= BNX_LOG_LEVEL_INFO
    clearCaplog();

    BNX_LOG_INFO("hello %s", "world");

    fflush(stdout);
    fseek(caplog, 0, SEEK_SET);

    char buf[256] = {0};

    fgets(buf, sizeof(buf), caplog);

    STRCMP_EQUAL("[INFO] hello world\n", buf);
#else
#endif
}

TEST(BnxLog, error)
{
#if BNX_LOG_LEVEL >= BNX_LOG_LEVEL_ERROR
    clearCaplog();

    BNX_LOG_ERROR("hello %s", "world");

    fflush(stderr);
    fseek(caplog, 0, SEEK_SET);

    char buf[256] = {0};

    fgets(buf, sizeof(buf), caplog);

    STRCMP_EQUAL("[ERROR] hello world\n", buf);
#else
#endif
}

TEST(BnxLog, fatal)
{
#if BNX_LOG_LEVEL >= BNX_LOG_LEVEL_FATAL
    clearCaplog();

    BNX_LOG_FATAL("hello %s", "world");

    fflush(stderr);
    fseek(caplog, 0, SEEK_SET);

    char buf[256] = {0};
    fgets(buf, sizeof(buf), caplog);

    STRCMP_EQUAL("[FATAL] hello world\n", buf);
#else
#endif
}

TEST(BnxLog, debug)
{
#if BNX_LOG_LEVEL >= BNX_LOG_LEVEL_DEBUG
    clearCaplog();

    BNX_LOG_FATAL("hello %s", "world");

    fflush(stdout);
    fseek(caplog, 0, SEEK_SET);

    char buf[256] = {0};
    fgets(buf, sizeof(buf), caplog);

    STRCMP_EQUAL("[DEBUG] hello world\n", buf);
#else
#endif
}
