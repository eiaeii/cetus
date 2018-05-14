#include "socket_buffer.h"
using namespace terra;

SocketBuffer::SocketBuffer(uint32_t size)
{
	Expects(size > 0);

	capacity_ = RoundUpExp2(size);
	buffer_ = new char[capacity_];
	front_ = back_ = 0;
}

SocketBuffer::~SocketBuffer()
{
	delete[] buffer_;
}

void SocketBuffer::Append(const char* data, uint32_t len)
{
	AllocBuffer(len);
	memcpy(buffer_ + back_, data, len);
	PushBack(len);
}

void SocketBuffer::AllocBuffer(uint32_t size)
{
	Expects(static_cast<int>(size) > 0);
	int free = capacity_ - back_;
	if (free >= static_cast<int>(size))
	{
		return;
	}
	free += front_;
	if (free >= static_cast<int>(size))
	{
		int len = back_ - front_;
		memmove(buffer_, buffer_ + front_, len);
		front_ = 0;
		back_ = len;
	}
	else
	{
		ExpandBuffer(size);
	}
}

void SocketBuffer::PushBack(uint32_t size)
{
	back_ += size;
	Ensures(back_ <= capacity_);
}

void SocketBuffer::PopFront(uint32_t size)
{
	front_ += size;
	Ensures(front_ <= back_);
	if (front_ == back_)
	{
		front_ = back_ = 0;
	}
}

void SocketBuffer::ExpandBuffer(uint32_t size)
{
	Expects(size <= std::numeric_limits<uint32_t>::max()/2);
	uint32_t alloc_size = capacity_;
	while ((alloc_size - back_) < size)
	{
		alloc_size *= 2;
	}
	char* temp = new char[alloc_size];
	memcpy(temp, buffer_ + front_, back_ - front_);
	delete[] buffer_;
	buffer_ = temp;
	capacity_ = alloc_size;
}
