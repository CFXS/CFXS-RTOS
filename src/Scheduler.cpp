// [CFXS] //
#include <CFXS/RTOS/Scheduler.hpp>
#include <CFXS/Base/Time.hpp>
#include <CFXS/Base/CPU.hpp>

namespace CFXS::RTOS {

    // temp
    __attribute__((always_inline)) uint32_t __get_SP(void) {
        register uint32_t result;

        asm volatile("MRS %0, msp\n" : "=r"(result));
        return (result);
    }

    __attribute__((always_inline)) void __set_SP(uint32_t topOfMainStack) { asm volatile("MSR msp, %0\n" : : "r"(topOfMainStack) : "sp"); }

    ////////////////////////////////////////////////////////
    static constexpr auto MAX_THREAD_COUNT = 8;
    ////////////////////////////////////////////////////////
    static Thread* volatile s_Threads[MAX_THREAD_COUNT] = {};
    Thread* volatile s_CurrentThread                    = nullptr;
    Thread* volatile s_NextThread                       = nullptr;
    ////////////////////////////////////////////////////////

    // Process scheduler event
    void Scheduler::SchedulerEvent() {
        if (s_NextThread != s_CurrentThread) {
            *(size_t volatile*)0xE000ED04 = (1 << 28);
        }
    }

    // Process context switch event
    __attribute__((naked)) void Scheduler::ContextEvent() {
        asm volatile("cpsid i");
        if (s_CurrentThread) {
            asm volatile("push {r4-r11}");
            s_CurrentThread->SetSP((void*)__get_SP());
        }

        __set_SP((uint32_t)s_NextThread->GetSP());
        s_CurrentThread = s_NextThread;

        asm volatile("pop {r4-r11}");

        asm volatile("cpsie i");

        asm volatile("bx lr");
    }

    // Create new thread
    Thread* Scheduler::CreateThread(const char* label, const ThreadFunction& func, void* stackAddr, size_t stackSize) {
        auto thread = new Thread(label, func, stackAddr, stackSize);

        for (int i = 0; i < MAX_THREAD_COUNT; i++) {
            if (!s_Threads[i])
                s_Threads[i] = thread;
            break;
        }

        return thread;
    }

} // namespace CFXS::RTOS