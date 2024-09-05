#include "../include/ft_traceroute.h"

int main(int argc, char **argv) {
	t_tracert tracert;
	init_program(&tracert);

	if (argc != 2) {
		help_message(argv[0], 0);
		exit_program(NULL, 0);
	}
	parse_input(argc, argv, &tracert);
	setup_socket(&tracert);
	traceroute(&tracert);
	exit_program(&tracert, 0);
	return 0;
}
