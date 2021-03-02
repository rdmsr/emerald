/*-
 * SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Copyright (c) 2021 Abb1x
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "PCI.h"
#include "pci_id.h"
#include <libk/logging.h>
PCIDevice *pci_devices;

uint32_t PCI_read_dword(PCIDevice *device, uint8_t reg)
{
    uint32_t bus32 = (uint32_t)device->bus;
    uint32_t device32 = (uint32_t)device->device;
    uint32_t function32 = (uint32_t)device->function;

    uint32_t target = 0x80000000 | (bus32 << 16) | ((device32) << 11) | ((function32) << 8) | (reg & 0xFC);

    IO_outl(0xcf8, target);

    return IO_inl(0xCFC);
}

/* Get stuff from device */

uint16_t get_vendor(PCIDevice *device)
{
    return (uint16_t)(PCI_read_dword(device, 0));
}

uint16_t get_device_id(PCIDevice *device)
{
    return (uint16_t)(((uint32_t)PCI_read_dword(device, 0)) >> 16);
}

uint8_t get_class(PCIDevice *device)
{
    uint8_t class = (uint8_t)(PCI_read_dword(device, 0x8) >> 24);
    return class;
}

uint8_t get_subclass(PCIDevice *device)
{
    uint8_t subclass = (uint8_t)(PCI_read_dword(device, 0x8) >> 16);

    return subclass;
}

uint8_t get_header_type(PCIDevice *device)
{
    uint8_t header_type = (uint8_t)(PCI_read_dword(device, 0xC) >> 16);
    return header_type & ~(1 << 7);
}

uint8_t get_secondary_bus(PCIDevice *device)
{
    return (uint8_t)(PCI_read_dword(device, 0x18) >> 8);
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

uint64_t current_count = 0;

/* FIXME: implement multifunction parsing */
void PCI_scan_bus(uint8_t bus)
{
    log(INFO, "Checking bus %x", (uint32_t)bus);

    uint8_t device;

    for (device = 0; device < 32; device++)
    {
        pci_devices[device].bus = bus;
        pci_devices[device].device = device;
        pci_devices[device].function = 0;

        if (get_vendor(&pci_devices[device]) != 0xFFFF)
        {
            current_count++;
            if (is_bridge(&pci_devices[device])) /* If it's a bridge */
            {

                PCI_scan_bus(get_secondary_bus(&pci_devices[device]));
            }
        }
    }
}

void PCI_init()
{
    module("PCI");

    PCI_scan_bus(0);

    uint64_t device;

    for (device = 0; device < current_count; device++)
    {
        pci_devices[device].class = get_class(&pci_devices[device]);
        pci_devices[device].subclass = get_subclass(&pci_devices[device]);
        pci_devices[device].vendor_id = get_vendor(&pci_devices[device]);
        pci_devices[device].device_id = get_device_id(&pci_devices[device]);

        log(INFO, "Found device with vendor %x, device id: %x", pci_devices[device].vendor_id, pci_devices[device].device_id);

        VBE_putf("[PCI] 00:%x.%d %s: %s %s", device, pci_devices[device].function, PCI_id_to_string(&pci_devices[device]), PCI_vendor_to_string(&pci_devices[device]), PCI_device_id_to_string(&pci_devices[device]));
    }
}

