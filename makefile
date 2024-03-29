PROGRAM_NAME= assembler
CC 			= gcc
CFLAGS 		= -ansi -pedantic -Wall -g
GLOBALS_FILE = dataStructuresAndConstants.h
EXE_DEPS = assembler.o dataConversion.o error.o fileFunctions.o firstPass.o secondPass.o initializationFunctions.o listFunctions.o preAsmblr.o syntaxAnalyzer.o

$(PROGRAM_NAME): $(EXE_DEPS)
	$(CC) $(CFLAGS)  $^ -o $@

%.o: %.c %.h $(GLOBALS_FILE)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o
