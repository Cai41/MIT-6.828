#include "ns.h"
#include <inc/lib.h>

extern union Nsipc nsipcbuf;

void
input(envid_t ns_envid)
{
	binaryname = "ns_input";

	// LAB 6: Your code here:
	// 	- read a packet from the device driver
	//	- send it to the network server
	// Hint: When you IPC a page to the network server, it will be
	// reading from it for a while, so don't immediately receive
	// another packet in to the same physical page.
	int r;
	
	while (1) {
		if ((r = sys_page_alloc(0, (void *)&nsipcbuf, PTE_P|PTE_U|PTE_W)) < 0)
			panic("sys_page_alloc: %e", r);
		while((r = sys_recv_pkt(&nsipcbuf.pkt.jp_data, (size_t *)&nsipcbuf.pkt.jp_len)) != 0) {
			if (r != -E_RXD_BUF_EMPTY) {
				cprintf("NS INTPUT: sys_recv_pkt error:%e\n", r);
				break;
			}
		}
		if (r != 0) continue;
		ipc_send(ns_envid, NSREQ_INPUT, &nsipcbuf, PTE_U|PTE_W|PTE_P);
		sys_page_unmap(0, (void *)&nsipcbuf);
	}
}
