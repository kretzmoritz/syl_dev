#include <iostream>

#include "../common/unit_testing.h"
#include "../framework/window_desc.h"
#include "../framework/window.h"
#include "window_procedure.h"
#include "../math/matrix.h"
#include "../math/vector.h"

// In progress:
// - Unit testing

// To do:
// - Input system
// - Quaternions

class Foo
{
};

class FooOp
{
public:
	FooOp operator+(float _rhs) const { return FooOp(); }
	float operator*(float _rhs) const { return 1.0; }
};

SYLDEV_TESTSUITE(mysuite2)
{
	SYLDEV_TESTSUITE_DEPEND(mysuite3)

	SYLDEV_UNITTEST(myfirsttest)
	{
		int a = 0;
		SYLDEV_UNITTEST_ASSERT(1 + 2 != 3);
	}

	SYLDEV_UNITTEST(mysecondtest)
	{
	}
}

SYLDEV_TESTSUITE(mysuite)
{
	SYLDEV_TESTSUITE_DEPEND(mysuite2)
	SYLDEV_TESTSUITE_DEPEND(mysuite2222222)

	SYLDEV_TESTSUITE_INIT
	{
		int a = 0;
	}

	SYLDEV_TESTSUITE_FIXTURE_ENTER
	{
		int a = 0;
	}

	SYLDEV_TESTSUITE_FIXTURE_LEAVE
	{
		int a = 0;
	}

	SYLDEV_UNITTEST(myfirsttest)
	{
		int a = 0;
		SYLDEV_UNITTEST_ASSERT(1 + 2 == 3);
	}

	SYLDEV_UNITTEST(mysecondtest)
	{
	}

	SYLDEV_TESTSUITE_CLOSE
	{
		int a = 0;
	}
}

SYLDEV_TESTSUITE(mysuite3)
{
	SYLDEV_TESTSUITE_DEPEND(mysuite)

	SYLDEV_UNITTEST(myfirsttest)
	{
		int a = 0;
		SYLDEV_UNITTEST_ASSERT(1 + 2 != 3);
	}

	SYLDEV_UNITTEST(mysecondtest)
	{
	}
}

SYLDEV_TESTSUITE(mysuite4)
{
	SYLDEV_TESTSUITE_DEPEND(mysuite5)

	SYLDEV_UNITTEST(myfirsttest)
	{
		int a = 0;
		SYLDEV_UNITTEST_ASSERT(1 + 2 != 3);
	}

	SYLDEV_UNITTEST(mysecondtest)
	{
	}
}

SYLDEV_TESTSUITE(mysuite5)
{
	SYLDEV_TESTSUITE_DEPEND(mysuite4)

	SYLDEV_UNITTEST(myfirsttest)
	{
		int a = 0;
		SYLDEV_UNITTEST_ASSERT(1 + 2 != 3);
	}

	SYLDEV_UNITTEST(mysecondtest)
	{
	}
}

int main()
{
	auto testresult = SylDev::Common::TestEnvironment::GetInstance().Run();

	SylDev::Framework::WindowCreationResult result;
	SylDev::Framework::WindowClassDesc classDesc;
	SylDev::Framework::WindowDesc wndDesc;

	SylDev::Framework::Window<SylDev::App::WindowProcedure> Window(result, classDesc, wndDesc);

	wndDesc.lpWindowName = "SylWindow2";
	SylDev::Framework::Window<SylDev::App::WindowProcedure> Window2(result, classDesc, wndDesc);

	SylDev::Math::Matrix<int, 3, 3, true> Matrix;
	Matrix(0, 0) = 1;
	Matrix(0, 1) = 1;
	Matrix(0, 2) = 1;
	Matrix(1, 0) = 1;
	Matrix(1, 1) = 1;
	Matrix(1, 2) = 1;
	Matrix(2, 0) = 1;
	Matrix(2, 1) = 1;
	Matrix(2, 2) = 1;

	SylDev::Math::Matrix<float, 2, 2, true> Matrix2;
	Matrix2(0, 0) = 2;
	Matrix2(0, 1) = 1;
	Matrix2(1, 0) = -1;
	Matrix2(1, 1) = 3;
	SylDev::Math::Matrix<float, 2, 2, true> Matrix2inv;

	auto val = SylDev::Math::MatHelper::inverse(Matrix2, Matrix2inv);
	auto testident = Matrix2 * Matrix2inv;

	//auto Matrix3 = Matrix + Matrix2;
	//Matrix += Matrix2;

	SylDev::Math::MatHelper::make_identity(Matrix2);

	SylDev::Math::Matrix<std::string, 3, 3, false> MatrixString;

	//std::cout << Matrix3(0, 1) << std::endl;

	SylDev::Math::Vec3f Vec(1.0f, 2.0f, 3.0f);
	SylDev::Math::Vector<int, 3> Vec2(1, 2, 3);

	Vec += Vec;
	Vec += Vec2;

	auto vectest = Vec2 + Vec;

	SylDev::Math::Matrix<int, 1, 3, false> Matrix4;

	Vec += Matrix4;
	auto mat = Matrix4 + Vec;
	//auto vec = Vec + Matrix4;

	float a = 2.0f;
	Vec = Vec * a;
	Vec *= a;

	Vec = Vec / 2.0f;
	Vec /= 2.0f;

	std::cout << (-Vec).y << std::endl;

	SylDev::Math::Matrix<SylDev::Math::Matrix<float, 4, 3, true>, 3, 4, false> Mult1;
	SylDev::Math::Matrix<SylDev::Math::Matrix<int, 3, 4, false>, 4, 3, true> Mult2;

	auto MultResult = Mult1 * Mult2;

	auto sss = Vec * 1.0f;

	return 0;
}