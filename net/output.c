#include <inc/error.h>
#include <inc/lib.h>

#include "ns.h"

extern union Nsipc nsipcbuf;

void
output(envid_t ns_envid)
{
	binaryname = "ns_output";

	// LAB 6: Your code here:
	// 	- read a packet from the network server
	//	- send the packet to the device driver
	uint32_t req, whom;
	int perm, r;

	while (1) {
		perm = 0;
		req = ipc_recv((int32_t *)&whom, &nsipcbuf, &perm);

		if (req != NSREQ_OUTPUT) {
			cprintf("NS OUTPUT: receive request %x not OUTPUT\n", req);
			continue;
		}
		if (whom != ns_envid) {
			cprintf("NS OUTPUT: output timer got IPC message from env %x not NS\n", whom);
			continue;
		}
		if (!(perm & PTE_P)) {
			cprintf("NS OUTPUT: invalide request from NS: no argument page\n");
			continue;
		}

		while((r = sys_trans_pkt(&nsipcbuf.pkt.jp_data, nsipcbuf.pkt.jp_len)) != 0) {
			if (r != -E_TXD_BUF_FULL) {
				cprintf("NS OUTPUT: sys_trans_pkt error:%e\n", r);
				break;
			}
		}
		sys_page_unmap(0, &nsipcbuf);
	}
}
