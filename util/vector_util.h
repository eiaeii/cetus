#pragma once

#include "core.h"
#include "container/make_heap.h"

namespace terra
{
	template<class T, typename = std::enable_if_t<is_vector<T>::value>>
	class VectorUtil
	{
		using ValueType = typename T::value_type;
		using SizeType = typename T::size_type;
		using DiffType = ssize_t;
	public:
		static void RemoveAtSwap(T& vec, SizeType index)
		{
			std::swap(vec.at(index), vec.back());
			vec.pop_back();
		}
		static void SwapToEnd(T& vec, SizeType index)
		{
			std::swap(vec.at(index), vec.back());
		}
		static SizeType PushUnique(T& vec, const ValueType& val)
		{
			return PushUniqueImpl(vec, val);
		}

		static SizeType PushUnique(T& vec, ValueType&& val)
		{	// insert by moving into element at end, provide strong guarantee
			return PushUniqueImpl(vec, std::move(val));
		}

		static bool Find(T& vec, const ValueType& val, DiffType& index)
		{
			index = Find(vec, val);
			return index != -1;
		}
		static DiffType Find(T& vec, const ValueType& val)
		{
			for (auto it = vec.begin(); it != vec.end(); ++it)
			{
				if (*it == val)
				{
					return (it - vec.begin());
				}
			}
			return -1;
		}
		static bool FindLast(T& vec, const ValueType& val, DiffType& index)
		{
			index = FindLast(vec, val);
			return index != -1;
		}
		static DiffType FindLast(T& vec, const ValueType& val)
		{
			for (auto it = vec.rbegin(); it != vec.rend(); ++it)
			{
				if (*it == val)
				{
					return vec.rend() - it - 1;
				}
			}
			return -1;
		}

		template<typename Pred>
		static T Filter_If(T& vec, Pred pred)
		{
			T temp;
			for (auto it = vec.begin(); it != vec.end(); ++it)
			{
				if (pred(*it))
				{
					temp.push_back(*it);
				}
			}

			return temp;
		}

		//	Heap Util
		static void HeapPush(T& vec, const ValueType& val)
		{
			vec.push_back(val);
			heap::PushHeap(vec.begin(), vec.end());
		}

		template<typename Pred>
		static void HeapPush(T& vec, const ValueType& val, const Pred& pred)
		{
			vec.push_back(val);
			heap::PushHeap(vec.begin(), vec.end(), pred);
		}

		static void HeapPush(T& vec, ValueType&& val)
		{
			vec.push_back(std::move(val));
			heap::PushHeap(vec.begin(), vec.end());
		}

		template<typename Pred>
		static void HeapPush(T& vec, ValueType&& val, const Pred& pred)
		{
			vec.push_back(std::move(val));
			heap::PushHeap(vec.begin(), vec.end(), pred);
		}

		static void HeapPop(T& vec, ValueType& out_val)
		{
			out_val = vec[0];
			heap::PopHeap(vec.begin(), vec.end());
			vec.pop_back();
		}

		template<typename Pred>
		static void HeapPop(T& vec, ValueType& out_val, const Pred& pred)
		{
			out_val = vec[0];
			heap::PopHeap(vec.begin(), vec.end(), pred);
			vec.pop_back();
		}

		static void HeapRemoveAt(T& vec, DiffType pos)
		{
			heap::PopHeapHoleByIndex(vec.begin(), pos, vec.end() - vec.begin(), vec.back());
		}

		static ValueType& HeadTop(T& vec) { return vec.front(); }
	private:
		static SizeType PushUniqueImpl(T& vec, const ValueType& val)
		{
			DiffType index;
			if (Find(vec, val, index))
			{
				return index;
			}
			vec.push_back(val);
			return vec.size() - 1;
		}
		static SizeType PushUniqueImpl(T& vec, ValueType&& val)
		{
			DiffType index;
			if (Find(vec, val, index))
			{
				return index;
			}
			vec.push_back(std::move(val));
			return vec.size() - 1;
		}
	};
	
}