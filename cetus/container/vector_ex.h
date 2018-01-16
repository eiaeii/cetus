#pragma once

#include <vector>
namespace terra
{
	template<class T, class Alloc = std::allocator<T>>
	class TVector : public std::vector<T, Alloc>
	{
	public:
		void RemoveAtSwap()
		{
			using std::swap;
			swap(vec[i], vec.back());
			vec.pop_back();
		}


	};
}