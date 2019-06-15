#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(MemoryManager)
	{
	public:
		
		TEST_METHOD(ConstructionTest)
		{
            Assert::IsTrue(2 + 2 == 4);
		}

	};
}