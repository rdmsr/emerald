#include "ahci.h"
#include <memory/pmm.h>
#include <libk/logging.h>
#include <libk/module.h>

void ahci_init(PCIDevice *ahci_device)
{
	module("AHCI");
	/* Just checks the device for now */
	switch(ahci_device->prog_if)
	{
		case 0:
			log(DEBUG, "Detected a vendor specific interface (get a new pc duh)");
			return;
		case 1:
			log(INFO, "Detected AHCI compliant device");
			break;
		case 2:
			log(DEBUG, "Detected a serial storage bus");
			return;
	}

	PCI_set_bus_master(ahci_device);

	PCIBar bar;
	if (PCI_get_bar(ahci_device, &bar, 5) == 1) return;
	log(INFO, "Bar size: %d %x, bar base: %d %x", bar.size, bar.size, bar.base, bar.base);	
	/* volatile HBA_mem *hba_mem = (volatile HBA_mem*)((size_t)bar.base + MEM_OFFSET);
	stuff to be added */
}


