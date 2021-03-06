/*
 *  Outside World Water Blocks Limit Adjuster
 *  Copyright (C) 2014 LINK/2012 <dma_2012@hotmail.com>
 *  Licensed under the MIT License (http://opensource.org/licenses/MIT)
 */
#include "LimitAdjuster.h"
#include <stdexcept>
 
// Assembly related stuff
extern "C" void ASM_OutsideWorldWaterBlocks();
extern "C" int  LIMIT_OutsideWorldWaterBlocks;
extern "C" void* ASM_OutsideWorldWaterBlocks_ret1;
extern "C" void* ASM_OutsideWorldWaterBlocks_ret2;

 
/*
 *  Key: OutsideWorldWaterBlocks
 *  Value: Integer, Maximum value ~500
 *  Game: SA
 */
struct OutsideWorldWaterBlocks : public SimpleAdjuster
{
    uint16_t *a, *aX, *aY;
    
    // Constructs initializing array to null
    OutsideWorldWaterBlocks() : a(0)
    {}
    
    // Destructs deleting array
    ~OutsideWorldWaterBlocks()
    { delete[] a; }
    
    // Limit Name
    const char* GetLimitName()
    {
        return (GetGVM().IsSA()? "OutsideWorldWaterBlocks" : nullptr);
    }

    // Sets the water blocks limit
    void ChangeLimit(int, const std::string& value)
    {
        using namespace injector;
    
        auto limit = std::stoi(value);
        
        // This maximum value for the limit is related to the maximum number of vertices/indices the temporary vertices/indices array
        // for immediate mode can handle...
        // If this value is too high, it will end up overwriting game info at .data segment, after the temporary immediate arrays
        if(limit > 500) throw std::runtime_error("Maximum value for OutsideWorldWaterBlocks is 500");
        
        // Allocate array to store X and Y position of the outside world water blocks
        this->a  = new uint16_t[limit * 2];
	    this->aX = &a[0];
		this->aY = &a[limit];
		LIMIT_OutsideWorldWaterBlocks = limit;

        // Patch the necessary references
        MakeJMP(0x6E6CE9, raw_ptr(ASM_OutsideWorldWaterBlocks));
        WriteMemory(0x6E6CEE + 0x4, aX, true);
        WriteMemory(0x6EF6E0 + 0x4, aX, true);
        WriteMemory(0x6EFE82 + 0x4, aX, true);
        WriteMemory(0x6E6CF6 + 0x4, aY, true);
        WriteMemory(0x6EF6E8 + 0x4, aY, true);
        WriteMemory(0x6EFE95 + 0x4, aY, true);
        WriteMemory(0x6EFEAF + 0x4, aY, true);
        
        // Find real offsets for the assembly hook
        ASM_OutsideWorldWaterBlocks_ret1 = memory_pointer(0x6E6CEE).get();
        ASM_OutsideWorldWaterBlocks_ret2 = memory_pointer(0x6E6D04).get();
    }
    

// Instantiate the adjuster on the global scope
} adjuster_OutsideWorldWaterBlocks;



/*
 *  Assembly patches
 */

int LIMIT_OutsideWorldWaterBlocks;
void* ASM_OutsideWorldWaterBlocks_ret1;
void* ASM_OutsideWorldWaterBlocks_ret2;

#if defined(_MSC_VER)

void __declspec(naked) ASM_OutsideWorldWaterBlocks()
{
    __asm
    {
	        cmp     eax, LIMIT_OutsideWorldWaterBlocks
	        jge     _Full
	        push    ASM_OutsideWorldWaterBlocks_ret1
	        ret
        _Full:
            push    ASM_OutsideWorldWaterBlocks_ret2
            ret
	}
}


#else

__asm__(R"(
    .intel_syntax noprefix
    .globl _ASM_OutsideWorldWaterBlocks
    .globl _LIMIT_OutsideWorldWaterBlocks
    .globl _ASM_OutsideWorldWaterBlocks_ret1
    .globl _ASM_OutsideWorldWaterBlocks_ret2
    

    _ASM_OutsideWorldWaterBlocks:
	        cmp     eax, _LIMIT_OutsideWorldWaterBlocks
	        jge     _Full
	        push    _ASM_OutsideWorldWaterBlocks_ret1
	        ret
        _Full:
            push    _ASM_OutsideWorldWaterBlocks_ret2
            ret
    
    .att_syntax prefix
)");

#endif


