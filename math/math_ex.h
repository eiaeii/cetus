#pragma once

#include <cstdint>

namespace terra
{
	constexpr uint32_t RoundUpExp2(uint32_t v) {
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;
		return v;
	}
}