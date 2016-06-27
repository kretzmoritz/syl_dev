#include "matrix.h"

#include <assert.h>

namespace SylDev { namespace Math {

namespace Impl {

template<class T, size_t rows, size_t columns>
MatrixBase<T, rows, columns>::MatrixBase()
	: data()
{
}

template<class T, size_t rows, size_t columns>
T& MatrixSpecialized<T, rows, columns, true>::get(size_t _row, size_t _column)
{
	assert(_row >= 0 && _row < rows && _column >= 0 && _column < columns);
	
	return data[_row * columns + _column];
}

template<class T, size_t rows, size_t columns>
T const& MatrixSpecialized<T, rows, columns, true>::get(size_t _row, size_t _column) const
{
	assert(_row >= 0 && _row < rows && _column >= 0 && _column < columns);

	return data[_row * columns + _column];
}

template<class T, size_t rows, size_t columns>
T& MatrixSpecialized<T, rows, columns, false>::get(size_t _row, size_t _column)
{
	assert(_row >= 0 && _row < rows && _column >= 0 && _column < columns);

	return data[_column * rows + _row];
}

template<class T, size_t rows, size_t columns>
T const& MatrixSpecialized<T, rows, columns, false>::get(size_t _row, size_t _column) const
{
	assert(_row >= 0 && _row < rows && _column >= 0 && _column < columns);

	return data[_column * rows + _row];
}

} // Impl

template<class T, size_t rows, size_t columns, bool row_major>
Matrix<T, rows, columns, row_major>::Matrix(Matrix<T, rows, columns, true> const& _m)
{
	*this = _m;
}

template<class T, size_t rows, size_t columns, bool row_major>
Matrix<T, rows, columns, row_major>::Matrix(Matrix<T, rows, columns, false> const& _m)
{
	*this = _m;
}

template<class T, size_t rows, size_t columns, bool row_major>
Matrix<T, rows, columns, row_major>& Matrix<T, rows, columns, row_major>::operator=(Matrix<T, rows, columns, true> const& _m)
{
	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < columns; ++j)
		{
			(*this)(i, j) = _m(i, j);
		}
	}

	return *this;
}

template<class T, size_t rows, size_t columns, bool row_major>
Matrix<T, rows, columns, row_major>& Matrix<T, rows, columns, row_major>::operator=(Matrix<T, rows, columns, false> const& _m)
{
	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < columns; ++j)
		{
			(*this)(i, j) = _m(i, j);
		}
	}

	return *this;
}

template<class T, size_t rows, size_t columns, bool row_major>
T& Matrix<T, rows, columns, row_major>::operator()(size_t _row, size_t _column)
{
	return get(_row, _column);
}

template<class T, size_t rows, size_t columns, bool row_major>
T const& Matrix<T, rows, columns, row_major>::operator()(size_t _row, size_t _column) const
{
	return get(_row, _column);
}

template<class T, size_t rows, size_t columns, bool row_major>
Matrix<T, 1, columns, row_major> Matrix<T, rows, columns, row_major>::get_row(size_t _idx) const
{
	Matrix<T, 1, columns, row_major> result;

	for (size_t i = 0; i < columns; ++i)
	{
		result(0, i) = (*this)(_idx, i);
	}

	return result;
}

template<class T, size_t rows, size_t columns, bool row_major>
template<bool either_major> void Matrix<T, rows, columns, row_major>::set_row(size_t _idx, Matrix<T, 1, columns, either_major> const& _row)
{
	for (size_t i = 0; i < columns; ++i)
	{
		(*this)(_idx, i) = _row(0, i);
	}
}

template<class T, size_t rows, size_t columns, bool row_major>
Matrix<T, rows, 1, row_major> Matrix<T, rows, columns, row_major>::get_column(size_t _idx) const
{
	Matrix<T, rows, 1, row_major> result;

	for (size_t i = 0; i < rows; ++i)
	{
		result(i, 0) = (*this)(i, _idx);
	}

	return result;
}

template<class T, size_t rows, size_t columns, bool row_major>
template<bool either_major> void Matrix<T, rows, columns, row_major>::set_column(size_t _idx, Matrix<T, rows, 1, either_major> const& _column)
{
	for (size_t i = 0; i < rows; ++i)
	{
		(*this)(i, _idx) = _column(i, 0);
	}
}

