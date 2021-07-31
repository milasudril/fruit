#ifndef FRUIT_MATHUTILS_HPP
#define FRUIT_MATHUTILS_HPP

#include "./vec_t.hpp"
#include "./vector.hpp"

namespace fruit
{
	template<class T>
	class Point
	{
	public:
		constexpr explicit Point(vec4_t<T> v): m_value{v}
		{
			m_value[3] = static_cast<T>(1);
		}

		constexpr explicit Point(T x, T y, T z): m_value{x, y, z, static_cast<T>(1)}
		{}

		constexpr T x() const
		{ return m_value[0]; }

		constexpr T y() const
		{ return m_value[1]; }

		constexpr T z() const
		{ return m_value[2]; }

		constexpr vec4_t<T> value() const
		{ return m_value; }

		constexpr Point& operator+=(Vector<T> vec)
		{
			m_value += vec.value();
			return *this;
		}

		Point& operator-=(Vector<T> vec)
		{
			m_value -= vec.value();
			return *this;
		}

	private:
		vec4_t<T> m_value;
	};

	template<class T>
	constexpr Vector<T> operator-(Point<T> a, Point<T> b)
	{
		return Vector<T>{a.value() - b.value()};
	}

	template<class T>
	constexpr Point<T> operator+(Point<T> a, Vector<T> b)
	{
		a += b;
		return a;
	}

	template<class T>
	constexpr Point<T> operator-(Point<T> a, Vector<T> b)
	{
		a -= b;
		return a;
	}

	template<class T>
	constexpr T distance_squared(Point<T> a, Point<T> b)
	{
		return length_squared(a - b);
	}

	template<class T>
	constexpr auto distance(Point<T> a, Point<T> b)
	{
		return length(a - b);
	}

	template<class T>
	constexpr auto midpoint(Point<T> a, Point<T> b)
	{
		return Point<T>{0.5f*(a.value() + b.value())};
	}

	template<class T, class U>
	constexpr auto vector_cast(Point<U> v)
	{
		return Point<T>{vector_cast<T>(v.value())};
	}

	template<class T>
	constexpr Point<T> Origin{0.0f, 0.0f, 0.0f};
}
#endif