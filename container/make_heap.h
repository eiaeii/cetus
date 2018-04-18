#pragma once
#include <functional>

namespace terra
{
	namespace heap
	{
		template <class Iter>
		using value_type = typename std::iterator_traits<Iter>::value_type;

		template <class Iter>
		using diff_type = typename std::iterator_traits<Iter>::difference_type;

		template <class Comp, class T1, class T2>
		inline constexpr bool Debug_lt_pred(Comp&& comp, T1&& left, T2&& right)
		{  // test if _Pred(_Left, _Right) and _Pred is strict weak ordering
			return (comp(left, right) ? (comp(right, left) ? (assert(false), true)  // "invalid comparator"
														   : true)
									  : false);
		}

		template <class Iter, class Diff, class T, class Comp>
		inline void PushHeapByIndex(Iter first, Diff hole, Diff top, T&& val, Comp comparator)
		{
			for (Diff idx = (hole - 1) / 2; top < hole && Debug_lt_pred(comparator, *(first + idx), val);
				 idx = (hole - 1) / 2) {
				*(first + hole) = std::move(*(first + idx));
				hole = idx;
			}
			*(first + hole) = std::move(val);
		}

		template <class Iter, class Diff, class T, class Comp>
		inline void PopHeapHoleByIndex(Iter first, Diff hole, Diff bottom, T&& val, Comp comparator)
		{
			// percolate _Hole to _Bottom, then push _Val, using _Pred
			// precondition: _Bottom != 0
			const Diff top = hole;
			Diff idx = hole;

			// Check whether _Idx can have a child before calculating that child's index, since
			// calculating the child's index can trigger integer overflows
			// max_sequence_non_leaf:无叶子节点的节点总数
			const Diff max_sequence_non_leaf = (bottom - 1) / 2;
			while (idx < max_sequence_non_leaf) {
				idx = 2 * idx + 2;
				if (Debug_lt_pred(comparator, *(first + idx), *(first + (idx - 1)))) --idx;
				*(first + hole) = std::move(*(first + idx));
				hole = idx;
			}
			if (idx == max_sequence_non_leaf && bottom % 2 == 0) {
				*(first + hole) = std::move(*(first + (bottom - 1)));
				hole = bottom - 1;
			}
			PushHeapByIndex(first, hole, top, std::move(val), comparator);
		}

		template <class Iter, class Comp>
		inline void MakeHeap(Iter first, Iter last, Comp comparator)
		{
			diff_type<Iter> bottom = last - first;
			if (bottom >= 2) {
				for (diff_type<Iter> hole = bottom / 2; hole > 0;) {
					--hole;
					value_type<Iter> val = std::move(*(first + hole));
					PopHeapHoleByIndex(first, hole, bottom, std::move(val), comparator);
				}
			}
		}

		template <class Iter>
		inline void MakeHeap(Iter first, Iter last)
		{
			MakeHeap(first, last, std::less<>());
		}

		template <class Iter, class T, class Comp>
		inline void PopHeapHole(Iter first, Iter last, Iter dest, T&& val, Comp& comp)
		{  // pop *_First to *_Dest and reheap, using _Pred
			// precondition: _First != _Last
			// precondition: _First != _Dest
			*dest = std::move(*first);
			PopHeapHoleByIndex(first, diff_type<Iter>(0), diff_type<Iter>(last - first), std::move(val), comp);
		}

		template <class Iter, class Comp>
		inline void PopHeap(Iter first, Iter last, Comp comparator)
		{
			if (2 <= last - first) {
				--last;
				value_type<Iter> val = std::move(*last);
				PopHeapHole(first, last, last, std::move(val), comparator);
			}
		}

		template <class Iter>
		inline void PopHeap(Iter first, Iter last)
		{
			PopHeap(first, last, std::less<>());
		}

		template <class Iter, class Comp>
		inline void PushHeap(Iter first, Iter last, Comp comparator)
		{
			diff_type<Iter> count = last - first;
			if (2 <= count) {
				value_type<Iter> val = std::move(*--last);
				PushHeapByIndex(first, --count, diff_type<Iter>(0), std::move(val), comparator);
			}
		}

		// TEMPLATE FUNCTION push_heap
		template <class Iter>
		inline void PushHeap(Iter first, Iter last)
		{  // push *(_Last - 1) onto heap at [_First, _Last - 1), using operator<
			PushHeap(first, last, std::less<>());
		}

		template <class Iter, class Comp>
		inline void SortHeap(Iter first, Iter last, Comp comparator)
		{
			for (; 2 <= last - first; --last) PopHeap(first, last, comparator);
		}

		// TEMPLATE FUNCTION sort_heap
		template <class Iter>
		inline void SortHeap(Iter first, Iter last)
		{  // order heap by repeatedly popping, using operator<
			SortHeap(first, last, std::less<>());
		}

		template <class Iter, class Diff, class T>
		inline void PushHeapHoleByIndex(Iter first, Diff hole, Diff bottom, T&& val)
		{
			PushHeapHoleByIndex(first, hole, bottom, std::move(val), std::less<>());
		}

		template <class Iter, class Diff, class T>
		inline void PopHeapHoleByIndex(Iter first, Diff hole, Diff bottom, T&& val)
		{
			PopHeapHoleByIndex(first, hole, bottom, std::move(val), std::less<>());
		}
	} 
}