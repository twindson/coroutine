#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <string>

using namespace std;

// 演示不同的内存使用场景
// Demonstrate different memory usage scenarios

const size_t MB = 1024 * 1024;
const size_t MEMORY_SIZE = 100 * MB; // 100 MB

void print_info(const string& scenario) {
    cout << "========================================" << endl;
    cout << "Scenario: " << scenario << endl;
    cout << "Process ID: " << getpid() << endl;
    cout << "Allocated Memory: " << (MEMORY_SIZE / MB) << " MB" << endl;
    cout << "========================================" << endl;
}

void scenario_unused() {
    // 场景1: 分配内存但不使用
    // Scenario 1: Allocate memory but don't use it
    print_info("UNUSED - Allocated but not accessed");
    
    // 分配内存但不访问任何字节
    // Allocate memory but don't access any bytes
    char* memory = (char*)malloc(MEMORY_SIZE);
    if (!memory) {
        cerr << "Memory allocation failed!" << endl;
        return;
    }
    
    cout << "Memory allocated at: " << (void*)memory << endl;
    cout << "Status: Memory allocated but NOT used" << endl;
    cout << "Expected dump size: ~0 MB (only metadata)" << endl;
    cout << endl;
    cout << "Waiting 30 seconds... (use CRIU to checkpoint now)" << endl;
    cout << "Run: sudo criu dump -t " << getpid() << " -D ./checkpoint-unused --shell-job" << endl;
    
    sleep(30);
    
    free(memory);
}

void scenario_partial() {
    // 场景2: 分配内存并部分使用
    // Scenario 2: Allocate memory and use partially
    print_info("PARTIAL - Allocated and partially used");
    
    char* memory = (char*)malloc(MEMORY_SIZE);
    if (!memory) {
        cerr << "Memory allocation failed!" << endl;
        return;
    }
    
    // 只使用前10MB
    // Only use the first 10MB
    size_t used_size = 10 * MB;
    memset(memory, 'A', used_size);
    
    cout << "Memory allocated at: " << (void*)memory << endl;
    cout << "Status: " << (used_size / MB) << " MB used out of " << (MEMORY_SIZE / MB) << " MB" << endl;
    cout << "Expected dump size: ~" << (used_size / MB) << " MB" << endl;
    cout << endl;
    cout << "Waiting 30 seconds... (use CRIU to checkpoint now)" << endl;
    cout << "Run: sudo criu dump -t " << getpid() << " -D ./checkpoint-partial --shell-job" << endl;
    
    sleep(30);
    
    free(memory);
}

void scenario_full() {
    // 场景3: 分配内存并完全使用
    // Scenario 3: Allocate memory and use fully
    print_info("FULL - Allocated and fully used");
    
    char* memory = (char*)malloc(MEMORY_SIZE);
    if (!memory) {
        cerr << "Memory allocation failed!" << endl;
        return;
    }
    
    // 使用全部内存
    // Use all memory
    memset(memory, 'B', MEMORY_SIZE);
    
    cout << "Memory allocated at: " << (void*)memory << endl;
    cout << "Status: All " << (MEMORY_SIZE / MB) << " MB used" << endl;
    cout << "Expected dump size: ~" << (MEMORY_SIZE / MB) << " MB" << endl;
    cout << endl;
    cout << "Waiting 30 seconds... (use CRIU to checkpoint now)" << endl;
    cout << "Run: sudo criu dump -t " << getpid() << " -D ./checkpoint-full --shell-job" << endl;
    
    sleep(30);
    
    free(memory);
}

void scenario_progressive() {
    // 场景4: 逐步使用内存
    // Scenario 4: Progressive memory usage
    print_info("PROGRESSIVE - Gradual memory usage");
    
    char* memory = (char*)malloc(MEMORY_SIZE);
    if (!memory) {
        cerr << "Memory allocation failed!" << endl;
        return;
    }
    
    cout << "Memory allocated at: " << (void*)memory << endl;
    cout << endl;
    
    // 每10秒使用10MB
    // Use 10MB every 10 seconds
    for (int i = 1; i <= 10; i++) {
        size_t offset = (i - 1) * 10 * MB;
        memset(memory + offset, 'C', 10 * MB);
        
        cout << "Step " << i << ": Used " << (i * 10) << " MB total" << endl;
        cout << "Expected dump size: ~" << (i * 10) << " MB" << endl;
        cout << "Run: sudo criu dump -t " << getpid() << " -D ./checkpoint-step" << i << " --shell-job" << endl;
        cout << endl;
        
        sleep(10);
    }
    
    free(memory);
}

void print_usage() {
    cout << "CRIU Memory Dump Demonstration" << endl;
    cout << "==============================" << endl;
    cout << endl;
    cout << "Usage: ./criu_memory_demo [scenario]" << endl;
    cout << endl;
    cout << "Scenarios:" << endl;
    cout << "  unused      - Allocate memory but don't use it (dump size ~0 MB)" << endl;
    cout << "  partial     - Allocate and use 10% of memory (dump size ~10 MB)" << endl;
    cout << "  full        - Allocate and use all memory (dump size ~100 MB)" << endl;
    cout << "  progressive - Gradually use memory over time" << endl;
    cout << "  all         - Run all scenarios sequentially" << endl;
    cout << endl;
    cout << "Example:" << endl;
    cout << "  ./criu_memory_demo partial" << endl;
    cout << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }
    
    string scenario = argv[1];
    
    if (scenario == "unused") {
        scenario_unused();
    } else if (scenario == "partial") {
        scenario_partial();
    } else if (scenario == "full") {
        scenario_full();
    } else if (scenario == "progressive") {
        scenario_progressive();
    } else if (scenario == "all") {
        cout << "Running all scenarios sequentially..." << endl;
        cout << endl;
        scenario_unused();
        sleep(2);
        scenario_partial();
        sleep(2);
        scenario_full();
    } else {
        cout << "Unknown scenario: " << scenario << endl;
        cout << endl;
        print_usage();
        return 1;
    }
    
    cout << endl;
    cout << "Demo completed!" << endl;
    
    return 0;
}
