#!/bin/bash
# Build script for hg-watchman integration
# This script is triggered by watchman when source files change

set -e

echo "Starting build process..."
echo "Timestamp: $(date)"

# Example build for C++ project
if [ -f "test.cpp" ]; then
    echo "Building C++ project..."
    g++ -o test test.cpp
    echo "Build completed successfully"
fi

# Add your build commands here
# For example:
# make clean && make
# npm run build
# cargo build
# etc.

echo "Build process finished at $(date)"