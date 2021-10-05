// [CFXS] //

#ifdef CFXS_CORE_CORTEX_M4
    #include <CFXS/RTOS/Impl/Cortex_M4/Cortex_M4.hpp>
namespace CFXS::RTOS {
    using Impl = CoreImpl::Cortex_M4;
}
#else
    #error Invalid core
#endif
