#ifndef AHCI_H
#define AHCI_H
#include <devices/pci/PCI.h>

#define SATA_ATA 0x101
#define SATA_ATAPI 0xeb140101
#define SATA_PM 0xc33C0101

typedef enum
{
	FIS_TYPE_REG_H2D	= 0x27,
	FIS_TYPE_REG_D2H	= 0x34,
	FIS_TYPE_DMA_ACT	= 0x39,
	FIS_TYPE_DMA_SETUP	= 0x41,
	FIS_TYPE_DATA		= 0x46,
	FIS_TYPE_BIST		= 0x58,
	FIS_TYPE_PIO_SETUP	= 0x5F,
	FIS_TYPE_DEV_BITS	= 0xA1,
} FIS_TYPE;

typedef volatile struct HBA_port
{
	uint32_t clb;		
	uint32_t clbu;		
	uint32_t fb;		
	uint32_t fbu;		
	uint32_t is;		
	uint32_t ie;		
	uint32_t cmd;		
	uint32_t rsv0;		
	uint32_t tfd;		
	uint32_t sig;		
	uint32_t ssts;		
	uint32_t sctl;		
	uint32_t serr;		
	uint32_t sact;		
	uint32_t ci;		
	uint32_t sntf;		
	uint32_t fbs;		
	uint32_t rsv1[11];	
	uint32_t vendor[4];	
} HBA_port;

typedef volatile struct tag_HBA_mem
{
	uint32_t cap;		
	uint32_t ghc;		
	uint32_t is;		
	uint32_t pi;		
	uint32_t vs;		
	uint32_t ccc_ctl;	
	uint32_t ccc_pts;	
	uint32_t em_loc;	
	uint32_t em_ctl;	
	uint32_t cap2;		
	uint32_t bohc;		
 
	uint8_t  rsv[0xA0-0x2C];
 
	uint8_t  vendor[0x100-0xA0];
 
	HBA_port	ports[];
} HBA_mem;

void ahci_init(PCIDevice *device);

#endif

