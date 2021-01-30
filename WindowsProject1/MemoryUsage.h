#pragma once
#include<Windows.h>
#include<iostream>
#include"Log.h"


struct MemoryUsage {
    uint32_t totalAllocated = 0;
    uint32_t totalFreed = 0;

    uint32_t getCurrentUsage() { return totalAllocated - totalFreed; }
};


namespace {
    MemoryUsage memUsage;
}


void* operator new(size_t size) {
    ::memUsage.totalAllocated += size;
    return malloc(size);
}

void operator delete(void* memory, size_t size) {
    ::memUsage.totalFreed += size;
    free(memory);
}


void printMemoryUsageConsole() {
    std::cout << "Total Memory Usage: " << ::memUsage.getCurrentUsage() << std::endl;
}

void printMemoryUsageWin32() {
    LOG(L"Total Memory Usage: ");
    LOG_INT(::memUsage.getCurrentUsage());
}

#ifdef WIN32_APP
#define printMemoryUsage printMemoryUsageWin32
#else
#define printMemoryUsage printMemoryUsageConsole
#endif // UNICODE

