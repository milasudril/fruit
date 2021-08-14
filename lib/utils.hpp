#ifndef FRUIT_UTILS_HPP
#define FRUIT_UTILS_HPP

#include <concepts>
#include <span>
#include <numeric>
#include <algorithm>

namespace fruit
{
	template<std::floating_point T>
	void normalize_sum(std::span<T> values)
	{
		auto sum = std::accumulate(std::begin(values), std::end(values), T{0}, [](auto a, auto b) {
			return a + b;
		});

		sum = (sum == T{0}) ? T{1} : sum;

		std::ranges::for_each(std::begin(values), std::end(values), [sum](auto& val){
			val /= sum;
		});
	}
}

#endif