#pragma once

#include <mutex>
#include <list>
#include <vector>
#include <memory>

namespace terra
{
	template<typename>
	class Delegate;

	namespace DelegateImpl
	{
		template <typename TReturnType, typename... TArgs>
		struct Invoker
		{
			using ReturnType = std::vector<TReturnType>;

		public:
			static ReturnType Invoke(Delegate<TReturnType(TArgs...)> &delegate, TArgs... params)
			{
				std::lock_guard<std::mutex> lock(delegate.mutex_);
				ReturnType returnValues;

				for (const auto &function_ptr : delegate.function_list_)
				{
					returnValues.push_back((*function_ptr)(params...));
				}

				return returnValues;
			}
		};

		template <typename... TArgs>
		struct Invoker<void, TArgs...>
		{
			using ReturnType = void;

		public:
			static void Invoke(Delegate<void(TArgs...)> &delegate, TArgs... params)
			{
				std::lock_guard<std::mutex> lock(delegate.mutex_);

				for (const auto &function_ptr : delegate.mFunctionList)
				{
					(*function_ptr)(params...);
				}
			}
		};
	}

	template<typename TReturnType, typename... TArgs>
	class Delegate<TReturnType(TArgs...)>
	{
		using Invoker = DelegateImpl::Invoker<TReturnType, TArgs...>;
		using functionType = std::function<TReturnType(TArgs...)>;

		friend Invoker;

	public:
		Delegate() {}
		~Delegate() {}

		Delegate(const Delegate&) = delete;
		const Delegate& operator =(const Delegate&) = delete;

		Delegate& Connect(const functionType &function)
		{
			std::lock_guard<std::mutex> lock(this->mutex_);

			this->function_list_.push_back(std::make_shared<functionType>(function));

			return *this;
		}

		Delegate& Remove(const functionType &function)
		{
			std::lock_guard<std::mutex> lock(this->mutex_);

			this->function_list_.remove_if([&](std::shared_ptr<functionType> &functionPtr)
			{
				return Hash(function) == Hash(*functionPtr);
			});

			return *this;
		}

		inline typename Invoker::ReturnType Invoke(TArgs... args)
		{
			return Invoker::Invoke(*this, args...);
		}

		Delegate& Clear()
		{
			std::lock_guard<std::mutex> lock(this->mutex_);

			this->function_list_.clear();

			return *this;
		}

		inline Delegate& operator +=(const functionType &function)
		{
			return Connect(function);
		}

		inline Delegate& operator -=(const functionType &function)
		{
			return Remove(function);
		}

		inline typename Invoker::ReturnType operator ()(TArgs... args)
		{
			return Invoker::Invoke(*this, args...);
		}

	private:
		std::mutex mutex_;
		std::list<std::shared_ptr<functionType>> function_list_;

		inline constexpr size_t Hash(const functionType &function) const
		{
			return function.target_type().hash_code();
		}
	};
}
