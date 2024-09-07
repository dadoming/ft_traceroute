#include "../include/ft_traceroute.h"

static void set_destination_address(t_tracert *tracert, char **argv, int argc, int i);
static void check_invalid_input_arguments(t_tracert *tracert, int argc);
static void enter_flag_data_with_value(t_flag *flag, char **argv, int argc, int *i);

void parse_input(int argc, char **argv, t_tracert *tracert) {
	check_invalid_input_arguments(tracert, argc);
	help_flag(tracert, argc, argv);

	int i = 1;
	while (i < argc - 1) {
		if (argv[i][0] != '-') {
			usage_message(argv[0], 3);
			exit_program(NULL, 0);
		}
		if (argv[i][0] == '-' && argv[i][1]) {
			switch (argv[i][1]) {
				case 'h':
					usage_message(argv[0], 1);
					break;
				case 'f':
					enter_flag_data_with_value(&tracert->flags.f, argv, argc, &i);
					break;
				case 'm':
					enter_flag_data_with_value(&tracert->flags.m, argv, argc, &i);
					break;
				case 'n':
					tracert->flags.n.entered = true;
					break;
				case 'q':
					enter_flag_data_with_value(&tracert->flags.q, argv, argc, &i);
					break;
				default:
					printf("Unknown flag: %s\n", argv[i]);
					usage_message(argv[0], 4);
					exit_program(NULL, 0);
			}
		}
		i++;
	}

	if (tracert->flags.f.entered && (tracert->flags.f.value < 1)) {
		printf("First TTL value cannot be less than 1\n");
		exit_program(tracert, 1);
	}

	if (tracert->flags.m.entered && (tracert->flags.m.value < 1)) {
		printf("Max TTL value cannot be less than 1\n");
		exit_program(tracert, 1);
	}

	if (tracert->flags.f.entered && tracert->flags.m.entered && tracert->flags.f.value == tracert->flags.m.value) {
		printf("First TTL value cannot be equal to max TTL value\n");
		exit_program(tracert, 1);
	}

	if (tracert->flags.f.entered && tracert->flags.m.entered && tracert->flags.f.value > tracert->flags.m.value) {
		printf("First TTL value cannot be greater than max TTL value\n");
		exit_program(tracert, 1);
	}

	if (tracert->flags.f.entered && !tracert->flags.m.entered && tracert->flags.f.value > DEFAULT_HOP_LIMIT) {
		printf("First TTL value cannot be greater than %d\n", DEFAULT_HOP_LIMIT);
		exit_program(tracert, 1);
	}

	if (tracert->flags.q.entered && tracert->flags.q.value < 1) {
		printf("Number of queries per hop cannot be less than 1\n");
		exit_program(tracert, 1);
	}

	set_destination_address(tracert, argv, argc, i);
}

static void enter_flag_data_with_value(t_flag *flag, char **argv, int argc, int *i) {
	if (*i + 1 < argc) { // Because last argument is destination address, so, if required, there should be a value after the flag
		*i += 1;
		flag->value = ft_atol(argv[*i]);
		flag->entered = true;
	} else {
		printf("Flag %s requires a value\n", flag->name);
		exit_program(NULL, 1);
	}
}

static void set_destination_address(t_tracert *tracert, char **argv, int argc, int i) {
	if (i >= argc) {
		printf("Destination address not provided\n");
		usage_message(argv[0], 5);
		exit_program(tracert, 1);
	} else if (argv[i][0] != '-') {
		tracert->dest_addr = argv[i];
	} else {
		usage_message(argv[0], 2);
		exit_program(tracert, 1);
	}
}

static void check_invalid_input_arguments(t_tracert *tracert, int argc) {
	if (argc < 2) {
		printf("No destination address provided\n");
		exit_program(tracert, 1);
	}
}
