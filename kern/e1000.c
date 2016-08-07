#include <inc/error.h>
#include <inc/string.h>

#include <kern/e1000.h>
#include <kern/pci.h>
#include <kern/pmap.h>

// LAB 6: Your driver code here
#define debug 1

volatile uint32_t *e1000;
struct tx_desc tx_desc_array[E1000_TDNUM];
struct tx_buffer tx_buffer_array[E1000_TDNUM];

int
e1000_attach(struct pci_func *f)
{
	int i;
	
	pci_func_enable(f);
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
