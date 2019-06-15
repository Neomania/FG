#include "MemoryManager.h"
#include <exception>
#include <iostream>

#define MEMORY_SIZE 1024 * 1024

#ifdef MEM_INTEGRITY_CHECK
#define MEM_INTEGRITY_MAGIC_NUM 1234567890
#endif

//#define DUMMY_IMPLEMENTATION


ContainedMemory::ContainedMemory()
{
    memoryBlock = new char[MEMORY_SIZE];
    if (!memoryBlock)
    {
        throw std::bad_alloc();
    }
}

ContainedMemory::~ContainedMemory()
{
    delete[] memoryBlock;
}

void ContainedMemory::Initialize()
{
    //reset memory
    MemoryBlock* startBlock = reinterpret_cast<MemoryBlock*>(memoryBlock);
    startBlock->nextBlock = nullptr;
    startBlock->used = false;
    startBlock->size = MEMORY_SIZE - sizeof(MemoryBlock);
    startBlock->prevBlock = nullptr;

#ifdef MEM_INTEGRITY_CHECK
    startBlock->integrity = MEM_INTEGRITY_MAGIC_NUM;
#endif
}

void * ContainedMemory::Allocate(unsigned size)
{
#ifdef DUMMY_IMPLEMENTATION
    return new char[size];
#else
    //dumb search for suitable block
    MemoryBlock* block = reinterpret_cast<MemoryBlock*>(memoryBlock);
    while (block)
    {
        if (block->used == false && block->size >= size)
        {
            //suitable block found
            if (block->size > size + sizeof(MemoryBlock))
            {
                //block does not fit exactly enough, need to create new block for remainder
                MemoryBlock* newBlock = reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(block) + sizeof(MemoryBlock) + size);
                newBlock->size = block->size - (size + sizeof(MemoryBlock));
                newBlock->prevBlock = block;
                newBlock->nextBlock = block->nextBlock;
                newBlock->used = false;

#ifdef MEM_INTEGRITY_CHECK
                newBlock->integrity = MEM_INTEGRITY_MAGIC_NUM;
#endif

                block->nextBlock = newBlock;
                block->size = size;
                block->used = true;

                //linking done, return new block
                return reinterpret_cast<char*>(block) + sizeof(MemoryBlock);
            }
            else
            {
                block->used = true;
                return reinterpret_cast<char*>(block) + sizeof(MemoryBlock);
            }
        }
        else
        {
            block = block->nextBlock;
        }
    }

    //could not find a block
    throw std::bad_alloc();
#endif
}

void ContainedMemory::Free(void * data)
{
#ifdef DUMMY_IMPLEMENTATION
    delete[] data;
    return;
#endif
    MemoryBlock* block = reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(data) - sizeof(MemoryBlock));

#ifdef MEM_INTEGRITY_CHECK
    if (block->integrity != MEM_INTEGRITY_MAGIC_NUM)
    {
        throw std::bad_alloc();
    }
#endif
    //there should not be more than one free block ahead and behind, at best
    if (block->prevBlock && block->prevBlock->used == false)
    {
        //valid previous block to link to
        block->prevBlock->size += block->size + sizeof(MemoryBlock);
        block->prevBlock->nextBlock = block->nextBlock;
        if (block->nextBlock) block->nextBlock->prevBlock = block->prevBlock;

        block = block->prevBlock;
    }

    if (block->nextBlock && block->nextBlock->used == false)
    {
        //valid next block to link to
        block->size += block->nextBlock->size + sizeof(MemoryBlock);

        block->nextBlock = block->nextBlock->nextBlock;
        //free nextblock
        if (block->nextBlock)
        {
            block->nextBlock->prevBlock = block;
        }
    }

    block->used = false;
    //should be done :D
}

bool ContainedMemory::IsEmpty()
{
#ifdef DUMMY_IMPLEMENTATION
    return true;
#endif

    MemoryBlock* startBlock = reinterpret_cast<MemoryBlock*>(memoryBlock);
#ifdef MEM_INTEGRITY_CHECK
    if (startBlock->integrity != MEM_INTEGRITY_MAGIC_NUM) return false;
#endif
    return (startBlock->prevBlock == nullptr) && (startBlock->nextBlock == nullptr) && (startBlock->size == MEMORY_SIZE - sizeof(MemoryBlock)) && (startBlock->used == false);
}

void ContainedMemory::Print()
{
    MemoryBlock* block = reinterpret_cast<MemoryBlock*>(memoryBlock);
    while (block)
    {
        std::cout << "==========================" << std::endl;
        std::cout << "In use:" << block->used << std::endl;
        std::cout << "Size: " << block->size << std::endl;
        block = block->nextBlock;
    }
}
