#ifndef FRUIT_UTILS_HPP
#define FRUIT_UTILS_HPP

#include <span>
#include <numeric>
#include <algorithm>
#include <functional>

namespace fruit
{
	template<class T, class Projection = std::identity>
	void normalize_sum(std::span<T> values, Projection&& proj = {})
	{
		auto sum = std::accumulate(std::begin(values), std::end(values), T{0}, [f = std::forward<Projection>(proj)]
			(auto a, auto b) {
			return f(a) + f(b);
		});

		sum = (sum == T{0}) ? T{1} : sum;

		std::ranges::for_each(std::begin(values), std::end(values), [sum](auto& val){
			val /= sum;
		});
	}

}

#endif