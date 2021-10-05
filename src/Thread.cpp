// [CFXS] //
#include <CFXS/RTOS/Thread.hpp>

namespace CFXS::RTOS {

    void Thread::SetLabel(const char* newLabel) { m_Label = newLabel; }

    const char* Thread::GetLabel() const { return m_Label; }

} // namespace CFXS::RTOS