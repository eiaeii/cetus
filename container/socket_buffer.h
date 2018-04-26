#pragma once

#include "core.h"

namespace terra
{
	class SocketBuffer
	{
	private:
		static const uint32_t kInitialBufferSize = 8;
		char* buffer_{ nullptr };
		uint32_t capacity_{ kInitialBufferSize };   
		uint32_t front_{ 0 };
		uint32_t back_{ 0 };
	public:
		SocketBuffer(uint32_t capacity = kInitialBufferSize);
		~SocketBuffer();

		void Append(const char* data, uint32_t len);

		void AllocBuffer(uint32_t size);

		bool IsEmpty() { return back_ == front_; }
		char* Back() { return buffer_ + back_; }
		const char* GetBuffer() const { return buffer_ + front_; }
		uint32_t Size() const { return back_ - front_; }
		uint32_t Capacity() const { return capacity_; }
		void PushBack(uint32_t size);
		void PopFront(uint32_t size);

	private:
		void ExpandBuffer(uint32_t size);
	};

}