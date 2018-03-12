#pragma once

#include "core.h"

namespace terra
{
	//brevity is the soul of wit.
	class ring_buffer
	{
	private:
		int in_{ 0 };
		int out_{ 0 };
		int size_{ 0 };
		char* buffer_{ nullptr };
	public:
		ring_buffer(int size);
		~ring_buffer();

		void write(const char* data, int len);
		void read(char* data, int len);
		void peek(char* data, int len);

		int readable_size() { return in_ - out_; }
		int writable_size() { return size_ - in_ + out_; }
		int buffer_size() { return size_; }
	};
}