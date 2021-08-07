//@	{"target":{"name":"error_message.o"}}

#include "./error_message.hpp"

fruit::ErrorMessage::ErrorMessage(char const* file, int line, char const* tag):m_write_offset{0}, m_storage{}
{
	auto const ptr = std::begin(m_storage);
	auto result = fmt::format_to_n(ptr, BufferSize, "{}:{}: {}: ", file, line, tag);
	m_write_offset = std::distance(ptr, result.out);
}

