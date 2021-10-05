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
#pragma once
#include <CFXS/RTOS/_Config.hpp>
#include <CFXS/RTOS/Types.hpp>

namespace CFXS::RTOS {

    class Thread;
    extern void _Impl_Thread_InitializeStackFrame(Thread* thread);

    class Thread {
        friend class Scheduler;
        friend void _Impl_InitializeStackFrame(Thread* thread);

    public:
        __always_inline size_t GetStackSize() const { return m_StackSize; }
        __always_inline void* GetStackBottomAddress() { return m_StackEndAddress; }
        __always_inline void* GetStackTopAddress() { return (void*)((size_t)GetStackBottomAddress() + GetStackSize()); }

        __always_inline void SetSP(void* addr) { m_ThreadSP = addr; }
        __always_inline void* GetSP() const { return m_ThreadSP; }

        __always_inline void SetLabel(const char* newLabel) { m_Label = newLabel; };
        __always_inline const char* GetLabel() const { return m_Label; };

        __always_inline void* GetThreadFunctionAddress() const { return reinterpret_cast<void*>(m_ThreadFunction); }

        /// Set next thread in linked list
        __always_inline void LL_SetNextThread(Thread* thread) { m_ll_NextThread = thread; }

        /// Get next thread in linked list
        __always_inline Thread* LL_GetNextThread() const { return m_ll_NextThread; }

    private:
        Thread(const char* label, const ThreadFunction& func, void* stackAddr, size_t stackSize);

        Thread* m_ll_NextThread = nullptr;

        const char* m_Label;
        bool m_Running = false;

        void* m_ThreadSP;
        ThreadFunction m_ThreadFunction;
        void* m_StackEndAddress;
        size_t m_StackSize;
    };

} // namespace CFXS::RTOS