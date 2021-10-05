// [CFXS] //
#include <CFXS/RTOS/Thread.hpp>

namespace CFXS::RTOS {

    Thread::Thread(const char* label, const ThreadFunction& func, void* stackAddr, size_t stackSize) :
        m_Label(label), m_ThreadFunction(func), m_StackEndAddress(stackAddr), m_StackSize(stackSize) {
        CFXS_ASSERT((((size_t)((size_t)stackAddr + stackSize)) % 8) == 0, "Stack top not aligned to 8");
        CFXS_ASSERT(GetStackSize() >= 32, "Not enough stack");
        _Impl_Thread_InitializeStackFrame(this);
    }

} // namespace CFXS::RTOS