template<class T, size_t rows, size_t columns, bool row_major>
Matrix<T, columns, rows, row_major> Matrix<T, rows, columns, row_major>::transposed() const
{
	Matrix<T, columns, rows, row_major> result;

	for (size_t i = 0; i < columns; ++i)
	{
		for (size_t j = 0; j < rows; ++j)
		{
			result(i, j) = (*this)(j, i);
		}
	}

	return result;
}

template<class T, size_t rows, size_t columns, bool lhs_row_major, bool rhs_row_major>
bool operator==(Matrix<T, rows, columns, lhs_row_major> const& _lhs, Matrix<T, rows, columns, rhs_row_major> const& _rhs)
{
	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < columns; ++j)
		{
			if (_lhs(i, j) != _rhs(i, j))
			{
				return false;
			}
		}
	}

	return true;
}

template<class T, size_t rows, size_t columns, bool lhs_row_major, bool rhs_row_major>
bool operator!=(Matrix<T, rows, columns, lhs_row_major> const& _lhs, Matrix<T, rows, columns, rhs_row_major> const& _rhs)
{
	return !(_lhs == _rhs);
}

template<class T>
Impl::UnaryMatrixReturnType<T> operator-(T _m)
{
	typedef Matrix<T::ElemType, T::Rows, T::Columns, T::RowMajor> MatrixTypeT;

	for (size_t i = 0; i < T::Rows; ++i)
	{
		for (size_t j = 0; j < T::Columns; ++j)
		{
			static_cast<MatrixTypeT&>(_m)(i, j) = -static_cast<MatrixTypeT const&>(_m)(i, j);
		}
	}

	return _m;
}

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U, typename T::template MyType<Impl::ElemOpReturnType<std::plus<>, T, U>>> operator+(T const& _lhs, U const& _rhs)
{
	typedef Matrix<T::ElemType, T::Rows, T::Columns, T::RowMajor> MatrixTypeT;
	typedef Matrix<U::ElemType, U::Rows, U::Columns, U::RowMajor> MatrixTypeU;
	typedef Matrix<Impl::ElemOpReturnType<std::plus<>, T, U>, T::Rows, T::Columns, T::RowMajor> MatrixTypePlus;

	T::template MyType<Impl::ElemOpReturnType<std::plus<>, T, U>> result;

	for (size_t i = 0; i < T::Rows; ++i)
	{
		for (size_t j = 0; j < T::Columns; ++j)
		{
			static_cast<MatrixTypePlus&>(result)(i, j) = static_cast<MatrixTypeT const&>(_lhs)(i, j) + static_cast<MatrixTypeU const&>(_rhs)(i, j);
		}
	}

	return result;
}

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U, Impl::CheckElemOpReturnT<std::plus<>, T, U>>& operator+=(T& _lhs, U const& _rhs)
{
	typedef Matrix<T::ElemType, T::Rows, T::Columns, T::RowMajor> MatrixTypeT;
	typedef Matrix<U::ElemType, U::Rows, U::Columns, U::RowMajor> MatrixTypeU;

	for (size_t i = 0; i < T::Rows; ++i)
	{
		for (size_t j = 0; j < T::Columns; ++j)
		{
			static_cast<MatrixTypeT&>(_lhs)(i, j) = static_cast<MatrixTypeT const&>(_lhs)(i, j) + static_cast<MatrixTypeU const&>(_rhs)(i, j);
		}
	}

	return _lhs;
}

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U, typename T::template MyType<Impl::ElemOpReturnType<std::minus<>, T, U>>> operator-(T const& _lhs, U const& _rhs)
{
	typedef Matrix<T::ElemType, T::Rows, T::Columns, T::RowMajor> MatrixTypeT;
	typedef Matrix<U::ElemType, U::Rows, U::Columns, U::RowMajor> MatrixTypeU;
	typedef Matrix<Impl::ElemOpReturnType<std::minus<>, T, U>, T::Rows, T::Columns, T::RowMajor> MatrixTypeMinus;

	T::template MyType<Impl::ElemOpReturnType<std::minus<>, T, U>> result;

	for (size_t i = 0; i < T::Rows; ++i)
	{
		for (size_t j = 0; j < T::Columns; ++j)
		{
			static_cast<MatrixTypeMinus&>(result)(i, j) = static_cast<MatrixTypeT const&>(_lhs)(i, j) - static_cast<MatrixTypeU const&>(_rhs)(i, j);
		}
	}

	return result;
}

