// [CFXS] //
#include <CFXS/RTOS/Scheduler.hpp>
#include <CFXS/Base/Time.hpp>
#include <CFXS/Base/CPU.hpp>
#include <CFXS/Base/Debug.hpp>

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
    int s_ThreadID                   = 0;
    Thread* volatile s_LastThread    = nullptr;
    Thread* volatile s_CurrentThread = nullptr;
    Thread* volatile s_NextThread    = nullptr;
    ////////////////////////////////////////////////////////

    // Process scheduler event
    void Scheduler::ProcessThreads() {
        Time_t currentTime = CFXS::Time::cycles;

        s_NextThread = s_NextThread->LL_GetNextThread();

        if (s_NextThread->GetID() == 0) {
            s_NextThread = s_NextThread->LL_GetNextThread();
        }

        while (currentTime < s_NextThread->GetSleepUntil()) {
            s_NextThread = s_NextThread->LL_GetNextThread();
        }

        if (s_NextThread && s_NextThread != s_CurrentThread) {
            *(size_t volatile*)0xE000ED04 = (1 << 28);
        }
    }

    // Process context switch event
    __naked void Scheduler::UpdateContext() {
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

    Thread* Scheduler::CreateThread(const char* label, const ThreadFunction& func, void* stackAddr, size_t stackSize) {
        asm volatile("cpsid i");
        if (s_ThreadID == 0) {
            // create idle thread
            static __attribute__((aligned(8))) uint32_t s_IdleThreadStack[32];
            s_NextThread = new Thread(
                s_ThreadID++,
                "CFXS RTOS Idle Thread",
                []() {
                    while (1 < 2) {
                        asm volatile("wfi");
                    }
                },
                s_IdleThreadStack,
                sizeof(s_IdleThreadStack));

            s_NextThread->LL_SetNextThread(s_NextThread);
            s_LastThread = s_NextThread;
        }

        auto thread = new Thread(s_ThreadID++, label, func, stackAddr, stackSize);

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

    Thread* Scheduler::GetCurrentThread() {
        return s_CurrentThread; //
    }

} // namespace CFXS::RTOS