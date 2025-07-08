#!/bin/bash
# Test runner script for hg-watchman integration
# This script is triggered by watchman when test files or source files change

set -e

echo "Starting test run..."
echo "Timestamp: $(date)"

# Example test execution
if [ -f "test" ]; then
    echo "Running compiled tests..."
    ./test
    echo "Tests completed successfully"
elif [ -f "test.cpp" ]; then
    echo "Compiling and running tests..."
    g++ -o test test.cpp && ./test
    echo "Tests completed successfully"
fi

# Add your test commands here
# For example:
# npm test
# python -m pytest
# cargo test
# make test
# etc.

echo "Test run finished at $(date)"