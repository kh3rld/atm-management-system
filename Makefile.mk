# Compiler and compilation flags
CC = cc
CFLAGS = -Wall -g -Isrc  # -Wall enables common warnings, -g adds debug info
                         # -Isrc tells the compiler to look in src/ for headers like "header.h"
LDFLAGS =                # Linker flags (if any)

# List all object files needed for the atm executable.
# Assuming their corresponding .c files are in the src/ directory.
OBJECTS = src/main.o src/system.o src/auth.o src/kbd.o src/command.o \
          src/display.o src/insert.o src/search.o src/files.o src/utils.o

# The common header file (adjust if necessary)
HEADER = src/header.h

# The default target to build when 'make' is run without arguments
all: atm

atm : $(OBJECTS)
	$(CC) $(LDFLAGS) -o atm $(OBJECTS)

# Pattern rule to compile any .c file in src/ to a .o file in src/
# It correctly sets dependencies on the .c file and the common HEADER.
src/%.o: src/%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f atm $(OBJECTS)

# Declare targets that are not actual files
.PHONY: all clean
