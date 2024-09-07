#include "../include/ft_traceroute.h"

static void fill_packet(t_packet *packet, int ttl, int seq, pid_t pid);
static bool get_reply(int socket, int can_print);
static void send_packet(t_tracert *t, int ttl, int seq, struct timeval *start, struct sockaddr_in addr);
static time_ms calculate_rtt(struct timeval *start, struct timeval *end);

#define TTL_

void traceroute(t_tracert *t) {

	struct sockaddr_in addr;
	ft_memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ((struct sockaddr_in*)&t->dest)->sin_addr.s_addr;

	char ipv4_address[INET_ADDRSTRLEN];
	long max_hops = t->flags.m.entered ? t->flags.m.value : DEFAULT_HOP_LIMIT;
	printf("traceroute to %s (%s), %ld hops max, %d byte packets\n", \
		t->dest_addr, \
		inet_ntop(AF_INET, &addr.sin_addr, ipv4_address, INET_ADDRSTRLEN), \
		max_hops, \
		PACKET_SIZE
	);
	long max_queries = t->flags.q.entered ? t->flags.q.value : PACKETS_PER_HOP;
	for (int ttl = 1; ttl <= max_hops; ttl++) {
		int can_print = t->flags.f.entered ? ttl >= t->flags.f.value : 1;
		if (can_print)
			printf("%-2d\t", ttl);

		char hop_hostname[NI_MAXHOST] = "";
		char hop_ip[INET_ADDRSTRLEN] = "";
		int print_hostname = 1;

		for (int seq = 0; seq < max_queries; seq++) {
			struct timeval start, end;
			send_packet(t, ttl, seq, &start, addr);

			if (!get_reply(t->socket, can_print)) {
				continue;
			}

			unsigned char buf[512];
			struct sockaddr_in recv_addr;
			ft_memset(&recv_addr, 0, sizeof(recv_addr));
			socklen_t len = sizeof(recv_addr);
			int recv_len = recvfrom(t->socket, buf, sizeof(buf), 0, (struct sockaddr*)&recv_addr, &len);
			if (recv_len < 0)
				continue;

			struct ip *ip = (struct ip*)buf;
			struct icmphdr *recv_packet_header = (struct icmphdr*)(buf + (ip->ip_hl << 2));

			gettimeofday(&end, NULL);
			time_ms rtt = calculate_rtt(&start, &end);
			if (recv_packet_header->type == ICMP_TIME_EXCEEDED || recv_packet_header->type == ICMP_ECHOREPLY) {
				if (print_hostname) {
					if (can_print) {
						if (t->flags.n.entered) {
							inet_ntop(AF_INET, &(recv_addr.sin_addr), hop_ip, sizeof(hop_ip));
							printf("%-20s\t", hop_ip);
						} else {
							// getnameinfo will resolve the hostname, if possible, WITHOUT performing a DNS lookup
							int status = getnameinfo((struct sockaddr*)&recv_addr, sizeof(recv_addr), hop_hostname, NI_MAXHOST, NULL, 0, 0);
							inet_ntop(AF_INET, &(recv_addr.sin_addr), hop_ip, sizeof(hop_ip));
							printf("%-35s (%s)\t", status == 0 ? hop_hostname : hop_ip, hop_ip);
						}
					}
					print_hostname = 0;
				}

				if (can_print)
					printf("%ld.%03ld ms\t", rtt.milliseconds, rtt.microseconds);
				if (recv_packet_header->type == ICMP_ECHOREPLY) {
					if (can_print) {
						printf("\nReached destination\n");
						exit_program(t, 0);
					}
				}
			}
		}
		if (can_print)
			printf("\n");
	}
}

static time_ms calculate_rtt(struct timeval *start, struct timeval *end) {
	time_ms rtt = {0, 0};
	long seconds = end->tv_sec - start->tv_sec;
	long useconds = end->tv_usec - start->tv_usec;
	if (useconds < 0) {
		seconds -= 1;
		useconds += 1000000;
	}
	long rtt_ms = seconds * 1000 + useconds / 1000;
	long rtt_us_fraction = useconds % 1000;

	rtt.milliseconds = rtt_ms;
	rtt.microseconds = rtt_us_fraction;
	return rtt;
}

static void send_packet(t_tracert *t, int ttl, int seq, struct timeval *start, struct sockaddr_in addr) {
	t_packet packet;
	fill_packet(&packet, ttl, seq, t->pid);
	if (setsockopt(t->socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
		exit_program(t, 1);
	gettimeofday(start, NULL);
	if (sendto(t->socket, &packet, sizeof(packet), 0, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		exit_program(t, 1);
}

static bool get_reply(int socket, int can_print) {
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(socket, &readfds);
	struct timeval tv = {1, 0};
	int retval = select(socket + 1, &readfds, NULL, NULL, &tv);
	if (retval == -1) {
		return false;
	} else if (retval == 0) {
		// Timeout, no response received within 1 second
		if (can_print)
			printf(" * ");
		fflush(stdout); // Subject does not specify that we can use it but it's so much prettier, remove if needed
		return false;
	}
	return true;
}

static void fill_packet(t_packet *packet, int ttl, int seq, pid_t pid) {
	ft_memset(packet, 0, sizeof(t_packet));
	packet->header.type = ICMP_ECHO;
	packet->header.code = 0;
	packet->header.un.echo.id = pid;
	packet->header.un.echo.sequence = ttl * 10 + seq;
	packet->header.checksum = 0;
	packet->header.checksum = calculate_checksum((unsigned short*)packet, sizeof(struct icmphdr) + PACKET_SIZE - sizeof(struct icmphdr));
}
