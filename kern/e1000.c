#include <kern/e1000.h>
#include <kern/pci.h>
#include <kern/pmap.h>

// LAB 6: Your driver code here
int debug = 1;

volatile uint32_t *e1000;

int
e1000_attach(struct pci_func *f)
{
	pci_func_enable(f);
	e1000 = mmio_map_region(f->reg_base[0], f->reg_size[0]);
	if (debug)
		cprintf("status register = 0x%0x\n", e1000[E1000_STATUS]);
	return 1;
}
