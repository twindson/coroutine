# Makefile for CRIU Memory Dump Demonstration

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = criu_memory_demo
SOURCE = criu_memory_demo.cpp

.PHONY: all clean test help

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)
	@echo "Build complete! Run './$(TARGET)' to see usage."

clean:
	rm -f $(TARGET)
	rm -rf checkpoint-*
	@echo "Cleaned build artifacts and checkpoint directories."

test: $(TARGET)
	@echo "Running CRIU memory demo test..."
	@echo "Note: CRIU requires root privileges for actual checkpointing."
	@echo ""
	@./$(TARGET) partial

help:
	@echo "CRIU Memory Dump Demonstration - Makefile"
	@echo "=========================================="
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build the demonstration program"
	@echo "  make clean    - Remove build artifacts and checkpoints"
	@echo "  make test     - Run a basic test (without CRIU)"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  1. Build:    make"
	@echo "  2. Run:      ./criu_memory_demo [scenario]"
	@echo "  3. Clean:    make clean"
	@echo ""
	@echo "Scenarios: unused, partial, full, progressive, all"
	@echo ""
