#include "dynamic_buffer.h"
using namespace terra;

DynamicBuffer::DynamicBuffer(uint32_t size)
{
	Expects(size > 0);

	capacity_ = RoundUpExp2(size);
	buffer_ = new char[capacity_];
	used_ = 0;
	free_ = capacity_;
}

DynamicBuffer::~DynamicBuffer()
{
	delete[] buffer_;
}

void DynamicBuffer::ReserveBuffer(uint32_t capacity)
{
	used_ = 0;
	free_ = capacity_;
	if (free_ >= capacity)
	{
		return;
	}
	ExpandBuffer(capacity);
}

void terra::DynamicBuffer::BufferConsumed(uint32_t size)
{
	used_ += size;
	free_ -= size;
}

void DynamicBuffer::ExpandBuffer(uint32_t capacity)
{
	Expects(capacity <= std::numeric_limits<uint32_t>::max()/2);
	uint32_t alloc_size = capacity_;
	while ((alloc_size - used_) > capacity)
	{
		alloc_size *= 2;
	}
	char* temp = new char[alloc_size];
	memcpy(temp, buffer_, used_);
	delete[] buffer_;
	buffer_ = temp;
	free_ = alloc_size - used_;
	capacity_ = alloc_size;
}
