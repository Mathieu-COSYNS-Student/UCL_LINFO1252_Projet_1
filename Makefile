# UTILS
not-containing = $(foreach v,$2,$(if $(findstring $1,$v),,$v))

# GOBAL
BIN							:=	bin
SRC							:=	src

# Common
COMMON_SRC					:=	$(wildcard $(SRC)/common/*.c)
COMMON_OBJ					:=	$(COMMON_SRC:.c=.o)

# Tache 1.3
READERS_WRITERS				:=	$(BIN)/readers_writers
READERS_WRITERS_MAIN		:=	$(SRC)/part1/tache1_3/readers_writers.c

# Tache 2.1
LOCK_SIMPLE_TEST			:=	$(BIN)/lock_simple_test
LOCK_SIMPLE_TEST_MAIN		:=	$(SRC)/part2/lock_simple_test.c
LOCK_OBJ					:=	$(patsubst %.c,%.o,$(wildcard $(SRC)/part2/tache2_3/*.c))

# Tache 2.4
SEMAPHORE_SIMPLE_TEST		:=	$(BIN)/semaphore_simple_test
SEMAPHORE_SIMPLE_TEST_MAIN	:=	$(SRC)/part2/tache2_4/semaphore_simple_test.c
SEMAPHORE_OBJ				:=	$(patsubst %.c,%.o,$(call not-containing,test,$(wildcard $(SRC)/part2/tache2_4/*.c)))

# Tache 2.5
#	- Adaptation de la tache 1.3
READERS_WRITERS_2			:=	$(BIN)/readers_writers_2
READERS_WRITERS_2_MAIN		:=	$(SRC)/part2/tache1_3/readers_writers.c

CC							:=	gcc
CFLAGS						:=	-pedantic -Wvla -Wall -Werror
LDFLAGS						:=	-pthread

PHONY						:=	all clean $(BIN) $(LOCK)

all: $(READERS_WRITERS) $(LOCK_SIMPLE_TEST) $(SEMAPHORE_SIMPLE_TEST) $(READERS_WRITERS_2)

$(READERS_WRITERS): $(READERS_WRITERS_MAIN) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(READERS_WRITERS_MAIN) $(COMMON_OBJ)

$(LOCK_SIMPLE_TEST): $(LOCK_SIMPLE_TEST_MAIN) $(LOCK_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LOCK_SIMPLE_TEST_MAIN) $(LOCK_OBJ) $(COMMON_OBJ)

$(SEMAPHORE_SIMPLE_TEST): $(SEMAPHORE_SIMPLE_TEST_MAIN) $(SEMAPHORE_OBJ) $(LOCK_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) -o $@ $(SEMAPHORE_SIMPLE_TEST_MAIN) $(SEMAPHORE_OBJ) $(LOCK_OBJ) $(COMMON_OBJ)

$(READERS_WRITERS_2): $(READERS_WRITERS_2_MAIN) $(SEMAPHORE_OBJ) $(LOCK_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(READERS_WRITERS_2_MAIN) $(SEMAPHORE_OBJ) $(LOCK_OBJ) $(COMMON_OBJ)

$(BIN):
	mkdir $@

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

clean:
	@$(RM) -rv $(BIN)
	@find . -type f -name '*.o' -exec rm -v {} \;

.PHONY = $(PHONY)