template<class T, class U>
Impl::BinaryMatrixReturnType<T, U, Impl::CheckElemOpReturnT<std::minus<>, T, U>>& operator-=(T& _lhs, U const& _rhs)
{
	typedef Matrix<T::ElemType, T::Rows, T::Columns, T::RowMajor> MatrixTypeT;
	typedef Matrix<U::ElemType, U::Rows, U::Columns, U::RowMajor> MatrixTypeU;

	for (size_t i = 0; i < T::Rows; ++i)
	{
		for (size_t j = 0; j < T::Columns; ++j)
		{
			static_cast<MatrixTypeT&>(_lhs)(i, j) = static_cast<MatrixTypeT const&>(_lhs)(i, j) - static_cast<MatrixTypeU const&>(_rhs)(i, j);
		}
	}

	return _lhs;
}

template<class T, class U>
Impl::MultiplyWithElementReturnType<T, U> operator*(T _lhs, U _rhs)
{
	_lhs *= _rhs;

	return _lhs;
}

template<class T, class U>
Impl::MultiplyWithElementReturnType<T, U> operator*(U _lhs, T _rhs)
{
	_rhs *= _lhs;

	return _rhs;
}

template<class T, class U>
Impl::MultiplyWithElementReturnType<T, U>& operator*=(T& _lhs, U _rhs)
{
	typedef Matrix<T::ElemType, T::Rows, T::Columns, T::RowMajor> MatrixTypeT;

	for (size_t i = 0; i < T::Rows; ++i)
	{
		for (size_t j = 0; j < T::Columns; ++j)
		{
			static_cast<MatrixTypeT&>(_lhs)(i, j) = static_cast<MatrixTypeT const&>(_lhs)(i, j) * _rhs;
		}
	}

	return _lhs;
}

template<class T, class U>
Impl::MultiplyWithElementReturnType<T, U> operator/(T _lhs, U _rhs)
{
	_lhs /= _rhs;

	return _lhs;
}

template<class T, class U>
Impl::MultiplyWithElementReturnType<T, U>& operator/=(T& _lhs, U _rhs)
{
	typedef Matrix<T::ElemType, T::Rows, T::Columns, T::RowMajor> MatrixTypeT;

	for (size_t i = 0; i < T::Rows; ++i)
	{
		for (size_t j = 0; j < T::Columns; ++j)
		{
			static_cast<MatrixTypeT&>(_lhs)(i, j) = static_cast<MatrixTypeT const&>(_lhs)(i, j) / _rhs;
		}
	}

	return _lhs;
}

template<class T, class U>
Impl::MultiplyWithMatrixReturnType<T, U, typename T::template MyType<Impl::ElemOpReturnType<std::multiplies<>, T, U>, U::Columns>> operator*(T const& _lhs, U const& _rhs)
{
	typedef Matrix<T::ElemType, T::Rows, T::Columns, T::RowMajor> MatrixTypeT;
	typedef Matrix<U::ElemType, U::Rows, U::Columns, U::RowMajor> MatrixTypeU;
	typedef Matrix<Impl::ElemOpReturnType<std::multiplies<>, T, U>, T::Rows, U::Columns, T::RowMajor> MatrixTypeMultiply;

	T::template MyType<Impl::ElemOpReturnType<std::multiplies<>, T, U>, U::Columns> result;

	for (size_t i = 0; i < T::Rows; ++i)
	{
		for (size_t j = 0; j < U::Columns; ++j)
		{
			for (size_t k = 0; k < T::Columns; ++k)
			{
				Impl::ElemOpReturnType<std::multiplies<>, T, U> temp = static_cast<MatrixTypeT const&>(_lhs)(i, k) * static_cast<MatrixTypeU const&>(_rhs)(k, j);
				static_cast<MatrixTypeMultiply&>(result)(i, j) = static_cast<MatrixTypeMultiply const&>(result)(i, j) + temp;
			}
		}
	}

	return result;
}

template<class T, class U>
Impl::SquareMatrixReturnType<T, U, Impl::CheckElemOpReturnT<std::multiplies<>, T, U>>& operator*=(T& _lhs, U const& _rhs)
{
	typedef Matrix<T::ElemType, T::Rows, T::Columns, T::RowMajor> MatrixTypeT;
	typedef Matrix<U::ElemType, U::Rows, U::Columns, U::RowMajor> MatrixTypeU;

	for (size_t i = 0; i < T::Rows; ++i)
	{
		for (size_t j = 0; j < U::Columns; ++j)
		{
			for (size_t k = 0; k < T::Columns; ++k)
			{
				Impl::ElemOpReturnType<std::multiplies<>, T, U> temp = static_cast<MatrixTypeT const&>(_lhs)(i, k) * static_cast<MatrixTypeU const&>(_rhs)(k, j);
				static_cast<MatrixTypeT&>(_lhs)(i, j) = static_cast<MatrixTypeT const&>(_lhs)(i, j) + temp;
			}
		}
	}

	return _lhs;
}

