
SRC = 2048.c
CFLAGS = -std=c99 -Wall -Wextra

BIN ?= c2048
PREFIX ?= /usr/local

$(BIN):
	gcc $(CFLAGS) -o $(BIN) $(SRC)

debug:
	gcc $(CFLAGS) -g -o $(BIN) $(SRC)

clean:
	rm -rf *.out *.o *.dSYM $(BIN)

install: $(BIN)
	cp -f $(BIN) $(PREFIX)/bin/

uninstall:
	rm -f $(PREFIX)/bin/$(BIN)

.PHONY: debug clean install uninstall
