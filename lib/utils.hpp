#ifndef FRUIT_LIB_UTILS_HPP
#define FRUIT_LIB_UTILS_HPP

#include <span>
#include <numeric>
#include <algorithm>
#include <functional>
#include <type_traits>

namespace fruit
{
	template<class T, class Projection = std::identity>
	decltype(auto) normalize_sum(std::span<T> values, Projection&& proj = {})
	{
		using SumType = std::remove_reference_t<decltype(std::declval<Projection>()(std::declval<T>()))>;

		auto sum = std::accumulate(std::begin(values), std::end(values), SumType{0}, [proj]
			(auto a, auto b) {
			return a + proj(b);
		});

		sum = (sum == SumType{0}) ? SumType{1} : sum;

		std::ranges::for_each(values, [sum, proj](auto& val){
			proj(val) /= sum;
		});

		return sum;
	}

}

#endif