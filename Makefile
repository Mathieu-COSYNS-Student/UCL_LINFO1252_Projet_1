# UTILS
not-containing = $(foreach v,$2,$(if $(findstring $1,$v),,$v))

#
BIN						:=	bin
SRC						:=	src

COMMON_SRC				:=	$(wildcard $(SRC)/common/*.c)
COMMON_OBJ				:=	$(COMMON_SRC:.c=.o)

READERS_WRITERS			:=	$(BIN)/readers_writers
READERS_WRITERS_MAIN	:=	$(SRC)/part1/tache1_3/readers_writers.c

LOCK_SIMPLE_TEST		:=	$(BIN)/lock_simple_test
LOCK_SIMPLE_TEST_MAIN	:=	$(SRC)/part2/lock_simple_test.c
LOCK_OBJ				:=	$(patsubst %.c,%.o,$(wildcard $(SRC)/part2/tache2_3/*.c))

SEMAPHORE_SIMPLE_TEST	:=	$(BIN)/semaphore_simple_test
SEMAPHORE_SIMPLE_TEST_MAIN	:=	$(SRC)/part2/tache2_4/semaphore_simple_test.c
SEMAPHORE_OBJ			:=	$(patsubst %.c,%.o,$(call not-containing,test,$(wildcard $(SRC)/part2/tache2_4/*.c)))

CC						:=	gcc
CFLAGS					:=	-pedantic -Wvla -Wall -Werror
LDFLAGS					:=	-pthread

PHONY					:=	all clean $(BIN) $(LOCK)

all: $(READERS_WRITERS) $(LOCK_SIMPLE_TEST) $(SEMAPHORE_SIMPLE_TEST)

$(READERS_WRITERS): $(READERS_WRITERS_MAIN) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(READERS_WRITERS_MAIN) $(COMMON_OBJ)

$(LOCK_SIMPLE_TEST): $(LOCK_SIMPLE_TEST_MAIN) $(LOCK_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LOCK_SIMPLE_TEST_MAIN) $(LOCK_OBJ) $(COMMON_OBJ)

$(SEMAPHORE_SIMPLE_TEST): $(SEMAPHORE_SIMPLE_TEST_MAIN) $(SEMAPHORE_OBJ) $(LOCK_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) -o $@ $(SEMAPHORE_SIMPLE_TEST_MAIN) $(SEMAPHORE_OBJ) $(LOCK_OBJ) $(COMMON_OBJ)

$(BIN):
	mkdir $@

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

clean:
	@$(RM) -rv $(BIN)
	@find . -type f -name '*.o' -exec rm -v {} \;

.PHONY = $(PHONY)