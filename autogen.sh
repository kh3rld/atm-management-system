#!/bin/sh

# Create necessary directories
mkdir -p m4

# Generate aclocal.m4
aclocal

# Generate configure.ac from configure.in if it exists
if [ -f configure.in ]; then
    mv configure.in configure.ac
fi

# Generate Makefile.in
automake --add-missing --copy

# Generate configure script
autoreconf --install --force

# Make the script executable
chmod +x configure 