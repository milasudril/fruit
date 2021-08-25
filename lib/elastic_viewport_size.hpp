#ifndef ELASTIC_VIEWPORT_SIZE_H
#define ELASTIC_VIEWPORT_SIZE_H

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

		inline ViewportSize scale(ViewportSize size, std::pair<float, int> value)
		{
			return  ViewportSize{size.width*value.first, value.second};
		}

		inline ViewportSize scale(ViewportSize size, std::pair<int, float> value)
		{
			return ViewportSize{value.first, size.height*value.second};
		}

		inline ViewportSize scale(ViewportSize size, std::pair<float, float> value)
		{
			return ViewportSize{size.width*value.first, size.height*value.second};
		}
	}

	class ElasticViewportSize
	{
	public:
		using StorageType = std::variant<std::pair<int, int>, std::pair<int, float>, std::pair<float, int>, std::pair<float, float>>;

		ViewportSize viewportSize(ViewportSize size) const
		{
			return std::visit([size](auto value){
				return scale(size, value);
			});
		}

		void set_width(int value)
		{
			std::visit([width = value](auto& value) {
				value.first = width;
			});
		};

		void set_width(float value)
		{
			std::visit([width = value](auto& value) {
				value.first = width;
			});
		};

		void set_height(int value)
		{
			std::visit([height = value](auto& value) {
				value.second = height;
			});
		};

		void set_height(float value)
		{
			std::visit([height = value](auto& value) {
				value.second = height;
			});
		};

		void set_value(StorageType value)
		{
			return m_value = value;
		}

	private:
		StorageType m_value;
	};

}

#endif