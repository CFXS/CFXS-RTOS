// [CFXS] //
#pragma once
#include <CFXS/RTOS/Types.hpp>

namespace CFXS::RTOS::CoreImpl {

    // Cortex-M4 Thread
    class _CM4_Thread {
    public:
        _CM4_Thread(const ThreadFunction& func, void* stackAddr, size_t stackSize);

        size_t GetStackSize() const;
        void* GetStackEndAddress();
        void* GetStackStartAddress();

        __always_inline void SetSP(void* addr);
        __always_inline void* GetSP() const;

    private:
        void InitializeStackFrame();

    private:
        void* m_cpu_SP; // thread SP
        ThreadFunction m_ThreadFunction;
        void* m_StackEndAddress;
        size_t m_StackSize;
    };

} // namespace CFXS::RTOS::CoreImpl