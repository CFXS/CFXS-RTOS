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
#include "Thread.hpp"

namespace CFXS::RTOS {

    class Scheduler {
    public:
        static void UpdateContext();
        static void ProcessThreads();

        // Create new thread
        static Thread* CreateThread(const char* label, const ThreadFunction& func, void* stackAddr, size_t stackSize);

        /// Get thread currently being executed
        static Thread* GetCurrentThread();
    };

} // namespace CFXS::RTOS