CC=gcc
DEST=./bin #/usr/local/bin
OBJS=$(wildcard *.o)

SRCS=$(wildcard *.c)
SRCS_CORE=$(wildcard src/core/*.c)
SRCS_EVENT=$(wildcard src/event/*.c)

TEST_SRCS=$(wildcard test/*.c)
TEST_SRCS_CORE=$(wildcard src/core/bnx_*.c)
TEST_SRCS_EVENT=$(wildcard src/event/bnx_*.c)
#TEST_OBJS=$(TEST_SRCS:.c=.o)
TEST_OBJS=$(wildcard *.o)

PROGRAM=bebnex
TEST_PROGRAM=test_bebnex

# src
$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)

objs: $(SRCS) $(SRCS_CORE) $(SRCS_EVENT)
	$(CC) -c $(SRCS) $(SRCS_CORE) $(SRCS_EVENT)

install: $(PROGRAM)
	install -s $(PROGRAM) $(DEST)

clean:
	rm -f $(OBJS) $(PROGRAM) $(TEST_PROGRAM)

# test
$(TEST_PROGRAM): $(TEST_OBJS)
	$(CC) -o $(TEST) $(TEST_OBJS)

testObjs: $(TEST_SRCS) $(TEST_SRCS_CORE) $(TEST_SRCS_EVENT)
	$(CC) -c $(TEST_SRCS) $(TEST_SRCS_CORE) $(TEST_SRCS_EVENT)

.PHONY: clean install test