#include <inc/error.h>
#include <inc/string.h>

#include <kern/env.h>
#include <kern/e1000.h>
#include <kern/pci.h>
#include <kern/pmap.h>
#include <kern/picirq.h>

// LAB 6: Your driver code here
#define debug 1

volatile uint32_t *e1000;
struct tx_desc tx_desc_array[E1000_TDNUM];
struct tx_buffer tx_buffer_array[E1000_TDNUM];
struct rx_desc rx_desc_array[E1000_RDNUM];
struct rx_buffer rx_buffer_array[E1000_RDNUM];

int
e1000_attach(struct pci_func *f)
{
	int i;
	
	pci_func_enable(f);
	cprintf("E1000 IRQ:%d\n", f->irq_line);
	irq_setmask_8259A(irq_mask_8259A & ~(1<<11));
	
	e1000 = mmio_map_region(f->reg_base[0], f->reg_size[0]);
	if (debug)
		cprintf("status register = 0x%0x\n", e1000[E1000_STATUS]);

	e1000[E1000_TDBAL] = PADDR(tx_desc_array);
	e1000[E1000_TDLEN] = sizeof(struct tx_desc) * E1000_TDNUM;
	for (i = 0; i < E1000_TDNUM; i++) {
		physaddr_t pa = PADDR(&tx_buffer_array[i].buf);
		tx_desc_array[i].addr = pa;
		tx_desc_array[i].cmd = E1000_TXD_CMD_RS;
		tx_desc_array[i].status = E1000_TXD_STA_DD;
	}
	e1000[E1000_TDH] = 0;
	e1000[E1000_TDT] = 0;
	e1000[E1000_TCTL] = E1000_TCTL_EN | E1000_TCTL_PSP | E1000_TCTL_COLD_FDUP;
	e1000[E1000_TIPG] = E1000_TIPG_IPGT_DEFAULT | E1000_TIPG_IPGR1_DEFAULT | E1000_TIPG_IPGR2_DEFAULT;

	e1000[E1000_RAL] = MAC_LOW_ADDR;
	e1000[E1000_RAH] = MAC_HIGH_ADDR | E1000_RAH_AV;
	cprintf("mac:%x:%x\n", e1000[E1000_RAH], e1000[E1000_RAL]);
	e1000[E1000_MTA] = 0;
	e1000[E1000_MTA+1] = 0;
	e1000[E1000_MTA+2] = 0;
	e1000[E1000_MTA+3] = 0;
	e1000[E1000_IMS] = E1000_ICR_RXT0;
	for (i = 0; i < E1000_RDNUM; i++) {
		physaddr_t pa = PADDR(&rx_buffer_array[i].buf);
		rx_desc_array[i].addr = pa;
	}	
	e1000[E1000_RDBAL] = PADDR(&rx_desc_array);
	e1000[E1000_RDLEN] = sizeof(struct rx_desc) * E1000_RDNUM;
	e1000[E1000_RDH] = 0;
	e1000[E1000_RDT] = E1000_RDNUM - 1;
	e1000[E1000_RCTL] = E1000_RCTL_EN | E1000_RCTL_BAM | E1000_RCTL_SZ_2048 | E1000_RCTL_SECRC;
	return 1;
}

int
transmit_packet(void *va, size_t len)
{
	int tail = e1000[E1000_TDT];
	if (len > TX_BUFFER_SIZE)
		panic("buffer is too large to be transmitted!\n");
	if (tx_desc_array[tail].status & E1000_TXD_STA_DD) {
		memmove(&tx_buffer_array[tail].buf, va, len);
		tx_desc_array[tail].length = len;
		tx_desc_array[tail].cmd |= E1000_TXD_CMD_EOP;
		tx_desc_array[tail].status &= ~E1000_TXD_STA_DD;
		e1000[E1000_TDT] = (1 + e1000[E1000_TDT]) % E1000_TDNUM;
		return 0;
	} else {
		return -E_TXD_BUF_FULL;
	}
}

int
receive_packet(void *va, size_t *len)
{
	int tail = (e1000[E1000_RDT] + 1) % E1000_RDNUM;
	if (rx_desc_array[tail].length > RX_BUFFER_SIZE)
		panic("buffer is too large to be received!\n");
	if (rx_desc_array[tail].status & E1000_RXD_STA_DD) {
		if (!(rx_desc_array[tail].status & E1000_RXD_STA_EOP))
			panic("packet too large\n");
		memmove(va, &rx_buffer_array[tail].buf, rx_desc_array[tail].length);
		*len = rx_desc_array[tail].length;
		rx_desc_array[tail].status &= ~E1000_RXD_STA_DD;
		e1000[E1000_RDT] = (1 + e1000[E1000_RDT]) % E1000_RDNUM;
		return 0;
	} else {
		return -E_RXD_BUF_EMPTY;
	}
}

void e1000_intr()
{
	int tail = (e1000[E1000_RDT] + 1) % E1000_RDNUM;
	if (rx_desc_array[tail].status & E1000_RXD_STA_DD) {
		int i;
		for (i = 0; i < NENV; i++) {
			if (envs[i].env_e1000_recving) {
				envs[i].env_e1000_recving = 0;
				envs[i].env_status = ENV_RUNNABLE;
				break;
			}
		}
	}
	e1000[E1000_ICR] |= E1000_ICR_RXT0;
}
