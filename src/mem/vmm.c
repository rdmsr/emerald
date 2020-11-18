#include "vmm.h"

void EmeraldMem_VMM_vmm_create_pagemap(pagemap_t pagemap)
{
    //TODO
}

void* get_physical_addr(void* virtualaddr)
{
    //Page directory index
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    //Page Table index
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
    //check if the PD entry is present
    unsigned long* pd = (unsigned long*)0xFFFFF000;
    //check if the PT entry is present
    unsigned long* pt = ((unsigned long*)0xFFC00000) + (0x400 * pdindex);

    return (void*)((pt[ptindex] & ~0xFFF) + ((unsigned long)virtualaddr & 0xFFF));
}
void EmeraldMem_VMM_map_page(void* physaddr, void* virtualaddr, unsigned int flags)
{

    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

    unsigned long* pd = (unsigned long*)0xFFFFF000;

    unsigned long* pt = ((unsigned long*)0xFFC00000) + (0x400 * pdindex);

    pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Bit Present
}