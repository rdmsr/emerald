#include "ahci.h"
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
	}
}

/* add sendcmd */

