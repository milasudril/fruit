#ifndef FRUIT_WORLDCLOCK_HPP
#define FRUIT_WORLDCLOCK_HPP

#include <chrono>

namespace fruit
{
	class WorldClock
	{
	public:
		using BaseClock = std::chrono::steady_clock;
		using rep = double;
		using period = std::ratio<1>;
		using duration = std::chrono::duration<rep, period>;
		using time_point = std::chrono::time_point<WorldClock>;
		static constexpr bool is_steady = BaseClock::is_steady;

		WorldClock():m_epoch{BaseClock::now()}{}

		explicit WorldClock(BaseClock::time_point epoch):m_epoch{epoch}{}

		void reset(BaseClock::time_point new_epoch = BaseClock::now())
		{
			m_epoch = new_epoch;
		}

		time_point now() const
		{
			return time_point{BaseClock::now() - m_epoch};
		}

	private:
		BaseClock::time_point m_epoch;
	};
}

#endif