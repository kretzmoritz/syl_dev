#include "matrix.h"

#include <assert.h>

BEGIN_2_NAMESPACES(SylDev, Math)

BEGIN_NAMESPACE(Impl)

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

END_NAMESPACE

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
Impl::BinaryMatrixReturnType<T, U, Impl::CheckElemOpReturnT<std::multiplies<>, T, U>>& operator*=(T& _lhs, U const& _rhs)
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

END_2_NAMESPACES