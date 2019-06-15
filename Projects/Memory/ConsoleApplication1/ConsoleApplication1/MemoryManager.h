#pragma once

/*

To ensure full state restoration, objects in contained memory cannot have pointers.

*/

#ifdef USE_INDIRECT_POINTERS
template <typename T>
class CMPtr
{
    unsigned offset;
    T* debug_ptr;
};
#else
template <typename T>
using CMPtr = T * ;
#endif

#define MEM_INTEGRITY_CHECK
class ContainedMemory
{
private:
    struct MemoryBlock
    {
#ifdef MEM_INTEGRITY_CHECK
        int integrity;
#endif
        bool used;
        unsigned size;
        MemoryBlock* nextBlock;
        MemoryBlock* prevBlock;
    };

    void* memoryBlock;
public:
    ContainedMemory();

    void Initialize();

    void* Allocate(unsigned size);
    void Free(void* data);
};