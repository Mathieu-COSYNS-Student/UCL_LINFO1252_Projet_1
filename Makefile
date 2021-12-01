# UTILS
not-containing = $(foreach v,$2,$(if $(findstring $1,$v),,$v))

# GOBAL
BIN							:=	bin
SRC							:=	src

# Common
COMMON_SRC					:=	$(wildcard $(SRC)/common/*.c)
COMMON_OBJ					:=	$(COMMON_SRC:.c=.o)

# Tache 1.1
PHILOSOPHES					:= $(BIN)/philosophes_1
PHILOSOPHES_MAIN			:= $(SRC)/part1/tache1_1/philosophes.c

# Tache 1.2
PRODUCTEURS_CONSOMMATEURS	:= $(BIN)/producteurs_consommateurs_1
PRODUCTEURS_CONSOMMATEURS_MAIN	:= $(SRC)/part1/tache1_2/producteurs_consommateurs.c

# Tache 1.3
READERS_WRITERS				:=	$(BIN)/readers_writers_1
READERS_WRITERS_MAIN		:=	$(SRC)/part1/tache1_3/readers_writers.c

# Tache 2.1 + 2.2
LOCK_SIMPLE_TEST			:=	$(BIN)/lock_simple_test_1
LOCK_SIMPLE_TEST_MAIN		:=	$(SRC)/part2/tache2_2/lock_simple_test.c
LOCK_OBJ					:=	$(patsubst %.c,%.o,$(wildcard $(SRC)/part2/tache2_1/*.c))

# Tache 2.3 + 2.2
LOCK_2_SIMPLE_TEST			:=	$(BIN)/lock_simple_test_2
LOCK_2_SIMPLE_TEST_MAIN		:=	$(SRC)/part2/tache2_2/lock_simple_test.c
LOCK_2_OBJ					:=	$(patsubst %.c,%.o,$(wildcard $(SRC)/part2/tache2_3/*.c))

# Tache 2.4
SEMAPHORE_SIMPLE_TEST		:=	$(BIN)/semaphore_simple_test
SEMAPHORE_SIMPLE_TEST_MAIN	:=	$(SRC)/part2/tache2_4/semaphore_simple_test.c
SEMAPHORE_OBJ				:=	$(patsubst %.c,%.o,$(call not-containing,test,$(wildcard $(SRC)/part2/tache2_4/*.c))) $(LOCK_OBJ)
SEMAPHORE_2_OBJ				:=	$(patsubst %.c,%.o,$(call not-containing,test,$(wildcard $(SRC)/part2/tache2_4/*.c))) $(LOCK_2_OBJ)

# Tache 2.5
#	- Adaptation de la tache 1.1
PHILOSOPHES_2				:= $(BIN)/philosophes_2
PHILOSOPHES_3				:= $(BIN)/philosophes_3
PHILOSOPHES_UPDATED_MAIN	:= $(SRC)/part2/tache1_1/philosophes.c

#	- Adaptation de la tache 1.2
PRODUCTEURS_CONSOMMATEURS_2	:= $(BIN)/producteurs_consommateurs_2
PRODUCTEURS_CONSOMMATEURS_3	:= $(BIN)/producteurs_consommateurs_3
PRODUCTEURS_CONSOMMATEURS_UPDATED_MAIN	:= $(SRC)/part2/tache1_2/producteurs_consommateurs.c

#	- Adaptation de la tache 1.3
READERS_WRITERS_2			:=	$(BIN)/readers_writers_2
READERS_WRITERS_3			:=	$(BIN)/readers_writers_3
READERS_WRITERS_UPDATED_MAIN:=	$(SRC)/part2/tache1_3/readers_writers.c

CC							:=	gcc
CFLAGS						:=	-pedantic -Wvla -Wall -Werror
LDFLAGS						:=	-pthread

PHONY						:=	all clean benchmark benchmark-post $(BIN)

all: $(PHILOSOPHES) $(PRODUCTEURS_CONSOMMATEURS) $(READERS_WRITERS) $(LOCK_SIMPLE_TEST) $(LOCK_2_SIMPLE_TEST) $(SEMAPHORE_SIMPLE_TEST) $(PHILOSOPHES_2) $(PRODUCTEURS_CONSOMMATEURS_2) $(READERS_WRITERS_2) $(PHILOSOPHES_3) $(PRODUCTEURS_CONSOMMATEURS_3) $(READERS_WRITERS_3)

$(PHILOSOPHES): $(PHILOSOPHES_MAIN) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(PHILOSOPHES_MAIN) $(COMMON_OBJ)

$(PRODUCTEURS_CONSOMMATEURS): $(PRODUCTEURS_CONSOMMATEURS_MAIN) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(PRODUCTEURS_CONSOMMATEURS_MAIN) $(COMMON_OBJ)

$(READERS_WRITERS): $(READERS_WRITERS_MAIN) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(READERS_WRITERS_MAIN) $(COMMON_OBJ)

$(LOCK_SIMPLE_TEST): $(LOCK_SIMPLE_TEST_MAIN) $(LOCK_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LOCK_SIMPLE_TEST_MAIN) $(LOCK_OBJ) $(COMMON_OBJ)
	
$(LOCK_2_SIMPLE_TEST): $(LOCK_2_SIMPLE_TEST_MAIN) $(LOCK_2_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LOCK_SIMPLE_TEST_MAIN) $(LOCK_2_OBJ) $(COMMON_OBJ)

$(SEMAPHORE_SIMPLE_TEST): $(SEMAPHORE_SIMPLE_TEST_MAIN) $(SEMAPHORE_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) -o $@ $(SEMAPHORE_SIMPLE_TEST_MAIN) $(SEMAPHORE_OBJ) $(COMMON_OBJ)

$(PHILOSOPHES_2): $(PHILOSOPHES_UPDATED_MAIN) $(LOCK_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(PHILOSOPHES_UPDATED_MAIN) $(COMMON_OBJ) $(LOCK_OBJ)

$(PRODUCTEURS_CONSOMMATEURS_2): $(PRODUCTEURS_CONSOMMATEURS_UPDATED_MAIN) $(SEMAPHORE_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(PRODUCTEURS_CONSOMMATEURS_UPDATED_MAIN) $(COMMON_OBJ) $(SEMAPHORE_OBJ)

$(READERS_WRITERS_2): $(READERS_WRITERS_UPDATED_MAIN) $(SEMAPHORE_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(READERS_WRITERS_UPDATED_MAIN) $(SEMAPHORE_OBJ) $(COMMON_OBJ)

$(PHILOSOPHES_3): $(PHILOSOPHES_UPDATED_MAIN) $(LOCK_2_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(PHILOSOPHES_UPDATED_MAIN) $(COMMON_OBJ) $(LOCK_2_OBJ)

$(PRODUCTEURS_CONSOMMATEURS_3): $(PRODUCTEURS_CONSOMMATEURS_UPDATED_MAIN) $(SEMAPHORE_2_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(PRODUCTEURS_CONSOMMATEURS_UPDATED_MAIN) $(COMMON_OBJ) $(SEMAPHORE_2_OBJ)

$(READERS_WRITERS_3): $(READERS_WRITERS_UPDATED_MAIN) $(SEMAPHORE_2_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(READERS_WRITERS_UPDATED_MAIN) $(SEMAPHORE_2_OBJ) $(COMMON_OBJ)

$(BIN):
	mkdir $@

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

benchmark: FRC
	@./benchmark/run-all.sh

benchmark-post:
	@./benchmark/run-all_post_processor.sh

FRC:

clean:
	@$(RM) -rv $(BIN)
	@find . -path ./venv -prune -o -type f -name '*.o' -exec rm -v {} \;
	@./benchmark/clean.sh

.PHONY = $(PHONY)