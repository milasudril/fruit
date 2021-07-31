#ifndef FRUIT_WORLDCLOCK_HPP
#define FRUIT_WORLDCLOCK_HPP

#include <chrono>

namespace fruit
{
	using WorldClock = std::chrono::time_point<std::chrono::local_t, std::chrono::duration<double>>;
}

#endif