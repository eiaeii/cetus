#pragma once

#include <vector>

namespace terra
{
	template<class T, class Alloc = std::allocator<T>>
	class TVector : public std::vector<T, Alloc>
	{
	public:
		void swap_to_end(size_type index)
		{
			std::swap(at(index), back());
		}
		//FixMe size_type must be signed type
		size_type push_unique(const T& val)
		{	// insert element at end, provide strong guarantee
			return push_unique_impl(_Val);
		}

		size_type push_unique(T&& val)
		{	// insert by moving into element at end, provide strong guarantee
			return push_unique_impl(std::move(val));
		}

		bool find(const T& val, size_type& index) const
		{
			index = this->find(val);
			return index != -1;
		}
		size_type find(const T& val) const
		{
			for (const_iterator it = begin(); it != end(); ++it)
			{
				if (*it == val)
				{
					return (it - begin());
				}
			}
			return -1;
		}
		bool find_last(const T& val, size_type& index) const
		{
			index = this->find(val);
			return index != -1;
		}
		size_type find_last(const T& val) const
		{
			for (const_reverse_iterator it = rbegin(); it != rend(); ++it)
			{
				if (*it == val)
				{
					return rend() - it - 1;
				}
			}
			return -1;
		}
	private:
		size_type push_unique_impl(T&& val)
		{
			size_type index;
			if (this->find(val, index))
			{
				return index;
			}
			push_back(std::forward<T>(val));
			return size() - 1;
		}
	};
}