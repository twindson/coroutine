#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <string>

// 演示不同的内存使用场景
// Demonstrate different memory usage scenarios
// Note: Using malloc instead of new to demonstrate low-level memory behavior with CRIU

const size_t MB = 1024 * 1024;
const size_t MEMORY_SIZE = 100 * MB; // 100 MB
const int CHECKPOINT_WAIT_TIME = 30; // seconds

void print_info(const std::string& scenario) {
    std::cout << "========================================" << std::endl;
    std::cout << "Scenario: " << scenario << std::endl;
    std::cout << "Process ID: " << getpid() << std::endl;
    std::cout << "Allocated Memory: " << (MEMORY_SIZE / MB) << " MB" << std::endl;
    std::cout << "========================================" << std::endl;
}

void scenario_unused() {
    // 场景1: 分配内存但不使用
    // Scenario 1: Allocate memory but don't use it
    print_info("UNUSED - Allocated but not accessed");
    
    // 分配内存但不访问任何字节 (Using malloc for low-level demonstration)
    // Allocate memory but don't access any bytes
    char* memory = (char*)malloc(MEMORY_SIZE);
    if (!memory) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return;
    }
    
    std::cout << "Memory allocated at: " << (void*)memory << std::endl;
    std::cout << "Status: Memory allocated but NOT used" << std::endl;
    std::cout << "Expected dump size: ~0 MB (only metadata)" << std::endl;
    std::cout << std::endl;
    std::cout << "Waiting " << CHECKPOINT_WAIT_TIME << " seconds... (use CRIU to checkpoint now)" << std::endl;
    std::cout << "Run: sudo criu dump -t " << getpid() << " -D ./checkpoint-unused --shell-job" << std::endl;
    
    sleep(CHECKPOINT_WAIT_TIME);
    
    free(memory);
}

void scenario_partial() {
    // 场景2: 分配内存并部分使用
    // Scenario 2: Allocate memory and use partially
    print_info("PARTIAL - Allocated and partially used");
    
    char* memory = (char*)malloc(MEMORY_SIZE);
    if (!memory) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return;
    }
    
    // 只使用前10MB
    // Only use the first 10MB
    size_t used_size = 10 * MB;
    memset(memory, 'A', used_size);
    
    std::cout << "Memory allocated at: " << (void*)memory << std::endl;
    std::cout << "Status: " << (used_size / MB) << " MB used out of " << (MEMORY_SIZE / MB) << " MB" << std::endl;
    std::cout << "Expected dump size: ~" << (used_size / MB) << " MB" << std::endl;
    std::cout << std::endl;
    std::cout << "Waiting " << CHECKPOINT_WAIT_TIME << " seconds... (use CRIU to checkpoint now)" << std::endl;
    std::cout << "Run: sudo criu dump -t " << getpid() << " -D ./checkpoint-partial --shell-job" << std::endl;
    
    sleep(CHECKPOINT_WAIT_TIME);
    
    free(memory);
}

void scenario_full() {
    // 场景3: 分配内存并完全使用
    // Scenario 3: Allocate memory and use fully
    print_info("FULL - Allocated and fully used");
    
    char* memory = (char*)malloc(MEMORY_SIZE);
    if (!memory) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return;
    }
    
    // 使用全部内存
    // Use all memory
    memset(memory, 'B', MEMORY_SIZE);
    
    std::cout << "Memory allocated at: " << (void*)memory << std::endl;
    std::cout << "Status: All " << (MEMORY_SIZE / MB) << " MB used" << std::endl;
    std::cout << "Expected dump size: ~" << (MEMORY_SIZE / MB) << " MB" << std::endl;
    std::cout << std::endl;
    std::cout << "Waiting " << CHECKPOINT_WAIT_TIME << " seconds... (use CRIU to checkpoint now)" << std::endl;
    std::cout << "Run: sudo criu dump -t " << getpid() << " -D ./checkpoint-full --shell-job" << std::endl;
    
    sleep(CHECKPOINT_WAIT_TIME);
    
    free(memory);
}

void scenario_progressive() {
    // 场景4: 逐步使用内存
    // Scenario 4: Progressive memory usage
    print_info("PROGRESSIVE - Gradual memory usage");
    
    char* memory = (char*)malloc(MEMORY_SIZE);
    if (!memory) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return;
    }
    
    std::cout << "Memory allocated at: " << (void*)memory << std::endl;
    std::cout << std::endl;
    
    // 每10秒使用10MB
    // Use 10MB every 10 seconds
    for (int i = 1; i <= 10; i++) {
        size_t offset = (i - 1) * 10 * MB;
        memset(memory + offset, 'C', 10 * MB);
        
        std::cout << "Step " << i << ": Used " << (i * 10) << " MB total" << std::endl;
        std::cout << "Expected dump size: ~" << (i * 10) << " MB" << std::endl;
        std::cout << "Run: sudo criu dump -t " << getpid() << " -D ./checkpoint-step" << i << " --shell-job" << std::endl;
        std::cout << std::endl;
        
        sleep(10);
    }
    
    free(memory);
}

void print_usage() {
    std::cout << "CRIU Memory Dump Demonstration" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage: ./criu_memory_demo [scenario]" << std::endl;
    std::cout << std::endl;
    std::cout << "Scenarios:" << std::endl;
    std::cout << "  unused      - Allocate memory but don't use it (dump size ~0 MB)" << std::endl;
    std::cout << "  partial     - Allocate and use 10% of memory (dump size ~10 MB)" << std::endl;
    std::cout << "  full        - Allocate and use all memory (dump size ~100 MB)" << std::endl;
    std::cout << "  progressive - Gradually use memory over time" << std::endl;
    std::cout << "  all         - Run all scenarios sequentially" << std::endl;
    std::cout << std::endl;
    std::cout << "Example:" << std::endl;
    std::cout << "  ./criu_memory_demo partial" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }
    
    std::string scenario = argv[1];
    
    if (scenario == "unused") {
        scenario_unused();
    } else if (scenario == "partial") {
        scenario_partial();
    } else if (scenario == "full") {
        scenario_full();
    } else if (scenario == "progressive") {
        scenario_progressive();
    } else if (scenario == "all") {
        std::cout << "Running all scenarios sequentially..." << std::endl;
        std::cout << std::endl;
        scenario_unused();
        sleep(2);
        scenario_partial();
        sleep(2);
        scenario_full();
    } else {
        std::cout << "Unknown scenario: " << scenario << std::endl;
        std::cout << std::endl;
        print_usage();
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "Demo completed!" << std::endl;
    
    return 0;
}
