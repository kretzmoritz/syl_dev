///////////////////////////////////////
//  (             (                
//  )\ )     (    )\ )             
// (()/((    )\  (()/(    (   )    
//  /(_))\ )((_)  /(_))  ))\ /((   
// (_))(()/( _   (_))_  /((_|_))\  
// / __|)(_)) |   |   \(_)) _)((_) 
// \__ \ || | |   | |) / -_)\ V /  
// |___/\_, |_|   |___/\___| \_/   
//      |__/                       
//
// 2016 Moritz Kretz
///////////////////////////////////////

#ifndef SYLDEV_MATH_VECTOR
#define SYLDEV_MATH_VECTOR

#include "matrix.h"

namespace SylDev { namespace Math {

template<class T, size_t n>
class Vector;

namespace Impl {

template<class T, size_t n>
class VectorBase
	: public Matrix<T, 1, n>
{
public:
	virtual ~VectorBase() = default;

	T& operator()(size_t _n);
	T const& operator()(size_t _n) const;

	T length() const;
	T length_sqr() const;

	template<class U, size_t other_n = n> using MyType = Vector<U, other_n>;
};

} // Impl

template<class T, size_t n>
class Vector
	: public Impl::VectorBase<T, n>
{
public:
	Vector() = default;
	template<class... Ts> Vector(T _value, Ts... _rest);
	virtual ~Vector() = default;

private:
	template<class... Ts> void SetVariadic(size_t _n, T _value, Ts... _rest);
	void SetVariadic(size_t _n, T _value);
};

template<class T>
class Vector<T, 2>
	: public Impl::VectorBase<T, 2>
{
public:
	Vector();
	Vector(T _x, T _y);
	Vector(Vector<T, 2> const& _v);
	virtual ~Vector() = default;

	Vector<T, 2>& operator=(Vector<T, 2> const& _v);

	T& x;
	T& y;
};

template<class T>
class Vector<T, 3>
	: public Impl::VectorBase<T, 3>
{
public:
	Vector();
	Vector(T _x, T _y, T _z);
	Vector(Vector<T, 3> const& _v);
	virtual ~Vector() = default;

	Vector<T, 3>& operator=(Vector<T, 3> const& _v);

	T& x;
	T& y;
	T& z;
};

template<class T>
class Vector<T, 4>
	: public Impl::VectorBase<T, 4>
{
public:
	Vector();
	Vector(T _x, T _y, T _z, T _w);
	Vector(Vector<T, 4> const& _v);
	virtual ~Vector() = default;

	Vector<T, 4>& operator=(Vector<T, 4> const& _v);

	T& x;
	T& y;
	T& z;
	T& w;
};

typedef Vector<float, 2> Vec2f;
typedef Vector<float, 3> Vec3f;
typedef Vector<float, 4> Vec4f;

typedef Vector<int32_t, 2> Vec2i;
typedef Vector<int32_t, 3> Vec3i;
typedef Vector<int32_t, 4> Vec4i;

namespace VecHelper {

template<class T, size_t n>
Vector<T, n>& normalize(Vector<T, n>& _v);

template<class T, size_t n>
T dot(Vector<T, n> const& _v1, Vector<T, n> const& _v2);

template<class T>
Vector<T, 3> cross(Vector<T, 3> const& _v1, Vector<T, 3> const& _v2);

template<class T, size_t n>
Vector<T, n> project(Vector<T, n> const& _v, Vector<T, n> const& _onto);

} // VecHelper

} } // SylDev, Math

#include "vector.tcc"

#endif