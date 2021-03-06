#ifndef FRUIT_LIB_ELASTICVIEWPORTSIZE_HPP
#define FRUIT_LIB_ELASTICVIEWPORTSIZE_HPP

#include "./viewport_size.hpp"

#include <variant>
#include <utility>

namespace fruit
{
	namespace elastic_viewport_detail
	{
		inline ViewportSize scale(ViewportSize, std::pair<int, int> value)
		{
			return  ViewportSize{value.first, value.second};
		}

		inline ViewportSize scale(ViewportSize size, std::pair<int, float> value)
		{
			return ViewportSize{value.first, static_cast<int>(size.height*value.second + 0.5f)};
		}

		inline ViewportSize scale(ViewportSize size, std::pair<float, int> value)
		{
			return  ViewportSize{static_cast<int>(size.width*value.first + 0.5f), value.second};
		}

		inline ViewportSize scale(ViewportSize size, std::pair<float, float> value)
		{
			return ViewportSize{static_cast<int>(size.width*value.first + 0.5f),
				static_cast<int>(size.height*value.second + 0.5f)};
		}
	}

	class ElasticViewportSize
	{
	public:
		using StorageType = std::variant<std::pair<int, int>, std::pair<int, float>, std::pair<float, int>, std::pair<float, float>>;

		explicit ElasticViewportSize(ViewportSize size = ViewportSize{0, 0}):m_value{std::pair{size.width, size.height}}
		{}

		template<class A, class B>
		explicit ElasticViewportSize(A width, B height):m_value{std::pair{width, height}}
		{}

		ViewportSize viewportSize(ViewportSize size) const
		{
			return std::visit([size](auto value){
				return elastic_viewport_detail::scale(size, value);
			}, m_value);
		}

		void set_width(int value)
		{
			std::visit([width = value, this](auto value) {
				m_value = StorageType{std::pair{width, value.second}};
			}, m_value);
		};

		void set_width(float value)
		{
			std::visit([width = value, this](auto value) {
				m_value = StorageType{std::pair{width, value.second}};
			}, m_value);
		};

		void set_height(int value)
		{
			std::visit([height = value, this](auto value) {
				m_value = StorageType{std::pair{value.first, height}};
			}, m_value);
		};

		void set_height(float value)
		{
			std::visit([height = value, this](auto value) {
				m_value = StorageType{std::pair{value.first, height}};
			}, m_value);
		};

		void set_value(StorageType value)
		{ m_value = value; }

		bool operator==(ElasticViewportSize const&) const = default;
		bool operator!=(ElasticViewportSize const&) const = default;

		StorageType const& value() const
		{
			return m_value;
		}

		StorageType& value()
		{
			return m_value;
		}

	private:
		StorageType m_value;
	};

	inline auto to_string(ElasticViewportSize const& obj)
	{
		return std::visit([](auto const item){
			return std::string{"("}
				.append(std::to_string(item.first))
				.append(" x ")
				.append(std::to_string(item.second))
				.append(")");
		}, obj.value());
	}
}
#endif