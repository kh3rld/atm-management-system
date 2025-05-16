#!/bin/sh

# Exit on error
set -e

# Create necessary directories
mkdir -p m4

# Clean up any existing generated files
rm -f aclocal.m4 configure Makefile.in

# Generate aclocal.m4
aclocal

# Generate configure.ac from configure.in if it exists
if [ -f configure.in ]; then
    mv configure.in configure.ac
fi

# Generate Makefile.in
automake --add-missing --copy --foreign

# Generate configure script
autoreconf --install --force

# Make the script executable
chmod +x configure

echo "Build system files generated successfully." 