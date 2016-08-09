#ifndef JOS_KERN_E1000_H
#define JOS_KERN_E1000_H

#include <kern/pci.h>
#include <inc/memlayout.h>

/* MAC address */
#define MAC_LOW_ADDR                     0x12005452
#define MAC_HIGH_ADDR                    0x00005634

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

#define E1000_RAL      0x05400/4  /* Receive Address - RW Array */
#define E1000_RAH      0x05404/4  /* Receive Address - RW Array */
#define E1000_MTA      0x05200/4  /* Multicast Table Array - RW Array */
#define E1000_ICR      0x000C0/4  /* Interrupt Cause Read - R/clr */
#define E1000_ICS      0x000C8/4  /* Interrupt Cause Set - WO */
#define E1000_IMS      0x000D0/4  /* Interrupt Mask Set - RW */
#define E1000_IMC      0x000D8/4  /* Interrupt Mask Clear - WO */
#define E1000_RDBAL    0x02800/4  /* RX Descriptor Base Address Low - RW */
#define E1000_RDBAH    0x02804/4  /* RX Descriptor Base Address High - RW */
#define E1000_RDLEN    0x02808/4  /* RX Descriptor Length - RW */
#define E1000_RDH      0x02810/4  /* RX Descriptor Head - RW */
#define E1000_RDT      0x02818/4  /* RX Descriptor Tail - RW */
#define E1000_RCTL     0x00100/4  /* RX Control - RW */

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

/* Interrupt Cause Read */
#define E1000_ICR_RXT0          0x00000080 /* rx timer intr (ring 0) */
/* Interrupt Mask Clear */
#define E1000_IMC_RXT0      E1000_ICR_RXT0      /* rx timer intr */
/* Interrupt Cause Set */
#define E1000_ICS_RXT0      E1000_ICR_RXT0      /* rx timer intr */
/* Interrupt Mask Set */
#define E1000_IMS_RXT0      E1000_ICR_RXT0      /* rx timer intr */

/* Receive Control */
#define E1000_RCTL_RST            0x00000001    /* Software reset */
#define E1000_RCTL_EN             0x00000002    /* enable */
#define E1000_RCTL_SBP            0x00000004    /* store bad packet */
#define E1000_RCTL_UPE            0x00000008    /* unicast promiscuous enable */
#define E1000_RCTL_MPE            0x00000010    /* multicast promiscuous enab */
#define E1000_RCTL_LPE            0x00000020    /* long packet enable */
#define E1000_RCTL_LBM_NO         0x00000000    /* no loopback mode */
#define E1000_RCTL_LBM_MAC        0x00000040    /* MAC loopback mode */
#define E1000_RCTL_LBM_SLP        0x00000080    /* serial link loopback mode */
#define E1000_RCTL_LBM_TCVR       0x000000C0    /* tcvr loopback mode */
#define E1000_RCTL_DTYP_MASK      0x00000C00    /* Descriptor type mask */
#define E1000_RCTL_DTYP_PS        0x00000400    /* Packet Split descriptor */
#define E1000_RCTL_RDMTS_HALF     0x00000000    /* rx desc min threshold size */
#define E1000_RCTL_RDMTS_QUAT     0x00000100    /* rx desc min threshold size */
#define E1000_RCTL_RDMTS_EIGTH    0x00000200    /* rx desc min threshold size */
#define E1000_RCTL_MO_SHIFT       12            /* multicast offset shift */
#define E1000_RCTL_MO_0           0x00000000    /* multicast offset 11:0 */
#define E1000_RCTL_MO_1           0x00001000    /* multicast offset 12:1 */
#define E1000_RCTL_MO_2           0x00002000    /* multicast offset 13:2 */
#define E1000_RCTL_MO_3           0x00003000    /* multicast offset 15:4 */
#define E1000_RCTL_MDR            0x00004000    /* multicast desc ring 0 */
#define E1000_RCTL_BAM            0x00008000    /* broadcast enable */
/* these buffer sizes are valid if E1000_RCTL_BSEX is 0 */
#define E1000_RCTL_SZ_2048        0x00000000    /* rx buffer size 2048 */
#define E1000_RCTL_SZ_1024        0x00010000    /* rx buffer size 1024 */
#define E1000_RCTL_SZ_512         0x00020000    /* rx buffer size 512 */
#define E1000_RCTL_SZ_256         0x00030000    /* rx buffer size 256 */
/* these buffer sizes are valid if E1000_RCTL_BSEX is 1 */
#define E1000_RCTL_SZ_16384       0x00010000    /* rx buffer size 16384 */
#define E1000_RCTL_SZ_8192        0x00020000    /* rx buffer size 8192 */
#define E1000_RCTL_SZ_4096        0x00030000    /* rx buffer size 4096 */
#define E1000_RCTL_VFE            0x00040000    /* vlan filter enable */
#define E1000_RCTL_CFIEN          0x00080000    /* canonical form enable */
#define E1000_RCTL_CFI            0x00100000    /* canonical form indicator */
#define E1000_RCTL_DPF            0x00400000    /* discard pause frames */
#define E1000_RCTL_PMCF           0x00800000    /* pass MAC control frames */
#define E1000_RCTL_BSEX           0x02000000    /* Buffer size extension */
#define E1000_RCTL_SECRC          0x04000000    /* Strip Ethernet CRC */
#define E1000_RCTL_FLXBUF_MASK    0x78000000    /* Flexible buffer size */
#define E1000_RCTL_FLXBUF_SHIFT   27            /* Flexible buffer shift */

#define E1000_RAH_AV  0x80000000        /* Receive descriptor valid */

/* Trasmit Queue and Buffer */
#define E1000_TDNUM                      0x20
#define TX_BUFFER_SIZE                   PGSIZE/2

/* Transmit Descriptor */
struct tx_desc {
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
struct tx_buffer {
	char buf[TX_BUFFER_SIZE];
};

/* Reiceive Queue and Buffer */
#define E1000_RDNUM                      0x100
#define RX_BUFFER_SIZE                   PGSIZE/2

/* Receive Descriptor */
struct rx_desc {
    uint64_t addr;       /* Address of the descriptor's data buffer */
    uint16_t length;     /* Length of data DMAed into data buffer */
    uint16_t csum;       /* Packet checksum */
    uint8_t status;      /* Descriptor status */
    uint8_t errors;      /* Descriptor Errors */
    uint16_t special;
};
#define E1000_RXD_STA_DD       0x01    /* Descriptor Done */
#define E1000_RXD_STA_EOP      0x02    /* End of Packet */

/* Receive Buffer */
struct rx_buffer {
	char buf[RX_BUFFER_SIZE];
};

int e1000_attach (struct pci_func *pcif);
int transmit_packet(void *va, size_t len);
int receive_packet(void *va, size_t *len);
void e1000_intr(void);
#endif	// JOS_KERN_E1000_H
