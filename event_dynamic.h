#pragma once

#include <functional>
#include <memory>
#include "type_traits_ex.h"

namespace terra
{
    // powerful wrapped-class
    class event_dynamic
    {
    private:
        struct base {
            virtual ~base(){};
        };

        template <typename... Args>
        struct wrapped : base {
            typedef std::function<void(Args...)> func_type;
			func_type f;
			wrapped(const func_type& aFunc) : f(aFunc) {};
			wrapped(func_type&& aFunc) : f(std::move(aFunc)) {};
        };

        std::unique_ptr<base> base_;

    public:
        template <typename... Args>
        event_dynamic(std::function<void(Args...)>&& aFunc)
            : base_(new wrapped<Args...>(std::forward<std::function<void(Args...)>>(aFunc))){};

        template <typename... Args>
        event_dynamic& operator=(std::function<void(Args...)>&& aFunc)
        {
            base_.reset(new wrapped<Args...>(std::forward<std::function<void(Args...)>>(aFunc)));
            return *this;
        };

        template <typename... Args>
        void operator()(Args... args) const
        {
            // boost::polymorphic_downcast
            wrapped<Args...>* p_wrapped = dynamic_cast<wrapped<Args...>*>(base_.get());
            if (p_wrapped)  // <- if cast successful
				p_wrapped->f(args...);
            else
                throw std::runtime_error("Invalid arguments to function object call!");
			 
        };
    };
}
