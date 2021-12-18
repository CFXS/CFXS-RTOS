// ---------------------------------------------------------------------
// CFXS Framework RTOS Module <https://github.com/CFXS/CFXS-RTOS>
// Copyright (C) 2021 | CFXS / Rihards Veips
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
// ---------------------------------------------------------------------
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
        Scheduler::ProcessThreads();
        asm volatile("cpsie i");
    }

    /// Sleep for at least us microseconds
    void Thread::Sleep_us(Time_t sleep_for_us) {
        m_SleepUntil = CFXS::Time::cycles + sleep_for_us * CPU::CYCLES_PER_USEC;
        asm volatile("cpsid i");
        Scheduler::ProcessThreads();
        asm volatile("cpsie i");
    }

} // namespace CFXS::RTOS