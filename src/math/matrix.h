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

	template<class U> using MyType = Matrix<U, rows, columns, row_major>;
	typedef T ElementType;
	static size_t const Rows = rows;
	static size_t const Columns = columns;
	static bool const RowMajor = row_major;
};

typedef Matrix<float, 2, 2, true> Mat2x2f;
typedef Matrix<float, 3, 3, true> Mat3x3f;
typedef Matrix<float, 4, 4, true> Mat4x4f;

BEGIN_NAMESPACE(Impl)

template<class T>
using UnaryMatrixReturnType 
	= typename std::enable_if<std::is_base_of<Matrix<typename T::ElementType, T::Rows, T::Columns, T::RowMajor>, T>::value, T>::type;

template<class T, class U>
using BinaryMatrixReturnType 
	= typename std::enable_if<std::is_base_of<Matrix<typename T::ElementType, T::Rows, T::Columns, T::RowMajor>, T>::value 
		&& std::is_base_of<Matrix<typename U::ElementType, U::Rows, U::Columns, U::RowMajor>, U>::value && T::Rows == U::Rows && T::Columns == U::Columns, T>::type;

template<class Op, class T, class U>
using OpMatrixReturnType 
	= typename std::enable_if<std::is_base_of<Matrix<typename T::ElementType, T::Rows, T::Columns, T::RowMajor>, T>::value 
		&& std::is_base_of<Matrix<typename U::ElementType, U::Rows, U::Columns, U::RowMajor>, U>::value && T::Rows == U::Rows && T::Columns == U::Columns, 
		typename T::template MyType<decltype(Op()(std::declval<typename T::ElementType>(), std::declval<typename U::ElementType>()))>>::type;

END_NAMESPACE

template<class T, size_t rows, size_t columns, bool lhs_row_major, bool rhs_row_major>
bool operator==(
	Matrix<T, rows, columns, lhs_row_major> const& _lhs, 
	Matrix<T, rows, columns, rhs_row_major> const& _rhs);

template<class T, size_t rows, size_t columns, bool lhs_row_major, bool rhs_row_major>
bool operator!=(
	Matrix<T, rows, columns, lhs_row_major> const& _lhs, 
	Matrix<T, rows, columns, rhs_row_major> const& _rhs);

template<class T, class U>
Impl::OpMatrixReturnType<std::plus<>, T, U> operator+(T const& _lhs, U const& _rhs);

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U>& operator+=(T& _lhs, U const& _rhs);

template<class T, class U>
Impl::OpMatrixReturnType<std::minus<>, T, U> operator-(T const& _lhs, U const& _rhs);

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U>& operator-=(T& _lhs, U const& _rhs);

template<class T>
Impl::UnaryMatrixReturnType<T> operator-(T _m);

template<class T>
Impl::UnaryMatrixReturnType<T> operator*(T _lhs, typename T::ElementType _rhs);

template<class T>
Impl::UnaryMatrixReturnType<T> operator*(typename T::ElementType _lhs, T _rhs);

template<class T>
Impl::UnaryMatrixReturnType<T>& operator*=(T& _lhs, typename T::ElementType _rhs);

END_2_NAMESPACES

#include "matrix.tcc"

#endif