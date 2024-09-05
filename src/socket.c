#include "../include/ft_traceroute.h"

static void create_icmp_socket(t_tracert *t);
static void resolve_destination_address(t_tracert *t, struct addrinfo **result);
static void set_socket_timeout(t_tracert *t);
static int resolve_hostname(struct addrinfo *res, char *hostname);
static void reverse_lookup(t_tracert *t, struct addrinfo *result);

void setup_socket(t_tracert *t) {
	struct addrinfo *result;
	resolve_destination_address(t, &result);
	reverse_lookup(t, result);
	create_icmp_socket(t);
	set_socket_timeout(t);
}

static void reverse_lookup(t_tracert *t, struct addrinfo *result) {
	struct addrinfo *res;
	for (res = result; res != NULL; res = res->ai_next) {
		char hostname[NI_MAXHOST];
		if (resolve_hostname(res, hostname) == 0 && *hostname != '\0') {
			t->dest = *res->ai_addr;
			break;
		}
	}
	freeaddrinfo(result);
	if (getnameinfo(&t->dest, sizeof(t->dest), t->hostname, NI_MAXHOST, NULL, 0, 0)) {
		printf("Could not resolve hostname\n");
		exit_program(t, 1);
	}
}

static void resolve_destination_address(t_tracert *t, struct addrinfo **result) {
	int error = getaddrinfo(t->dest_addr, NULL, NULL, result);
	if (error != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
		exit_program(t, 1);
	}
}


static int resolve_hostname(struct addrinfo *res, char *hostname) {
	int error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
	if (error != 0) {
		fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
		return -1;
	}
	return 0;
}

static void set_socket_timeout(t_tracert *t) {
	struct timeval tv = {1, 0};  // 1 second timeout
	if (setsockopt(t->socket, IPPROTO_IP, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) exit_program(t, 1);
}



static void create_icmp_socket(t_tracert *t) {
	t->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (t->socket < 0) exit_program(t, 1);
}
