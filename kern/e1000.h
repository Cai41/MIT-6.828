#ifndef JOS_KERN_E1000_H
#define JOS_KERN_E1000_H

#include <kern/pci.h>
#include <inc/memlayout.h>

/* 82540EM IDs */
#define E1000_VEN_ID_82540EM             0x8086
#define E1000_DEV_ID_82540EM             0x100E

/* Registers */
#define E1000_STATUS   0x00008/4  /* Device Status - RO */
#define E1000_TCTL     0x00400/4  /* TX Control - RW */
#define E1000_TIPG     0x00410/4  /* TX Inter-packet gap -RW */
#define E1000_TDBAL    0x03800/4  /* TX Descriptor Base Address Low - RW */
#define E1000_TDBAH    0x03804/4  /* TX Descriptor Base Address High - RW */
#define E1000_TDLEN    0x03808/4  /* TX Descriptor Length - RW */
#define E1000_TDH      0x03810/4  /* TX Descriptor Head - RW */
#define E1000_TDT      0x03818/4  /* TX Descripotr Tail - RW */

/* Transmit Control */
#define E1000_TCTL_RST    0x00000001    /* software reset */
#define E1000_TCTL_EN     0x00000002    /* enable tx */
#define E1000_TCTL_BCE    0x00000004    /* busy check enable */
#define E1000_TCTL_PSP    0x00000008    /* pad short packets */
#define E1000_TCTL_CT     0x00000ff0    /* collision threshold */
#define E1000_TCTL_COLD   0x003ff000    /* collision distance */
#define E1000_TCTL_SWXOFF 0x00400000    /* SW Xoff transmission */
#define E1000_TCTL_PBE    0x00800000    /* Packet Burst Enable */
#define E1000_TCTL_RTLC   0x01000000    /* Re-transmit on late collision */
#define E1000_TCTL_NRTU   0x02000000    /* No Re-transmit on underrun */
#define E1000_TCTL_MULR   0x10000000    /* Multiple request support */

#define E1000_TCTL_COLD_FDUP   0x40 << 12

/* Transmit TIPG */
#define E1000_TIPG_IPGT_DEFAULT         0xA
#define E1000_TIPG_IPGR1_DEFAULT        0x4 << 10
#define E1000_TIPG_IPGR2_DEFAULT        0x6 << 20

/* Trasmit Queue and Buffer */
#define E1000_TDNUM                      0x20
#define TX_BUFFER_SIZE                   PGSIZE/2

/* Transmit Descriptor */
struct tx_desc
{
	uint64_t  addr;
	uint16_t  length;
	uint8_t   cso;
	uint8_t   cmd;
	uint8_t   status;
	uint8_t   css;
	uint16_t  special;
};
#define E1000_TXD_CMD_RS    0x8
#define E1000_TXD_CMD_EOP   0x1
#define E1000_TXD_STA_DD    0x1

/* Transmit Buffer */
struct tx_buffer
{
	char buf[TX_BUFFER_SIZE];
};

int e1000_attach (struct pci_func *pcif);
int transmit_packet(void *va, size_t len);
#endif	// JOS_KERN_E1000_H
