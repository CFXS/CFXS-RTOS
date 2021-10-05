// [CFXS] //
#ifdef CFXS_CORE_CORTEX_M4
    #include <CFXS/Base/Debug.hpp>
    #include <CFXS/RTOS/Thread.hpp>

namespace CFXS::RTOS {

    void _Impl_Thread_InitializeStackFrame(Thread* thread) {
        size_t* sp = reinterpret_cast<size_t*>(thread->GetStackTopAddress());

        *--sp = (1 << 24); // xPSR thumb state = 1
        *--sp = (size_t)thread->GetThreadFunctionAddress();
        // LR, R12, R3, R2, R1, R0, R11, R10, R9, R8, R7, R6, R5, R4
        sp -= 14;

        thread->SetSP(sp);
        memset(thread->GetStackBottomAddress(), 0xC5, thread->GetStackSize() - 16 * 4);
    }

} // namespace CFXS::RTOS

#endif