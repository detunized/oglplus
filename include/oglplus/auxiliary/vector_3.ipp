/**
 *  .file oglplus/auxiliary/vector_3.ipp
 *  .brief Specialization of Vector for 3D vectors
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename T>
class Vector<T, 3>
 : public VectorBase<T, 3>
{
private:
	typedef VectorBase<T, 3> Base;
public:
	Vector(void)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = T(0))
	{ }

	template <typename U>
	Vector(const Vector<U, 3>& vector)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = T(std::declval<U>()))
	 : Base(vector)
	{ }

	Vector(const T (&v)[3])
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = std::declval<T>())
	 : Base(v)
	{ }

	Vector(const T* v, std::size_t n)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = std::declval<T>())
	 : Base(v, n)
	{ }

	explicit Vector(T v0)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = std::declval<T>())
	 : Base(v0)
	{ }

	Vector(T v0, T v1, T v2)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = std::declval<T>())
	 : Base(oglplus::Nothing())
	{
		this->_elem[0] = v0;
		this->_elem[1] = v1;
		this->_elem[2] = v2;
	}

	Vector(const Vector<T, 1>& v, T v1, T v2)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = std::declval<T>())
	 : Base(oglplus::Nothing())
	{
		this->_elem[0] = v[0];
		this->_elem[1] = v1;
		this->_elem[2] = v2;
	}

	Vector(const Vector<T, 2>& v, T v2)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = std::declval<T>())
	 : Base(oglplus::Nothing())
	{
		this->_elem[0] = v[0];
		this->_elem[1] = v[1];
		this->_elem[2] = v2;
	}

	explicit Vector(const Matrix<T, 1, 3>& matrix)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = std::declval<T>())
	 : Base(matrix)
	{ }

	explicit Vector(const Matrix<T, 3, 1>& matrix)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = std::declval<T>())
	 : Base(matrix)
	{ }

	T x(void) const
	OGLPLUS_NOEXCEPT_IF(T(std::declval<T&>()))
	{
		return this->At(0);
	}

	T y(void) const
	OGLPLUS_NOEXCEPT_IF(T(std::declval<T&>()))
	{
		return this->At(1);
	}

	T z(void) const
	OGLPLUS_NOEXCEPT_IF(T(std::declval<T&>()))
	{
		return this->At(2);
	}

	Vector<T, 2> xy(void) const
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = std::declval<T>())
	{
		return Vector<T, 2>(this->At(0), this->At(1));
	}

	friend Vector Negated(const Vector& a)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() = -std::declval<T>())
	{
		return Vector(-a[0], -a[1], -a[2]);
	}

	friend Vector Added(const Vector& a, const Vector& b)
	OGLPLUS_NOEXCEPT_IF(
		std::declval<T&>()=
		std::declval<T>()+
		std::declval<T>()
	)
	{
		return Vector(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
	}

	Vector& operator += (const Vector& v)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() += std::declval<T>())
	{
		this->Add(v);
		return *this;
	}

	friend Vector Subtracted(const Vector& a, const Vector& b)
	OGLPLUS_NOEXCEPT_IF(
		std::declval<T&>()=
		std::declval<T>()-
		std::declval<T>()
	)
	{
		return Vector(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
	}

	Vector& operator -= (const Vector& v)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() -= std::declval<T>())
	{
		this->Subtract(v);
		return *this;
	}


	friend Vector Multiplied(const Vector& a, T v)
	OGLPLUS_NOEXCEPT_IF(
		std::declval<T&>()=
		std::declval<T>()*
		std::declval<T>()
	)
	{
		return Vector(a[0]*v, a[1]*v, a[2]*v);
	}

	Vector& operator *= (T v)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() *= std::declval<T>())
	{
		this->MultiplyBy(v);
		return *this;
	}

	friend Vector Divided(const Vector& a, T v)
	OGLPLUS_NOEXCEPT_IF(
		std::declval<T&>()=
		std::declval<T>()/
		std::declval<T>()
	)
	{
		return Vector(a[0]/v, a[1]/v, a[2]/v);
	}

	Vector& operator /= (T v)
	OGLPLUS_NOEXCEPT_IF(std::declval<T&>() /= std::declval<T>())
	{
		this->DivideBy(v);
		return *this;
	}
};

