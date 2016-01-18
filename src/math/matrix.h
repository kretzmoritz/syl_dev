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
{
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

template<class T, size_t rows, size_t columns, bool row_major = true>
class Matrix
	: public Impl::MatrixSpecialized<T, rows, columns, row_major>
{
public:
	Matrix() = default;
	Matrix(Matrix<T, rows, columns, true> const& _m);
	Matrix(Matrix<T, rows, columns, false> const& _m);
	virtual ~Matrix() = default;

	Matrix<T, rows, columns, row_major>& operator=(Matrix<T, rows, columns, true> const& _m);
	Matrix<T, rows, columns, row_major>& operator=(Matrix<T, rows, columns, false> const& _m);

	T& operator()(size_t _row, size_t _column);
	T const& operator()(size_t _row, size_t _column) const;

	Matrix<T, 1, columns, row_major> get_row(size_t _idx) const;
	template<bool either_major> void set_row(size_t _idx, Matrix<T, 1, columns, either_major> const& _row);
	Matrix<T, rows, 1, row_major> get_column(size_t _idx) const;
	template<bool either_major> void set_column(size_t _idx, Matrix<T, rows, 1, either_major> const& _column);

	Matrix<T, columns, rows, row_major> transposed() const;

	template<class U, size_t other_columns = columns> using MyType = Matrix<U, rows, other_columns, row_major>;
	typedef T ElemType;
	static size_t const Rows = rows;
	static size_t const Columns = columns;
	static bool const RowMajor = row_major;
};

typedef Matrix<float, 2, 2> Mat2x2f;
typedef Matrix<float, 3, 3> Mat3x3f;
typedef Matrix<float, 4, 4> Mat4x4f;

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

template<class T, class U, class V>
using SquareMatrixReturnType 
	= typename std::enable_if<std::is_base_of<Matrix<typename T::ElemType, T::Rows, T::Rows, T::RowMajor>, T>::value 
		&& std::is_base_of<Matrix<typename U::ElemType, T::Rows, T::Rows, U::RowMajor>, U>::value, V>::type;

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
Impl::SquareMatrixReturnType<T, U, Impl::CheckElemOpReturnT<std::multiplies<>, T, U>>& operator*=(T& _lhs, U const& _rhs);

BEGIN_NAMESPACE(MatHelper)

BEGIN_NAMESPACE(Impl)

template<class T, size_t n, bool row_major>
Matrix<T, n, 1, row_major> solve_for_x(Matrix<T, n, n, row_major> const& _l, Matrix<T, n, n, row_major> const& _u, Matrix<T, n, 1, row_major> const& _b);

END_NAMESPACE

template<class T, size_t rows, size_t columns, bool row_major>
Matrix<T, rows, columns, row_major>& make_zero(Matrix<T, rows, columns, row_major>& _m);

template<class T, size_t n, bool row_major>
Matrix<T, n, n, row_major>& make_identity(Matrix<T, n, n, row_major>& _m);

template<class T, size_t n, bool row_major>
bool decompose(Matrix<T, n, n, row_major> const& _m, Matrix<T, n, n, row_major>& _l, Matrix<T, n, n, row_major>& _u);

template<class T, size_t n, bool row_major>
T determinant(Matrix<T, n, n, row_major> const& _m);

template<class T, size_t n, bool row_major>
bool inverse(Matrix<T, n, n, row_major> const& _m, Matrix<T, n, n, row_major>& _inv);

END_NAMESPACE

END_2_NAMESPACES

#include "matrix.tcc"

#endif