#ifndef JOS_KERN_E1000_H
#define JOS_KERN_E1000_H

#include <kern/pci.h>

/* 82540EM IDs */
#define E1000_VEN_ID_82540EM             0x8086
#define E1000_DEV_ID_82540EM             0x100E

/* Registers */
#define E1000_STATUS   0x00008/4  /* Device Status - RO */


int e1000_attach (struct pci_func *pcif);

#endif	// JOS_KERN_E1000_H
