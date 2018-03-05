#pragma once
#include <type_traits>
#include <cstdint>


namespace terra
{

	template<class TEnum>
	class TEnumAsByte
	{
		static_assert(std::is_enum<TEnum>::value, "error: invalid value type!");
		typedef TEnum EnumType;
	private:
		uint8_t value_{ 0 };

		friend uint32_t GetTypeHash(const TEnumAsByte& val)
		{
			return GetTypeHash(val.Value);
		}
	public:
		TEnumAsByte() = default;

		TEnumAsByte(const TEnumAsByte& in_val) : value_(in_val.value_) {}

		TEnumAsByte(TEnum in_val) : value_(static_cast<uint8_t>(in_val)) {}

		explicit TEnumAsByte(int32_t in_val) : value_(static_cast<uint8_t>(in_val)) {}

		explicit TEnumAsByte(uint8_t in_val) : value_(in_val) {}

	public:
		TEnumAsByte& operator=(TEnumAsByte in_val)
		{
			value_ = in_val.value_;
			return *this;
		}
		TEnumAsByte& operator=(TEnum in_val)
		{
			value_ = static_cast<uint8_t>(in_val);
			return *this;
		}
		bool operator==(TEnum in_val) const
		{
			return static_cast<TEnum>(value_) == in_val;
		}
		bool operator==(TEnumAsByte in_val) const
		{
			return value_ == in_val.value_;
		}
		operator TEnum() const
		{
			return (TEnum)value_;
		}

		TEnum GetValue() const
		{
			return (TEnum)value_;
		}
	};
}