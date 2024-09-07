#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include "utils.h"

#define FLAG_HELP "h"
#define FLAG_FIRST_TTL "f"
#define FLAG_MAX_TTL "m"
#define FLAG_DONT_MAP "n"
#define FLAG_QUERIES "q"
#define AVAILABLE_FLAGS FLAG_HELP FLAG_FIRST_TTL FLAG_MAX_TTL FLAG_DONT_MAP FLAG_QUERIES

#define DEBUG true

#define DEFAULT_HOP_LIMIT 30
#define PACKETS_PER_HOP 3
#define PACKET_SIZE 60

// remove this line if it works
#ifndef NI_MAXHOST
# define NI_MAXHOST 1025
#endif

typedef struct s_flag
{
	long value;
	bool entered;
	char *name;
} t_flag;

typedef struct s_flags
{
	t_flag f;
	t_flag n;
	t_flag m;
	t_flag q;
} t_flags;

typedef struct s_packet {
	struct icmphdr header;
	char data[PACKET_SIZE - sizeof(struct icmphdr)];
} t_packet;

typedef struct time_miliseconds_decimals {
	long milliseconds;
	long microseconds;
} time_ms;

typedef struct s_tracert {
	pid_t pid;
	int socket;
	struct sockaddr dest;

	char *dest_addr;
	char hostname[NI_MAXHOST];
	t_flags flags;
} t_tracert;

void parse_input(int argc, char **argv, t_tracert *tracert);
void exit_program(t_tracert *t, int num);
void usage_message(char *name, int num);
void setup_socket(t_tracert *t);
void init_program(t_tracert *t);
void traceroute(t_tracert *t);
void help_flag(t_tracert *t, int argc, char **argv);


#endif //FT_TRACEROUTE_H
