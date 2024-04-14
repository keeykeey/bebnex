CC=gcc
DEST=/usr/local/bin

# production
PROGRAM=bebnex
OBJS=$(wildcard *.o)
SRCS=$(wildcard *.c)
SRCS_CORE=$(wildcard src/core/*.c)

# test
TEST_PROGRAM=tebnex
TEST_OBJS=$(wildcard *.o)
TEST_SRCS=$(wildcard test/*.c)
TEST_CORE=$(wildcard test/core/test_bnx_*.c)
TEST_SRCS_CORE=$(wildcard src/core/bnx_*.c)

# production part
$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)

obj: $(SRCS) $(SRCS_CORE)
	$(CC) -c $(SRCS) $(SRCS_CORE)

install: $(PROGRAM)
	install -s $(PROGRAM) $(DEST)

clean:
	rm -f $(OBJS) $(PROGRAM) $(TEST_PROGRAM)

# test part
$(TEST_PROGRAM): $(TEST_OBJS)
	$(CC) -o $(TEST_PROGRAM) $(TEST_OBJS)

tobj: $(TEST_SRCS) $(TEST_CORE) $(TEST_SRCS_CORE)
	$(CC) -c $(TEST_SRCS) $(TEST_CORE) $(TEST_SRCS_CORE)

.PHONY: clean install test