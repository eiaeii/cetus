#pragma once

namespace terra
{
    template <class T>
    class TAutoRegister
    {
    public:
        static bool Init()
        {
            T::GetInstance();
            return true;
        }
    };

    /**	warning: the singleton that using
    this method should not care about initialization sequence
    */

    // the bool variable will be optimize in release mode, so it does not matter.
#define REG_INSTANCE_MOUDLE(class_name) \
    \
static bool class_name##_singleton_register = TAutoRegister<class_name>::Init();
}