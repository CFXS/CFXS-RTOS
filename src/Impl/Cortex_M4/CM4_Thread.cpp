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