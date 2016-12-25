SRC =	main.c lua_config.c

BIN = testConfig
VERSION = v0.1

CPPFLAGS = -DVERSION=\"${VERSION}\" -DNDEBUG -UDEBUG
CFLAGS   = -std=gnu11 -Wall -Wextra -Wpedantic -Os
CFLAGS  += -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wformat=2
CFLAGS  += -Wmissing-prototypes -Wmissing-prototypes -Wredundant-decls
LDFLAGS  =
LDLIBS   = -llua

## Debugging flags
CPPFLAGS += -UNDEBUG -DDEBUG
CFLAGS += -Og -ggdb3

## Enable LLVM/Clang
CC = clang
CFLAGS += -Os -Weverything

$(BIN): $(SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

clean:
	rm -f $(BIN)

.PHONY: clean
