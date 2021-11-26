BIN				:=	bin
SRC				:=	src

COMMON_SRC		:=	$(wildcard $(SRC)/common/*.c)
COMMON_OBJ		:=	$(COMMON_SRC:.c=.o)

LOCK			:=	$(BIN)/test
LOCK_SRC		:=	$(wildcard $(SRC)/part2/*.c)
LOCK_OBJ		:=	$(LOCK_SRC:.c=.o)

CC				:=	gcc
CFLAGS			:=	-pedantic -Wvla -Wall -Werror
LDFLAGS			:=	-pthread

PHONY			:=	all clean $(LOCK)

all: $(LOCK)

$(LOCK): $(LOCK_OBJ) $(COMMON_OBJ) | $(BIN)
	$(CC) -o $@ $(LOCK_OBJ) $(COMMON_OBJ)

$(BIN):
	mkdir $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@$(RM) -rv $(BIN)
	@find . -type f -name '*.o' -exec rm -v {} \;

.PHONY = $(PHONY)