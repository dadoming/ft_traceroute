#include "../include/main_header.h"

void exit_program(t_tracert *t, int num) {
	if (t) {
		// t->dest_addr = argv[i] dont need free
		if (t->socket > 0) {
			close(t->socket);
		}
	}
	exit(num);
}
