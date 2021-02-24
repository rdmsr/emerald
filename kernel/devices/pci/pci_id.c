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

#include "pci_id.h"
#include <libk/logging.h>
char *PCI_id_to_string(PCIDevice *device)
{
    switch (device->class)
    {
    case 0:
        return "Unknown";
    case 1:
    {
        switch (device->subclass)
        {
        case 0:
            return "SCSI storage controller";
        case 1:
            return "IDE interface";
        case 2:
            return "Floppy disk controller";
        case 3:
            return "IPI bus controller";
        case 4:
            return "RAID bus controller";
        case 5:
            return "ATA controller";
        case 6:
            return "SATA controller";
        case 7:
            return "Serial Attached SCSI controller";
        case 8:
            return "Non-Volatile memory controller";
        default:
            return "Mass Storage Controller";
        }
    }

    case 2:
    {
        switch (device->subclass)
        {
        case 0:
            return "Ethernet controller";
        case 1:
            return "Token ring network controller";
        case 2:
            return "FDDI network controller";
        case 3:
            return "ATM network controller";
        case 4:
            return "ISDN controller";
        case 5:
            return "WorldFip controller";
        case 6:
            return "PICMG controller";
        case 7:
            return "Infiniband controller";
        case 8:
            return "Fabric controller";
        default:
            return "Network controller";
        }
    }

    case 3:
        switch (device->subclass)
        {
        case 0:
            return "VGA compatible controller";
        case 1:
            return "XGA compatible controller";
        case 2:
            return "3D controller";
        default:
            return "Display controller";
        }
    case 4:
        switch (device->subclass)
        {
        case 0:
            return "Multimedia video controller";
        case 1:
            return "Multimedia audio controller";
        case 2:
            return "Computer telephony device";
        case 3:
            return "Audio device";
        default:
            return "Multimedia controller";
        }
    case 5:
        switch (device->subclass)
        {
        case 0:
            return "RAM memory";
        case 1:
            return "FLASH memory";
        default:
            return "Memory controller";
        }
    case 6:
        switch (device->subclass)
        {
        case 0:
            return "Host bridge";
        case 1:
            return "ISA bridge";
        case 2:
            return "EISA bridge";
        case 3:
            return "MicroChannel bridge";
        case 4:
            return "PCI bridge";
        case 5:
            return "PCMCIA bridge";
        case 6:
            return "NuBus bridge";
        case 7:
            return "CardBus bridge";
        case 8:
            return "RACEway bridge";
        case 9:
            return "Semi-transparent PCI-to-PCI bridge";
        case 0xa:
            return "InfiniBand to PCI host bridge";
        default:
            return "Bridge";
        }
    case 7:
        switch (device->subclass)
        {
        case 0:
            return "Serial controller";
        case 1:
            return "Parallel controller";
        case 2:
            return "Multiport serial controller";
        case 3:
            return "Modem";
        case 4:
            return "GPIB controller";
        case 5:
            return "Smard Card controller";
        default:
            return "Communication controller";
        }
    case 8:
        switch (device->subclass)
        {

        case 0:
            return "PIC";
        case 1:
            return "DMA controller";
        case 2:
            return "Timer";
        case 3:
            return "RTC";
        case 4:
            return "PCI Hot-plug controller";
        case 5:
            return "SD Host controller";
        case 6:
            return "IOMMU";
        default:
            return "System peripheral";
        }
    case 9:
        switch (device->subclass)
        {
        case 0:
            return "Keyboard controller";
        case 1:
            return "Digitizer Pen";
        case 2:
            return "Mouse controller";
        case 3:
            return "Scanner controller";
        case 4:
            return "Gameport controller";
        default:
            return "Input device controller";
        }
    case 0xa:
        switch (device->subclass)
        {
        case 0:
            return "Generic Docking Station";
        default:
            return "Docking Station";
        }
    case 0xb:
        switch (device->subclass)
        {
        case 0:
            return "386";
        case 1:
            return "486";
        case 2:
            return "Pentium";
        case 10:
            return "Alpha";
        case 20:
            return "Power PC";
        case 30:
            return "MIPS";
        case 40:
            return "Co-processor";
        default:
            return "Processor";
        }
    case 0xc:
        switch (device->subclass)
        {
        case 0:
            return "FireWire (IEEE 1394)";
        case 1:
            return "ACCESS Bus";
        case 2:
            return "SSA";
        case 3:
            return "USB controller";
        case 4:
            return "Fibre Channel";
        case 5:
            return "SMBus";
        case 6:
            return "InfiniBand";
        case 7:
            return "IPMI Interface";
        case 8:
            return "SERCOS interface";
        case 9:
            return "CANBUS";
        default:
            return "Serial bus controller";
        }
    case 0xd:
        switch (device->subclass)
        {
        case 0:
            return "IRDA controller";
        case 1:
            return "Consumer IR controller";
        case 10:
            return "RF controller";
        case 11:
            return "Bluetooth";
        case 12:
            return "Broadband";
        case 20:
            return "802.1a controller";
        case 21:
            return "802.1b controller";
        default:
            return "Wireless controller";
        }
    case 0xe:
        switch (device->subclass)
        {
        case 0:
            return "I2O";
        default:
            return "Intelligent controller";
        }
    default:
        return "Unknown device";
    }
}

char *PCI_vendor_to_string(PCIDevice *device)
{
    switch (device->vendor_id)
    {
    case 0x1002:
        return "Advanced Micro Devices (AMD) inc.";
    case 0x8086:
        return "Intel Corporation";
    case 0x168C:
        return "Qualcomm Atheros";
    case 0x10DE:
        return "NVIDIA";
    case 0x10ec:
        return "Realtek Semiconductor Co., Ltd.";
    case 0x1a58:
        return "Razer USA Ltd.";
    case 0x1014:
        return "IBM";
    case 0x1414:
        return "Microsoft Corporation";
    case 0x1234:
        return "Technical Corp.";

    case 0x1AE0:
        return "Google, Inc.";
    case 0x80EE:
        return "Oracle Corporation";
    case 0x1af4:
        return "Red Hat";
    default:
        return "Unknown vendor";
    }
}

char *PCI_device_id_to_string(PCIDevice *device)
{
    switch (device->vendor_id)
    {

    /* Intel */
    case 0x8086:
        switch (device->device_id)
        {
        case 0x100E:
            return "Gigabit Ethernet Controller";
        case 0x153A:
            return "Ethernet Connection I217-LM";
        case 0x10EA:
            return "Gigabit Network Connection";
        }
    /* Realtek */
    case 0x10EC:
        switch (device->device_id)
        {
        case 0x8139:
            return "RTL-8100/8101L/8139 PCI Fast Ethernet Adapter";
        }
    /* QEMU */
    case 0x1234:
        switch (device->device_id)
        {
        case 0x1111:
            return "QEMU Virtual Video Controller";
        }
    /* VirtualBox */
    case 0x80EE:
        switch (device->device_id)
        {
        case 0xBEEF:
            return "VirtualBox Graphics Adapter";
        case 0xCAFE:
            return "VirtualBox Guest Service";
        }

    default:
        return "Unknown device";
    }
}
