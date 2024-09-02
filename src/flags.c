#include "../include/main_header.h"

void help_message(char *name, int num) {
	printf("Usage:\n\
  %s [-%s] <destination>\n\
Options\n\
  -%s: Display this help message\n\
", name, FLAG_HELP, FLAG_HELP);

	if (DEBUG) {
		printf("Where: %d\n", num);
	}
}
