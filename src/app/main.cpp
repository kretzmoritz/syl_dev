#include <iostream>

#include "../framework/window_desc.h"
#include "window_procedure.h"
#include "../framework/window.h"
#include "../math/matrix.h"
#include "../math/vector.h"

int main()
{
	SylDev::Framework::WindowDesc desc;
	desc.lpWindowName = "My Window";

	SylDev::Framework::Window<SylDev::App::WindowProcedure> Window(desc, "Hello World.");

	SylDev::Math::Matrix<float, 3, 3, true> Matrix;
	Matrix(0, 1) = 1;

	SylDev::Math::Matrix<float, 3, 3, false> Matrix2;
	Matrix2(0, 1) = 1;

	auto Matrix3 = Matrix + Matrix2;
	Matrix += Matrix2;

	std::cout << Matrix3(0, 1) << std::endl;

	SylDev::Math::Vec3f Vec(1.0f, 2.0f, 3.0f);
	SylDev::Math::Vec3f Vec2(4.0f, 5.0f, 6.0f);

	Vec += Vec;
	Vec += Vec2;

	float a = 2.0f;
	Vec = Vec * a;
	Vec *= a;

	std::cout << (-Vec).y << std::endl;

	return 0;
}