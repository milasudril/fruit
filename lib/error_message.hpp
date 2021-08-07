//@	{
//@	"dependencies_extra":[
//@		{"ref":"./error_message.o", "rel":"implementation"},
//@		{"ref":"fmt", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef FRUIT_ERRORMESSAGE_HPP
#define FRUIT_ERRORMESSAGE_HPP

#include <fmt/core.h>
#include <fmt/format.h>
#include <unistd.h>

#include <cstdlib>
#include <array>
#include <span>

namespace fruit
{
	class ErrorMessage
	{
	public:
		explicit ErrorMessage(char const* file, int line, char const* tag);

		template<class ... Args>
		ErrorMessage& append(char const* message, Args&& ... args)
		{
			auto const n = BufferSize - m_write_offset;
			auto const ptr = std::begin(m_storage);
			auto const result = fmt::format_to_n(ptr + m_write_offset, n, message, std::forward<Args>(args)...);
			m_write_offset = std::distance(ptr, result.out);
			return *this;
		}

		std::span<char const> get() const
		{
			return std::span{std::data(m_storage), m_write_offset};
		}

	private:
		size_t m_write_offset;
		static constexpr size_t BufferSize = 4095;
		std::array<char, BufferSize + 1> m_storage;
	};

	[[noreturn]] inline void write_and_abort(ErrorMessage const& msg)
	{
		auto buffer = msg.get();
		write(STDERR_FILENO, std::data(buffer), std::size(buffer));
		fsync(STDERR_FILENO);
		abort();
	}

	template<class ... Args>
	ErrorMessage make_error_message(char const* file, int line, char const* tag, char const* message, Args&& ... args)
	{
		return ErrorMessage{file, line, tag}.append(message, std::forward<Args>(args)...);
	}

	template<class ... Args>
	[[noreturn]] void write_and_abort(char const* file, int line, char const* tag, char const* message, Args&& ... args)
	{
		write_and_abort(make_error_message(file, line, tag, message, std::forward<Args>(args)...));
	}
}

#define FRUIT_JAM(...) \
fruit::write_and_abort(__FILE__, __LINE__, "Fruit jam", __VA_ARGS__)

#ifndef NDEBUG
#define FRUIT_ASSERT(expression) \
do \
{ \
	if(!(expression)) \
	{ fruit::write_and_abort(__FILE__, __LINE__, "Fruit mold detected", "{}", expression); } \
} while(false)
#else
	#define FRUIT_ASSERT(expression)
#endif

#endif