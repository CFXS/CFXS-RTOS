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
    static Thread* volatile s_Threads[MAX_THREAD_COUNT] = {};
    static int s_ThreadCount                            = 0;
    Thread* volatile s_CurrentThread                    = nullptr;
    Thread* volatile s_NextThread                       = nullptr;
    ////////////////////////////////////////////////////////

    // Process scheduler event
    void Scheduler::SchedulerEvent() {
        static int s_ThreadIndex = 0;
        if (s_ThreadCount) {
            s_NextThread = s_Threads[s_ThreadIndex++ % s_ThreadCount];
        }

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

        for (int i = 0; i < MAX_THREAD_COUNT; i++) {
            if (!s_Threads[i]) {
                s_Threads[i] = thread;
                s_ThreadCount++;
                break;
            }
        }

        return thread;
    }

} // namespace CFXS::RTOS