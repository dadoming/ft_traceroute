#ifndef PROGRAM_MAIN_HEADER_H
#define PROGRAM_MAIN_HEADER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

#include "utils.h"

#define FLAG_HELP "h"
#define DEBUG true

typedef struct s_tracert {
	int socket;
	struct sockaddr dest;

	char *dest_addr;
} t_tracert;

void parse_input(int argc, char **argv, t_tracert *tracert);
void exit_program(t_tracert *t, int num);
void help_message(char *name, int num);

#endif //PROGRAM_MAIN_HEADER_H
