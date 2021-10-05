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

    class Thread : public Impl::Thread {
        friend class Scheduler;

    public:
        void SetLabel(const char* newLabel);
        const char* GetLabel() const;

    private:
        Thread(const char* label, const ThreadFunction& func, void* stackAddr, size_t stackSize) :
            Impl::Thread(func, stackAddr, stackSize), m_Label(label) {}

        const char* m_Label;
        bool m_Running = false;
    };

} // namespace CFXS::RTOS