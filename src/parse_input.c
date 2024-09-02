#include "../include/main_header.h"

static void set_destination_address(t_tracert *tracert, char **argv, int argc, int i);

void parse_input(int argc, char **argv, t_tracert *tracert) {
	int i = 1;
	while (i < argc - 1) {
		if (argv[i][0] != '-') {
			help_message(argv[0], 3);
			exit_program(NULL, 0);
		}
		if (argv[i][0] == '-' && argv[i][1]) {
			switch (argv[i][1]) {
				case 'h':
					help_message(argv[0], 1);
					break;
				default:
					printf("Unknown flag: %s\n", argv[i]);
					help_message(argv[0], 4);
					exit_program(NULL, 0);
			}
		}
		i++;
	}

	set_destination_address(tracert, argv, argc, i);
}

static void set_destination_address(t_tracert *tracert, char **argv, int argc, int i) {
	if (i >= argc) {
		printf("Destination address not provided\n");
		help_message(argv[0], 5);
		exit_program(tracert, 1);
	} else if (argv[i][0] != '-') {
		tracert->dest_addr = argv[i];
	} else {
		help_message(argv[0], 2);
		exit_program(tracert, 1);
	}
}
