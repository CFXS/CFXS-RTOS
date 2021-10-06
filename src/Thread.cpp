// [CFXS] //
#include <CFXS/RTOS/Thread.hpp>
#include <CFXS/RTOS/Scheduler.hpp>

namespace CFXS::RTOS {

    Thread::Thread(uint32_t threadId, const char* label, const ThreadFunction& func, void* stackAddr, size_t stackSize) :
        m_ThreadID(threadId), m_Label(label), m_ThreadFunction(func), m_StackEndAddress(stackAddr), m_StackSize(stackSize) {
        CFXS_ASSERT(stackAddr, "Stack address is nullptr");
        CFXS_ASSERT((((size_t)((size_t)stackAddr + stackSize)) % 8) == 0, "Stack top not aligned to 8");
        CFXS_ASSERT(GetStackSize() >= 32, "Not enough stack");
        _Impl_Thread_InitializeStackFrame(this);
    }

    /// Sleep for at least ms milliseconds
    void Thread::Sleep_ms(Time_t sleep_for_ms) {
        m_SleepUntil = CFXS::Time::cycles + sleep_for_ms * CPU::CYCLES_PER_MS;
        asm volatile("cpsid i");
        Scheduler::SchedulerEvent();
        asm volatile("cpsie i");
    }

    /// Sleep for at least us microseconds
    void Thread::Sleep_us(Time_t sleep_for_us) {
        m_SleepUntil = CFXS::Time::cycles + sleep_for_us * CPU::CYCLES_PER_USEC;
        asm volatile("cpsid i");
        Scheduler::SchedulerEvent();
        asm volatile("cpsie i");
    }

} // namespace CFXS::RTOS