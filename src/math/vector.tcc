#include "vector.h"

BEGIN_2_NAMESPACES(SylDev, Math)

BEGIN_NAMESPACE(Impl)

template<class T, size_t n>
T& VectorBase<T, n>::operator()(size_t _n)
{
	return get(0, _n);
}

template<class T, size_t n>
T const& VectorBase<T, n>::operator()(size_t _n) const
{
	return get(0, _n);
}

END_NAMESPACE

template<class T, size_t n>
template<class... Ts> Vector<T, n>::Vector()
{
}

template<class T, size_t n>
template<class... Ts> Vector<T, n>::Vector(Ts... _values)
{
	static_assert(sizeof...(Ts) == n, "Wrong number of parameters.");

	SetVariadic(0, _values...);
}

template<class T, size_t n>
template<class... Ts> void Vector<T, n>::SetVariadic(size_t _n, T _value, Ts... _values)
{
	(*this)(_n) = _value;
	SetVariadic(_n + 1, _values...);
}

template<class T, size_t n>
void Vector<T, n>::SetVariadic(size_t _n, T _value)
{
	(*this)(_n) = _value;
}

template<class T>
Vector<T, 2>::Vector()
	: x(data[0]), y(data[1])
{
}

template<class T>
Vector<T, 2>::Vector(T _x, T _y)
	: x(data[0]), y(data[1])
{
	x = _x;
	y = _y;
}

template<class T>
Vector<T, 2>::Vector(Vector<T, 2> const& _v)
	: x(data[0]), y(data[1])
{
	x = _v.x;
	y = _v.y;
}

template<class T>
Vector<T, 3>::Vector()
	: x(data[0]), y(data[1]), z(data[2])
{
}

template<class T>
Vector<T, 3>::Vector(T _x, T _y, T _z)
	: x(data[0]), y(data[1]), z(data[2])
{
	x = _x;
	y = _y;
	z = _z;
}

template<class T>
Vector<T, 3>::Vector(Vector<T, 3> const& _v)
	: x(data[0]), y(data[1]), z(data[2])
{
	x = _v.x;
	y = _v.y;
	z = _v.z;
}

template<class T>
Vector<T, 4>::Vector()
	: x(data[0]), y(data[1]), z(data[2]), w(data[3])
{
}

template<class T>
Vector<T, 4>::Vector(T _x, T _y, T _z, T _w)
	: x(data[0]), y(data[1]), z(data[2]), w(data[3])
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

template<class T>
Vector<T, 4>::Vector(Vector<T, 4> const& _v)
	: x(data[0]), y(data[1]), z(data[2]), w(data[3])
{
	x = _v.x;
	y = _v.y;
	z = _v.z;
	w = _v.w;
}

END_2_NAMESPACES