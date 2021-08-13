#ifndef FRUIT_IMAGE_HPP
#define FRUIT_IMAGE_HPP

#include "./image_view.hpp"

#include <memory>

namespace fruit
{
	template<class T>
	class Image
	{
	public:
		explicit Image(ImageView<T> view):Image{view.data(), view.width(), view.height()}{}

		explicit Image(T* data, int width, int height):
			m_data{std::make_unique<T[]>(width*height)},
			m_width{width},
			m_height{height}
		{}

		explicit Image(int width, int height):
			m_data{std::make_unique<T[]>(width*height)},
			m_width{width},
			m_height{height}
		{}

		T& operator()(int x, int y)
		{
			return m_data[y*m_width + x];
		}

		T operator()(int x, int y) const
		{
			return m_data[y*m_width + x];
		}

		int width() const
		{
			return m_width;
		}

		int height() const
		{
			return m_height;
		}

		T const* data() const
		{
			return m_data.get();
		}

		T* data()
		{
			return m_data.get();
		}

		operator ImageView<T>()
		{
			return ImageView{data(), m_width, m_height};
		}

		operator ImageView<T const>() const
		{
			return ImageView{data(), m_width, m_height};
		}

	private:
		std::unique_ptr<T[]> m_data;
		int m_width;
		int m_height;
	};

	template<class T>
	size_t size(Image<T> const& img)
	{
		return img.width() * img.height();
	}


	template<class T>
	T const* data(Image<T> const&& img) = delete;

	template<class T>
	T const* data(Image<T> const& img)
	{
		return img.data();
	}

	template<class T>
	T* data(Image<T>& img)
	{
		return img.data();
	}

	template<class T>
	T* begin(Image<T> const& img)
	{
		return img.data();
	}

	template<class T>
	T* end(Image<T> const& img)
	{
		return img.data() + size(img);
	}

	template<class T>
	std::span<T const> make_span(Image<T> const&& img) = delete;

	template<class T>
	std::span<T const> make_span(Image<T> const& img)
	{
		return std::span{data(img), size(img)};
	}

	template<class T>
	std::span<T> make_span(Image<T>& img)
	{
		return std::span{data(img), size(img)};
	}
}

#endif