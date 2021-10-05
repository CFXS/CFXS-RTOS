// [CFXS] //
#pragma once
#include "Thread.hpp"

namespace CFXS::RTOS {

    class Scheduler {
    public:
        static void ContextEvent();
        static void SchedulerEvent();

        static Thread* CreateThread(const char* label, const ThreadFunction& func, void* stackAddr, size_t stackSize);
    };

} // namespace CFXS::RTOS