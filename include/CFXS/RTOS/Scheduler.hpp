// [CFXS] //
#pragma once
#include "Thread.hpp"

namespace CFXS::RTOS {

    class Scheduler {
    public:
        static void UpdateContext();
        static void ProcessThreads();

        // Create new thread
        static Thread* CreateThread(const char* label, const ThreadFunction& func, void* stackAddr, size_t stackSize);

        /// Get thread currently being executed
        static Thread* GetCurrentThread();
    };

} // namespace CFXS::RTOS