#ifndef FRUIT_LIB_DEVICEID_HPP
#define FRUIT_LIB_DEVICEID_HPP

#include <cstdint>
#include <string>

namespace fruit
{
	class DeviceId
	{
	public:
		constexpr explicit DeviceId(intptr_t value):m_value{value}{}

		constexpr intptr_t value() const
		{
			return m_value;
		}

	private:
		intptr_t m_value;
	};

	constexpr bool operator==(DeviceId a, DeviceId b)
	{ return a.value() == b.value(); }

	constexpr bool operator!=(DeviceId a, DeviceId b)
	{ return !(a == b); }

	inline std::string to_string(DeviceId id)
	{
		return std::to_string(id.value());
	}
}

#endif