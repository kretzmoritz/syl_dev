#include "../common/unit_testing.h"

#include "../math/vector.h"

SYLDEV_TESTSUITE(Vector)
{
	SYLDEV_TESTSUITE_DEPEND(Matrix)

	SYLDEV_UNITTEST(Addition)
	{
		SylDev::Math::Vector<float, 3> VectorA(1, 2, 3);
		SylDev::Math::Vector<float, 3> VectorB(1, 2, 3);
		SylDev::Math::Vector<float, 3> VectorC(2, 4, 6);

		SYLDEV_UNITTEST_ASSERT(VectorA + VectorB == VectorC);
	}

	SYLDEV_UNITTEST(Subtraction)
	{
		SylDev::Math::Vector<float, 3> VectorA(1, 2, 3);
		SylDev::Math::Vector<float, 3> VectorB(1, 2, 3);
		SylDev::Math::Vector<float, 3> VectorC(0, 0, 0);

		SYLDEV_UNITTEST_ASSERT(VectorA - VectorB == VectorC);
	}
}