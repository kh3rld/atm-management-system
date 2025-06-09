# Makefile for ATM Management System

# Variables for compiler and flags
CC ?= cc
CFLAGS ?= -Wall -g -O2 # Common flags: All warnings, debug symbols, optimization level 2
CPPFLAGS ?= -Isrc      # Preprocessor flags, e.g., -I for include paths like "src/"
LDFLAGS ?=             # Linker flags
RM = rm -f             # Command for removing files

# Source directory
SRC_DIR = src

# Header file(s) - assuming all .c files might depend on header.h.
# For more complex projects, consider auto-dependency generation (e.g., with gcc -MMD).
HEADERS = $(SRC_DIR)/header.h

# List of source files.
# You could use a wildcard if all .c files in src/ should be compiled:
# SOURCES = $(wildcard $(SRC_DIR)/*.c)
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/system.c \
          $(SRC_DIR)/auth.c \
          $(SRC_DIR)/kbd.c \
          $(SRC_DIR)/command.c \
          $(SRC_DIR)/display.c \
          $(SRC_DIR)/insert.c \
          $(SRC_DIR)/search.c \
          $(SRC_DIR)/files.c \
          $(SRC_DIR)/utils.c

# Object files (derived from SOURCES, .o files will be in the same directory as .c files)
OBJECTS = $(SOURCES:.c=.o)

# Target executable name
TARGET = atm

# Installation directories (GNU Coding Standards)
# Users can override these: e.g., `make install prefix=/usr`
prefix ?= /usr/local
exec_prefix ?= $(prefix)
bindir ?= $(exec_prefix)/bin
datarootdir ?= $(prefix)/share
datadir ?= $(datarootdir)
includedir ?= $(prefix)/include

# Application-specific installation paths, respecting DESTDIR for staging
APP_BINDIR = $(DESTDIR)$(bindir)
APP_DATADIR = $(DESTDIR)$(datadir)/atm/data
APP_INCLUDEDIR = $(DESTDIR)$(includedir)/atm

# Local data files in the source tree to be removed by 'make clean'
LOCAL_DATA_FILES_TO_CLEAN = ./share/atm/data/users.txt ./share/atm/data/records.txt

# Default target: build the application
all: $(TARGET)

# Rule to link the target executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)
	@echo "$(TARGET) built successfully."

# Pattern rule to compile .c files from SRC_DIR to .o files in SRC_DIR
# Recompiles if the .c file or any of the $(HEADERS) change.
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@echo "Compiling $< -> $@"
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build artifacts..."
	$(RM) $(TARGET) $(OBJECTS)
	@echo "Cleaning local data files..."
	$(RM) ./share/atm/data/users.txt
	$(RM) ./share/atm/data/records.txt
	-rmdir ./share/atm/data 2>/dev/null || true
	-rmdir ./share/atm 2>/dev/null || true
	@echo "Clean complete."

uninstall:
	@echo "Uninstalling ATM Management System from prefix $(prefix)..."
	$(RM) $(APP_BINDIR)/$(TARGET)
	$(RM) $(APP_INCLUDEDIR)/header.h
	$(RM) $(APP_DATADIR)/users.txt
	$(RM) $(APP_DATADIR)/records.txt
	@echo "Removing data directory $(APP_DATADIR)..."
	$(RM) -r $(APP_DATADIR)
	@echo "Attempting to remove empty application directories..."
	-rmdir $(APP_INCLUDEDIR) 2>/dev/null || true
	-rmdir $(DESTDIR)$(datadir)/atm 2>/dev/null || true
	-rmdir $(DESTDIR)$(datadir)/atm/data 2>/dev/null || true
	@echo "Uninstallation complete."

install: all
	@echo "Installing ATM Management System..."
	@mkdir -p $(APP_BINDIR)
	@mkdir -p $(APP_INCLUDEDIR)
	@mkdir -p $(APP_DATADIR)
	@echo "Copying executable $(TARGET) to $(APP_BINDIR)/"
	@cp $(TARGET) $(APP_BINDIR)/
	@echo "Copying header $(SRC_DIR)/header.h to $(APP_INCLUDEDIR)/"
	@cp $(SRC_DIR)/header.h $(APP_INCLUDEDIR)/
	@echo "Installation complete."
	@echo "  Executable installed to: $(bindir)/$(TARGET)"
	@echo "  Header file installed to: $(includedir)/atm/header.h"
	@echo "  Data directory created at: $(datadir)/atm/data/"

.PHONY: all clean install uninstall