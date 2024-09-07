#include "../include/ft_traceroute.h"

void usage_message(char *name, int num) {
	printf("Usage:\n\
  %s [-%s] <destination>\n", name, AVAILABLE_FLAGS);

	if (DEBUG) {
		printf("Where: %d\n", num);
	}
}

void help_flag(t_tracert *t, int argc, char **argv) {
    int i = 1;
	while (i < argc) {
		if (argv[i][0] == '-' && argv[i][1] && (argv[i][1] == 'h' || argv[i][1] == '?') && !argv[i][2]) {
			printf("\n\
Usage:\n\
  sudo ./ft_traceroute [options] <destination>\n\
\n\
Options:\n\
  <destination>\t\tdestination address to trace route\n\
  -f <first_ttl>\t\tstart from the first_ttl hop (instead from 1)\n\
  -m <max_ttl>\t\tset the max number of hops(default is %d)\n\
  -q <nqueries>\t\tset the number of queries per hop(default is %d)\n\
  -n\t\t\tdo not map IP to hostname\n\
  -t <ttl>\t\tset the ttl value\n\
  -h\t\t\thelp\n\
 \n", DEFAULT_HOP_LIMIT, PACKETS_PER_HOP);
			exit_program(t, 0);
		}
        i++;
	}
}
