# Specify the programs to build
bin_PROGRAMS = atm

# Source files for the atm program
atm_SOURCES = src/main.c src/system.c src/auth.c

# Header files
include_HEADERS = src/header.h

# Include directories
AM_CPPFLAGS = -I$(srcdir)/src

# Create data directory during installation
install-data-hook:
	mkdir -p $(DESTDIR)$(datadir)/atm/data
	touch $(DESTDIR)$(datadir)/atm/data/users.txt
	touch $(DESTDIR)$(datadir)/atm/data/records.txt

# Clean up generated files
CLEANFILES = *~ *.o atm

# Required by automake
AUTOMAKE_OPTIONS = foreign 