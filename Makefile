PROGRAM_NAME = assembler
CC          = gcc
CPPFLAGS    = -Iinclude -MMD -MP
CFLAGS      = -ansi -pedantic -Wall -g
SRC_DIR     = src
OBJ_DIR     = obj
BIN_DIR     = .
GLOBALS_FILE = dataStructuresAndConstants.h

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEP = $(OBJ:.o=.d)

all: $(BIN_DIR)/$(PROGRAM_NAME)

$(BIN_DIR)/$(PROGRAM_NAME): $(OBJ)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rv $(OBJ_DIR)

-include $(DEP)