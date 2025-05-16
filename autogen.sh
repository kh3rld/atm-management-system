#!/bin/sh

# Generate configure script
autoreconf --install --force

# Make the script executable
chmod +x configure 