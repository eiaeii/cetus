#pragma once

#include <cstdint>
#include <cassert>
#include <iostream>

namespace terra
{
    class dynamic_bitset
    {
    private:
        uint32_t size_;
        uint32_t array_size_;
        uint32_t* array_data_;
        static const int kBitLength = sizeof(uint32_t) * 8;

    public:
        explicit dynamic_bitset(uint32_t size);
        ~dynamic_bitset();

        uint32_t count() const;
        uint32_t size() const { return size_; }
        bool test(uint32_t pos) const;
		bool any() const;

        bool none() const { return (!any()); }
        bool all() const { return (count() == size()); }

        dynamic_bitset& set();
        dynamic_bitset& set(uint32_t pos);

        dynamic_bitset& reset();
        dynamic_bitset& reset(uint32_t pos);

        dynamic_bitset& flip();
        dynamic_bitset& flip(uint32_t pos);

        std::string to_string();

    private:
        uint32_t count_bit(uint32_t i) const;
    };
}