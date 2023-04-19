# Define version and build date strings
VERSION    := 0.0.1
DATESTRING := $(shell date +%Y%m%d)

CC     := gcc
CFLAGS := -Wall -Wextra -O2 -g -Wno-unused-variable -pedantic -D__BUILD_DATE="\"$(DATESTRING)\"" -D__BUILD_VERSION="\"$(VERSION)\""
BIN    := check_sign
SRC    := src
OBJ    := build

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

ifeq ($(OS),Windows_NT)
	EXT=.exe
else
	EXT=
endif

all: $(BIN)$(EXT)

$(BIN)$(EXT): $(OBJECTS)
	$(CC) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -I$(SRC) -c $< -o $@

clean:
	@rm -rf ${OBJECTS}
	@rm -f $(BIN)$(EXT)

# Declare the phony targets
.PHONY: all clean
