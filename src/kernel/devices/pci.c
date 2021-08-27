/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "pci.h"
#include <emerald/ds/vec.h>

static vec_t(PCIDevice) devices;

uint32_t pci_raw_read(PCIDevice *device, uint8_t reg)
{
    uint32_t bus32 = (uint32_t)device->bus;
    uint32_t device32 = (uint32_t)device->device_id;
    uint32_t function32 = (uint32_t)device->function;

    uint32_t target = 0x80000000 | (bus32 << 16) | ((device32) << 11) | ((function32) << 8) | (reg & 0xFC);

    asm_outd(0xcf8, target);

    return asm_ind(0xCFC);
}

static uint16_t get_vendor(PCIDevice *device)
{
    return (uint16_t)(pci_raw_read(device, 0));
}

uint16_t get_device_id(PCIDevice *device)
{
    return (uint16_t)(((uint32_t)pci_raw_read(device, 0)) >> 16);
}

uint8_t get_class(PCIDevice *device)
{
    uint8_t class = (uint8_t)(pci_raw_read(device, 0x8) >> 24);
    return class;
}

uint8_t get_subclass(PCIDevice *device)
{
    uint8_t subclass = (uint8_t)(pci_raw_read(device, 0x8) >> 16);

    return subclass;
}

uint8_t get_header_type(PCIDevice *device)
{
    uint8_t header_type = (uint8_t)(pci_raw_read(device, 0xC) >> 16);

    return header_type & ~(1 << 7);
}

uint8_t get_secondary_bus(PCIDevice *device)
{
    return (uint8_t)(pci_raw_read(device, 0x18) >> 8);
}

uint8_t is_bridge(PCIDevice *device)
{
    if (get_header_type(device) != 0x1)
        return 0;
    if (get_class(device) != 0x6)
        return 0;
    if (get_subclass(device) != 0x4)
        return 0;

    return 1;
}

// FIXME: implement multifunction parsing
void pci_scan_bus(uint8_t bus)
{
    log("Checking bus {}", (uint32_t)bus);

    for (u8 device = 0; device < 32; device++)
    {
        PCIDevice new_dev;

        new_dev.bus = bus;
        new_dev.device_id = device;
        new_dev.function = 0;

        if (get_vendor(&new_dev) != 0xFFFF)
        {
            if (is_bridge(&new_dev))
            {
                pci_scan_bus(get_secondary_bus(&new_dev));
            }

            vec_push(&devices, new_dev);
        }
    }
}

void pci_init()
{

    vec_init(&devices);

    pci_scan_bus(0);

    int device;

    for (device = 0; device < devices.length; device++)
    {
        auto dev = devices.data[device];
        dev.class = get_class(&devices.data[device]);
        dev.subclass = get_subclass(&devices.data[device]);
        dev.vendor_id = get_vendor(&devices.data[device]);
        dev.device_id = get_device_id(&devices.data[device]);

        log("Found device with vendor {x}, device id: {x}, class: {x}, subclass: {x}", dev.vendor_id, dev.device_id, dev.class, dev.subclass);
    }
}
