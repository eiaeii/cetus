#include "ringbuffer.h"

using namespace terra;

ring_buffer::ring_buffer(int size) : size_(size)
{
	Expects(size > 0);
	size_ = RoundUpExp2(size);
	buffer_ = new char[size_];
}
ring_buffer::~ring_buffer() { delete[] buffer_; }

void ring_buffer::write(const char* data, int len)
{
	Expects(writable_size() >= len);
	len = std::min<int>(len, size_ - in_ + out_);

	auto l = std::min<int>(len, size_ - (in_ & (size_ - 1)));
	memcpy(buffer_ + (in_ & (size_ - 1)), data, l);
	memcpy(buffer_, data + l, len - l);

	in_ += len;
}
void ring_buffer::read(char* data, int len)
{
	Expects(readable_size() >= len);
	len = std::min<int>(len, in_ - out_);

	auto l = std::min<int>(len, size_ - (out_ & (size_ - 1)));
	memcpy(data, buffer_ + (out_ & (size_ - 1)), l);
	memcpy(data + l, buffer_, len - l);

	out_ += len;
}
void ring_buffer::peek(char* data, int len)
{
	Expects(readable_size() >= len);
	len = std::min<int>(len, in_ - out_);

	auto l = std::min<int>(len, size_ - (out_ & (size_ - 1)));
	memcpy(data, buffer_ + (out_ & (size_ - 1)), l);
	memcpy(data + l, buffer_, len - l);
}