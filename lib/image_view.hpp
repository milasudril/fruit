#ifndef FRUIT_IMAGEVIEW_HPP
#define FRUIT_IMAGEVIEW_HPP

#include <cstddef>
#include <span>

namespace fruit
{
	template<class T>
	class ImageView
	{
	public:
		constexpr explicit ImageView(T* data, int width, int height):
			m_data{data},
			m_width{width},
			m_height{height}
		{}

		constexpr T& operator()(int x, int y)
		{
			return data[y*m_width + x];
		}

		constexpr T operator()(int x, int y) const
		{
			return data[y*m_width + x];
		}

		constexpr int width() const
		{
			return m_width;
		}

		constexpr int height() const
		{
			return m_height;
		}

		constexpr T* data() const
		{
			return m_data;
		}

	private:
		T* m_data;
		int m_width;
		int m_height;
	};

	template<class T>
	size_t size(ImageView<T> img_view)
	{
		return img_view.width() * img_view.height();
	}

	template<class T>
	T* data(ImageView<T> img_view)
	{
		return img_view.data();
	}

	template<class T>
	T* begin(ImageView<T> img_view)
	{
		return img_view.data();
	}

	template<class T>
	T* end(ImageView<T> img_view)
	{
		return img_view.data() + size(img_view);
	}

	template<class T>
	std::span<T> make_span(ImageView<T> img_view)
	{
		return std::span{data(img_view), size(img_view)};
	}
}

#endif