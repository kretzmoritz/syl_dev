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
// 2015 Moritz Kretz
///////////////////////////////////////

#ifndef SYLDEV_MATH_MATRIX
#define SYLDEV_MATH_MATRIX

#include <type_traits>

#include "../common/namespace.h"

BEGIN_2_NAMESPACES(SylDev, Math)

BEGIN_NAMESPACE(Impl)

template<class T, size_t rows, size_t columns>
class MatrixBase
{
	static_assert(rows > 0, "The template parameter rows needs to be bigger than 0.");
	static_assert(columns > 0, "The template parameter columns needs to be bigger than 0.");

public:
	MatrixBase();
	virtual ~MatrixBase() = default;

protected:
	virtual T& get(size_t _row, size_t _column) = 0;
	virtual T const& get(size_t _row, size_t _column) const = 0;

	T data[rows * columns];
};

template<class T, size_t rows, size_t columns, bool row_major>
class MatrixSpecialized
	: public MatrixBase<T, rows, columns>
{
public:
	virtual ~MatrixSpecialized() = default;
};

// Partial template specialization

template<class T, size_t rows, size_t columns>
class MatrixSpecialized<T, rows, columns, true>
	: public MatrixBase<T, rows, columns>
{
public:
	virtual ~MatrixSpecialized() = default;

protected:
	T& get(size_t _row, size_t _column);
	T const& get(size_t _row, size_t _column) const;
};

template<class T, size_t rows, size_t columns>
class MatrixSpecialized<T, rows, columns, false>
	: public MatrixBase<T, rows, columns>
{
public:
	virtual ~MatrixSpecialized() = default;

protected:
	T& get(size_t _row, size_t _column);
	T const& get(size_t _row, size_t _column) const;
};

///////////////////////////////////

END_NAMESPACE

template<class T, size_t rows, size_t columns, bool row_major>
class Matrix
	: public Impl::MatrixSpecialized<T, rows, columns, row_major>
{
public:
	virtual ~Matrix() = default;

	T& operator()(size_t _row, size_t _column);
	T const& operator()(size_t _row, size_t _column) const;

	template<class U, size_t other_columns = columns> using MyType = Matrix<U, rows, other_columns, row_major>;
	typedef T ElemType;
	static size_t const Rows = rows;
	static size_t const Columns = columns;
	static bool const RowMajor = row_major;
};

typedef Matrix<float, 2, 2, true> Mat2x2f;
typedef Matrix<float, 3, 3, true> Mat3x3f;
typedef Matrix<float, 4, 4, true> Mat4x4f;

BEGIN_NAMESPACE(Impl)

template<class Op, class T, class U>
using ElemOpReturnType = decltype(Op()(std::declval<typename T::ElemType>(), std::declval<typename U::ElemType>()));

template<class Op, class T, class U>
using CheckElemOpReturnT = typename std::conditional<true, T, ElemOpReturnType<Op, T, U>>::type;

template<class T>
using UnaryMatrixReturnType 
	= typename std::enable_if<std::is_base_of<Matrix<typename T::ElemType, T::Rows, T::Columns, T::RowMajor>, T>::value, T>::type;

template<class T, class U, class V>
using BinaryMatrixReturnType 
	= typename std::enable_if<std::is_base_of<Matrix<typename T::ElemType, T::Rows, T::Columns, T::RowMajor>, T>::value 
		&& std::is_base_of<Matrix<typename U::ElemType, T::Rows, T::Columns, U::RowMajor>, U>::value, V>::type;

template<class T, class U>
using MultiplyWithElementReturnType 
	= typename std::enable_if<std::is_base_of<Matrix<typename T::ElemType, T::Rows, T::Columns, T::RowMajor>, T>::value 
		&& std::is_convertible<U, typename T::ElemType>::value, T>::type;

template<class T, class U, class V>
using MultiplyWithMatrixReturnType 
	= typename std::enable_if<std::is_base_of<Matrix<typename T::ElemType, T::Rows, T::Columns, T::RowMajor>, T>::value 
		&& std::is_base_of<Matrix<typename U::ElemType, T::Columns, U::Columns, U::RowMajor>, U>::value, V>::type;

END_NAMESPACE

template<class T, size_t rows, size_t columns, bool lhs_row_major, bool rhs_row_major>
bool operator==(
	Matrix<T, rows, columns, lhs_row_major> const& _lhs, 
	Matrix<T, rows, columns, rhs_row_major> const& _rhs);

template<class T, size_t rows, size_t columns, bool lhs_row_major, bool rhs_row_major>
bool operator!=(
	Matrix<T, rows, columns, lhs_row_major> const& _lhs, 
	Matrix<T, rows, columns, rhs_row_major> const& _rhs);

template<class T>
Impl::UnaryMatrixReturnType<T> operator-(T _m);

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U, typename T::template MyType<Impl::ElemOpReturnType<std::plus<>, T, U>>> operator+(T const& _lhs, U const& _rhs);

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U, Impl::CheckElemOpReturnT<std::plus<>, T, U>>& operator+=(T& _lhs, U const& _rhs);

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U, typename T::template MyType<Impl::ElemOpReturnType<std::minus<>, T, U>>> operator-(T const& _lhs, U const& _rhs);

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U, Impl::CheckElemOpReturnT<std::minus<>, T, U>>& operator-=(T& _lhs, U const& _rhs);

template<class T, class U>
Impl::MultiplyWithElementReturnType<T, U> operator*(T _lhs, U _rhs);

template<class T, class U>
Impl::MultiplyWithElementReturnType<T, U> operator*(U _lhs, T _rhs);

template<class T, class U>
Impl::MultiplyWithElementReturnType<T, U>& operator*=(T& _lhs, U _rhs);

template<class T, class U>
Impl::MultiplyWithElementReturnType<T, U> operator/(T _lhs, U _rhs);

template<class T, class U>
Impl::MultiplyWithElementReturnType<T, U>& operator/=(T& _lhs, U _rhs);

template<class T, class U>
Impl::MultiplyWithMatrixReturnType<T, U, typename T::template MyType<Impl::ElemOpReturnType<std::multiplies<>, T, U>, U::Columns>> operator*(T const& _lhs, U const& _rhs);

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U, Impl::CheckElemOpReturnT<std::multiplies<>, T, U>>& operator*=(T& _lhs, U const& _rhs);

END_2_NAMESPACES

#include "matrix.tcc"

#endif