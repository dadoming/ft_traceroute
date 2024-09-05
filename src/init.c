#include "../include/ft_traceroute.h"

void init_program(t_tracert *t) {
	t->dest_addr = NULL;
	t->socket = 0;
	ft_memset(&t->dest, 0, sizeof(t->dest));
	t->pid = getpid();
}
