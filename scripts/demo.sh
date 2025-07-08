#!/bin/bash
# Demo script to showcase hg-watchman integration
# This script sets up a demonstration environment

set -e

echo "=== Hg-Watchman Integration Demo ==="
echo

# Check if required tools are installed
check_tool() {
    if ! command -v $1 &> /dev/null; then
        echo "Error: $1 is not installed. Please install it first."
        exit 1
    fi
}

echo "Checking prerequisites..."
check_tool hg
check_tool watchman

echo "✓ All prerequisites are available"
echo

# Create a demo directory
DEMO_DIR="/tmp/hg-watchman-demo"
echo "Creating demo environment in $DEMO_DIR"
rm -rf "$DEMO_DIR"
mkdir -p "$DEMO_DIR"
cd "$DEMO_DIR"

# Initialize Mercurial repository
echo "Initializing Mercurial repository..."
hg init
echo "✓ Mercurial repository initialized"

# Copy configuration files
echo "Setting up configuration files..."
cp /home/runner/work/coroutine/coroutine/.watchmanconfig .
cp /home/runner/work/coroutine/coroutine/.hgignore .
cp -r /home/runner/work/coroutine/coroutine/scripts .
echo "✓ Configuration files copied"

# Create sample files
echo "Creating sample source files..."
cat > main.cpp << 'EOF'
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "Hello from hg-watchman demo!" << std::endl;
    std::cout << "Current time: " << std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
    return 0;
}
EOF

cat > test.h << 'EOF'
#ifndef TEST_H
#define TEST_H

void run_tests();

#endif
EOF

echo "✓ Sample files created"

# Add files to Mercurial
echo "Adding files to Mercurial..."
hg add .
hg commit -m "Initial commit for hg-watchman demo"
echo "✓ Initial commit completed"

# Start watchman
echo "Starting watchman..."
watchman watch .
echo "✓ Watchman is now watching the directory"

# Set up triggers
echo "Setting up watchman triggers..."

# Build trigger
watchman -- trigger . demo-build \
  --append-files \
  --stdout=log \
  --stderr=log \
  '*.cpp' '*.h' \
  -- ./scripts/build.sh

# Test trigger
watchman -- trigger . demo-test \
  --append-files \
  --stdout=log \
  --stderr=log \
  '*.cpp' '*.h' \
  -- ./scripts/run-tests.sh

echo "✓ Triggers configured"

echo
echo "=== Demo Setup Complete! ==="
echo
echo "Demo directory: $DEMO_DIR"
echo
echo "Try the following:"
echo "1. cd $DEMO_DIR"
echo "2. Edit main.cpp or test.h"
echo "3. Watch as watchman automatically triggers build and test scripts"
echo "4. Check 'watchman trigger-list .' to see active triggers"
echo "5. Use 'watchman watch-del .' to stop watching when done"
echo
echo "Monitor logs with: tail -f ~/.watchman/log"