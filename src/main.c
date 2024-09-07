#include "../include/ft_traceroute.h"

int main(int argc, char **argv) {
	t_tracert tracert;
	init_program(&tracert);
	parse_input(argc, argv, &tracert);
	setup_socket(&tracert);
	traceroute(&tracert);
	exit_program(&tracert, 0);
	return 0;
}
