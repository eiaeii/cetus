#pragma once

#include "core.h"

namespace terra
{
	class DynamicBuffer
	{
	private:
		static const uint32_t kInitialBufferSize = 8;
		char* buffer_{ nullptr };
		uint32_t capacity_{ kInitialBufferSize };   
		uint32_t used_{ 0 };
		uint32_t free_{ 0 };
	public:
		DynamicBuffer(uint32_t capacity = kInitialBufferSize);
		~DynamicBuffer();

		void ReserveBuffer(uint32_t size);

		char* Back() { return buffer_ + used_; }
		const char* GetBuffer() const { return buffer_; }
		uint32_t Size() const { return used_; }
		uint32_t Capacity() const { return capacity_; }
		void BufferConsumed(uint32_t size);

	private:
		void ExpandBuffer(uint32_t size);
	};

}