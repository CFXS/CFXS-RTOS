// [CFXS] //
#include <CFXS/RTOS/Scheduler.hpp>
#include <CFXS/Base/Time.hpp>
#include <CFXS/Base/CPU.hpp>

namespace CFXS::RTOS {

    // temp
    __always_inline uint32_t __get_SP(void) {
        register uint32_t result;

        asm volatile("MRS %0, msp\n" : "=r"(result));
        return (result);
    }
#pragma GCC diagnostic ignored "-Wdeprecated"
    __always_inline void __set_SP(uint32_t topOfMainStack) {
        asm volatile("MSR msp, %0\n" : : "r"(topOfMainStack) : "sp"); //
    }
#pragma GCC diagnostic pop

    ////////////////////////////////////////////////////////
    static constexpr auto MAX_THREAD_COUNT = 8;
    ////////////////////////////////////////////////////////
    Thread* volatile s_LastThread    = nullptr;
    Thread* volatile s_CurrentThread = nullptr;
    Thread* volatile s_NextThread    = nullptr;
    ////////////////////////////////////////////////////////

    // Process scheduler event
    void Scheduler::SchedulerEvent() {
        if (!s_NextThread)
            s_NextThread = s_LastThread ? s_LastThread->LL_GetNextThread() : nullptr;
        else
            s_NextThread = s_NextThread->LL_GetNextThread();

        if (s_NextThread && s_NextThread != s_CurrentThread) {
            *(size_t volatile*)0xE000ED04 = (1 << 28);
        }
    }

    // Process context switch event
    __naked void Scheduler::ContextEvent() {
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

        asm volatile("cpsid i");
        if (!s_LastThread) {
            // Initialize circular linked list
            s_LastThread = thread;
            s_LastThread->LL_SetNextThread(thread);
        } else {
            // insert at end
            thread->LL_SetNextThread(s_LastThread->LL_GetNextThread());
            s_LastThread->LL_SetNextThread(thread);
            s_LastThread = thread;
        }
        asm volatile("cpsie i");

        return thread;
    }

} // namespace CFXS::RTOS