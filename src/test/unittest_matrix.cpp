#include "../common/unit_testing.h"

#include "../math/matrix.h"

SYLDEV_TESTSUITE(Matrix)
{
	SYLDEV_UNITTEST(Addition)
	{
		SylDev::Math::Matrix<float, 2, 2, true> MatrixA;
		SylDev::Math::MatHelper::make_identity(MatrixA);

		SylDev::Math::Matrix<int, 2, 2, false> MatrixB;
		SylDev::Math::MatHelper::make_identity(MatrixB);

		SylDev::Math::Matrix<float, 2, 2, true> MatrixC;
		MatrixC(0, 0) = 2.0f;
		MatrixC(0, 1) = 0.0f;
		MatrixC(1, 0) = 0.0f;
		MatrixC(1, 1) = 2.0f;

		SYLDEV_UNITTEST_ASSERT(MatrixA + MatrixB == MatrixC);
	}

	SYLDEV_UNITTEST(Subtraction)
	{
		SylDev::Math::Matrix<float, 2, 2, true> MatrixA;
		SylDev::Math::MatHelper::make_identity(MatrixA);

		SylDev::Math::Matrix<int, 2, 2, false> MatrixB;
		SylDev::Math::MatHelper::make_identity(MatrixB);

		SylDev::Math::Matrix<float, 2, 2, true> MatrixC;
		SylDev::Math::MatHelper::make_zero(MatrixC);

		SYLDEV_UNITTEST_ASSERT(MatrixA - MatrixB == MatrixC);
	}
}