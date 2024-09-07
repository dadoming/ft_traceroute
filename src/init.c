#include "../include/ft_traceroute.h"

static void init_flags(t_tracert *t);

void init_program(t_tracert *t) {
	t->dest_addr = NULL;
	t->socket = 0;
	ft_memset(&t->dest, 0, sizeof(t->dest));
	t->pid = getpid();
	init_flags(t);
}

static void init_flags(t_tracert *t) {

	t->flags.m.value = 0;
	t->flags.m.name = FLAG_MAX_TTL;
	t->flags.m.entered = false;

	t->flags.n.value = 0;
	t->flags.n.name = FLAG_DONT_MAP;
	t->flags.n.entered = false;

	t->flags.f.value = 0;
	t->flags.f.name = FLAG_FIRST_TTL;
	t->flags.f.entered = false;

	t->flags.q.value = 0;
	t->flags.q.name = FLAG_QUERIES;
	t->flags.q.entered = false;
}
