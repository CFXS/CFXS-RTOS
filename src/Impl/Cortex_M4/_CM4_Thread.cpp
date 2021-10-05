// [CFXS] //
#ifdef CFXS_CORE_CORTEX_M4
    #include <CFXS/RTOS/Impl/Cortex_M4/_CM4_Thread.hpp>
    #include <CFXS/Base/Debug.hpp>

namespace CFXS::RTOS::CoreImpl {

    _CM4_Thread::_CM4_Thread(const ThreadFunction& func, void* stackAddr, size_t stackSize) :
        m_ThreadFunction(func), m_StackEndAddress(stackAddr), m_StackSize(stackSize) {
        CFXS_ASSERT((((size_t)((size_t)stackAddr + stackSize)) % 8) == 0, "Stack top not aligned to 8");
        CFXS_ASSERT(GetStackSize() >= 32, "Not enough stack");

        InitializeStackFrame();
    }

    size_t _CM4_Thread::GetStackSize() const { return m_StackSize; }

    void* _CM4_Thread::GetStackEndAddress() { return m_StackEndAddress; }

    void* _CM4_Thread::GetStackStartAddress() { return (void*)((size_t)GetStackEndAddress() + GetStackSize()); }

    void _CM4_Thread::SetSP(void* addr) { m_cpu_SP = addr; }

    void* _CM4_Thread::GetSP() const { return m_cpu_SP; }

    void _CM4_Thread::InitializeStackFrame() {
        size_t* sp = reinterpret_cast<size_t*>(GetStackStartAddress());

        *--sp = (1 << 24); // xPSR thumb state = 1
        *--sp = (size_t)m_ThreadFunction;
        // LR, R12, R3, R2, R1, R0, R11, R10, R9, R8, R7, R6, R5, R4
        *--sp = 1;
        *--sp = 2;
        *--sp = 3;
        *--sp = 4;
        *--sp = 5;
        *--sp = 6;
        *--sp = 7;
        *--sp = 8;
        *--sp = 9;
        *--sp = 10;
        *--sp = 11;
        *--sp = 12;
        *--sp = 13;
        *--sp = 14;

        SetSP(sp);
        memset(GetStackEndAddress(), 0xC5, GetStackSize() - 16 * 4);
    }

} // namespace CFXS::RTOS::CoreImpl

#endif