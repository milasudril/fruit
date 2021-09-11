#ifndef FRUIT_LIB_VECTOR_HPP
#define FRUIT_LIB_VECTOR_HPP

#include "./vec_t.hpp"

#include <concepts>
#include <cmath>

namespace fruit
{
	template<class T>
	class Vector
	{
	public:
		constexpr explicit Vector(vec4_t<T> value):m_value{value}
		{m_value[3] = 0.0f;}

		constexpr explicit Vector(T x, T y, T z): m_value{x, y, z}
		{}

		constexpr T x() const
		{ return m_value[0]; }

		constexpr T y() const
		{ return m_value[1]; }

		constexpr T z() const
		{ return m_value[2]; }

		constexpr T& x()
		{ return m_value[0]; }

		constexpr T& y()
		{ return m_value[1]; }

		constexpr T& z()
		{ return m_value[2]; }


		constexpr vec4_t<T> value() const
		{ return m_value; }

		constexpr Vector& operator+=(Vector vec)
		{
			m_value += vec.value();
			return *this;
		}

		constexpr Vector& operator-=(Vector vec)
		{
			m_value -= vec.value();
			return *this;
		}

		constexpr Vector& operator*=(T c)
		{
			m_value *= c;
			return *this;
		}

		constexpr inline Vector& normalize() requires std::floating_point<T>;

		constexpr Vector& scale(Vector a)
		{
			m_value *= a.value();
			return *this;
		}

		constexpr Vector& operator/=(T c)
		{
			m_value /= c;
			return *this;
		}


	private:
		vec4_t<T> m_value;
	};

	template<class T>
	constexpr Vector<T> operator*(T c, Vector<T> v)
	{
		v *= c;
		return v;
	}

	template<class T>
	constexpr Vector<T> operator/(Vector<T> v, T c)
	{
		v /= c;
		return v;
	}

	template<class T>
	constexpr Vector<T> scale(Vector<T> a, Vector<T> b)
	{
		a.scale(b);
		return a;
	}

	template<class T>
	constexpr Vector<T> operator+(Vector<T> a, Vector<T> b)
	{
		a += b;
		return a;
	}

	template<class T>
	constexpr Vector<T> operator-(Vector<T> a, Vector<T> b)
	{
		a -= b;
		return a;
	}

	template<class T>
	constexpr T dot(Vector<T> a, Vector<T> b)
	{
		auto temp = a.value() * b.value();
		return temp[0] + temp[1] + temp[2];
	}

	template<class T>
	constexpr auto length_squared(Vector<T> v)
	{
		return dot(v, v);
	}

	template<class T>
	constexpr auto length(Vector<T> v)
	{
		return std::sqrt(length_squared(v));
	}

	template<class T>
	constexpr Vector<T>& Vector<T>::normalize() requires std::floating_point<T>
	{
		m_value /= length(*this);
		return *this;
	}

	template<std::floating_point T>
	constexpr auto normalized(Vector<T> v)
	{
		return v.normalize();
	}

	template<class T>
	constexpr auto normalized(Vector<T> v)
	{
		return normalized(Vector<double>{vector_cast<double>(v.value())});
	}

	template<class T, class U>
	constexpr auto vector_cast(Vector<U> v)
	{
		return Vector<T>{vector_cast<T>(v.value())};
	}

	template<class T>
	constexpr Vector<T> transform(Vector<T> v, Vector<T> x_vec, Vector<T> y_vec, Vector<T> z_vec)
	{
		return Vector<T>{dot(v, x_vec), dot(v, y_vec), dot(v, z_vec)};
	}

	template<class T>
	constexpr Vector<T> X{1, 0, 0};

	template<class T>
	constexpr Vector<T> Y{0, 1, 0};

	template<class T>
	constexpr Vector<T> Z{0, 0, 1};
}
#endif