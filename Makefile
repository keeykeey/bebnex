CC=gcc
DEST=/usr/local/bin

# production
PROGRAM=bebnex
OBJS=$(wildcard *.o)
SRCS=$(wildcard *.c)
SRCS_CORE=$(wildcard src/core/*.c)
SRCS_EVENT=$(wildcard src/event/*.c)

# test
TEST_PROGRAM=test_bebnex
TEST_OBJS=$(wildcard *.o)
TEST_SRCS=$(wildcard test/*.c)
TEST_CORE=$(wildcard test/core/test_bnx_*.c)
TEST_EVENT=$(wildcard test/event/test_bnx_*.c)
TEST_SRCS_CORE=$(wildcard src/core/bnx_*.c)
TEST_SRCS_EVENT=$(wildcard src/event/bnx_*.c)

# production part
$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)

objs: $(SRCS) $(SRCS_CORE) $(SRCS_EVENT)
	$(CC) -c $(SRCS) $(SRCS_CORE) $(SRCS_EVENT)

install: $(PROGRAM)
	install -s $(PROGRAM) $(DEST)

clean:
	rm -f $(OBJS) $(PROGRAM) $(TEST_PROGRAM)

# test part
$(TEST_PROGRAM): $(TEST_OBJS)
	$(CC) -o $(TEST_PROGRAM) $(TEST_OBJS)

testObjs: $(TEST_SRCS) $(TEST_CORE) $(TEST_SRCS_CORE) $(TEST_SRCS_EVENT)
	$(CC) -c $(TEST_SRCS) $(TEST_CORE) $(TEST_EVENT) $(TEST_SRCS_CORE) $(TEST_SRCS_EVENT)

.PHONY: clean install test