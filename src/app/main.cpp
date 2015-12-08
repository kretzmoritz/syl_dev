#include <iostream>

#include "../framework/window_desc.h"
#include "window_procedure.h"
#include "../framework/window.h"
#include "../math/matrix.h"
#include "../math/vector.h"

class Foo
{
};

class FooOp
{
public:
	FooOp operator+(float _rhs) const { return FooOp(); }
};

int main()
{
	SylDev::Framework::WindowDesc desc;
	desc.lpWindowName = "My Window";

	SylDev::Framework::Window<SylDev::App::WindowProcedure> Window(desc, "Hello World.");

	SylDev::Math::Matrix<int, 3, 3, true> Matrix;
	Matrix(0, 1) = 1;

	SylDev::Math::Matrix<float, 3, 3, false> Matrix2;
	Matrix2(0, 1) = 1;

	auto Matrix3 = Matrix + Matrix2;
	//Matrix += Matrix2;

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

	std::cout << (-Vec).y << std::endl;

	return 0;
}