namespace MatHelper {

namespace Impl {

template<class T, size_t n, bool row_major>
Matrix<T, n, 1, row_major> solve_for_x(Matrix<T, n, n, row_major> const& _l, Matrix<T, n, n, row_major> const& _u, Matrix<T, n, 1, row_major> const& _b)
{
	// http://www.gamedev.net/page/resources/_/technical/math-and-physics/matrix-inversion-using-lu-decomposition-r3637

	Matrix<T, n, 1, row_major> d;

	for (size_t i = 0; i < n; ++i) // lower
	{
		T sum(0);

		for (size_t j = 0; j < i; ++j)
		{
			sum = sum + _l(i, j) * d(j, 0);
		}

		d(i, 0) = (_b(i, 0) - sum) / _l(i, i);
	}

	Matrix<T, n, 1, row_major> x;

	for (size_t i = 0; i < n; ++i) // upper
	{
		size_t idx = n - 1 - i;

		T sum(0);

		for (size_t j = idx; j < n; ++j)
		{
			sum = sum + _u(idx, j) * x(j, 0);
		}

		x(idx, 0) = d(idx, 0) - sum;
	}

	return x;
}

} // Impl

template<class T, size_t rows, size_t columns, bool row_major>
Matrix<T, rows, columns, row_major>& make_zero(Matrix<T, rows, columns, row_major>& _m)
{
	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < columns; ++j)
		{
			_m(i, j) = T(0);
		}
	}

	return _m;
}

template<class T, size_t n, bool row_major>
Matrix<T, n, n, row_major>& make_identity(Matrix<T, n, n, row_major>& _m)
{
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			if (i == j)
			{
				_m(i, j) = T(1);
			}
			else
			{
				_m(i, j) = T(0);
			}
		}
	}

	return _m;
}

template<class T, size_t n, bool row_major>
bool decompose(Matrix<T, n, n, row_major> const& _m, Matrix<T, n, n, row_major>& _l, Matrix<T, n, n, row_major>& _u)
{
	// https://en.wikipedia.org/wiki/Crout_matrix_decomposition

	make_zero(_l);
	make_identity(_u);

	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = i; j < n; ++j)
		{
			T sum(0);

			for (size_t k = 0; k < i; ++k)
			{
				sum = sum + _l(j, k) * _u(k, i);
			}

			_l(j, i) = _m(j, i) - sum;
		}

		for (size_t j = i; j < n; ++j)
		{
			T sum(0);

			for (size_t k = 0; k < i; ++k)
			{
				sum = sum + _l(i, k) * _u(k, j);
			}

			if (_l(i, i) == T(0))
			{
				make_zero(_l);
				make_zero(_u);

				return false;
			}

			_u(i, j) = (_m(i, j) - sum) / _l(i, i);
		}
	}

	return true;
}

template<class T, size_t n, bool row_major>
T determinant(Matrix<T, n, n, row_major> const& _m)
{
	Matrix<T, n, n, row_major> l;
	Matrix<T, n, n, row_major> u;

	bool success = decompose(_m, l, u);

	if (!success)
	{
		return T(0);
	}

	T result(1);

	for (size_t i = 0; i < n; ++i)
	{
		result = result * l(i, i);
	}

	return result;
}

template<class T, size_t n, bool row_major>
bool inverse(Matrix<T, n, n, row_major> const& _m, Matrix<T, n, n, row_major>& _inv)
{
	Matrix<T, n, n, row_major> l;
	Matrix<T, n, n, row_major> u;

	bool success = decompose(_m, l, u);

	if (!success)
	{
		make_zero(_inv);

		return false;
	}

	Matrix<T, n, n, row_major> identity;
	make_identity(identity);

	for (size_t i = 0; i < n; ++i)
	{
		_inv.set_column(i, Impl::solve_for_x(l, u, identity.get_column(i)));
	}

	return true;
}

} // MatHelper

} } // SylDev, Math