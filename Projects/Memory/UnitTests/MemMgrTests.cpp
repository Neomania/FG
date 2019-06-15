#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Memory/MemoryManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(MemoryManagerTests)
	{
	public:
		
		TEST_METHOD(ConstructorTest)
		{
            ContainedMemory memMgr;
		}

        TEST_METHOD(AllocateTest)
        {
            ContainedMemory memMgr;

            memMgr.Initialize();
            void* memory = memMgr.Allocate(256);
            memMgr.Free(memory);
        }

        TEST_METHOD(AllocateBlocksTest)
        {
            ContainedMemory memMgr;

            memMgr.Initialize();

            int* data[16];

            for (int i = 0; i < 16; ++i)
            {
                data[i] = (int*)memMgr.Allocate(sizeof(int));
                *(data[i]) = i;
            }

            for (int i = 0; i < 16; ++i)
            {
                Assert::IsTrue(*(data[i]) == i);
            }
            //memMgr.Print();

            for (int i = 0; i < 16; ++i)
            {
                memMgr.Free(data[i]);
            }
            //memMgr.Print();

            Assert::IsTrue(memMgr.IsEmpty());
        }
	};
}