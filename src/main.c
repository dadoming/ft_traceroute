#include "../include/main_header.h"
#include <netinet/ip.h>

#define HOP_LIMIT 30
#define PACKETS_PER_HOP 3

int main(int argc, char **argv) {
	if (argc != 2) {
		help_message(argv[0], 0);
		exit_program(NULL, 0);
	}

	t_tracert tracert;
	ft_memset(&tracert, 0, sizeof(t_tracert));
	parse_input(argc, argv, &tracert);

	struct addrinfo* result;
	struct addrinfo *res;
	int error = getaddrinfo(tracert.dest_addr, NULL, NULL, &result);
	if (error != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
		exit_program(&tracert, 1);
	}

	for (res = result; res != NULL; res = res->ai_next) {
		char hostname[NI_MAXHOST];
        error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
        if (error != 0) {
            fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
            continue;
        }
        if (*hostname != '\0') {
            printf("hostname: %s\n", hostname);
		    tracert.dest = *res->ai_addr;
			break;
		}
	}
	freeaddrinfo(result);

	tracert.socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (tracert.socket < 0) {
		perror("socket");
		exit_program(&tracert, 1);
	}

	struct timeval tv = {1, 0};
	if (setsockopt(tracert.socket, IPPROTO_IP, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		perror("setsockopt");
		exit_program(&tracert, 1);
	}

	printf("Tracing route to %s\n", tracert.dest_addr);
	printf("over a maximum of %d hops:\n\n", HOP_LIMIT);

	char aux[NI_MAXHOST];
	getnameinfo(&tracert.dest, sizeof(tracert.dest), aux, NI_MAXHOST, NULL, 0, 0);
	printf("Tracing route to %s [%s]\n", tracert.dest_addr, aux);

for (int ttl = 1; ttl <= HOP_LIMIT; ttl++) {
        int received = 0;
        printf("%d\t", ttl);

        char hop_hostname[NI_MAXHOST] = {0};
        char hop_ip[INET_ADDRSTRLEN] = {0};
        int print_hostname = 1;

        for (int p = 0; p < PACKETS_PER_HOP; p++) {
            struct sockaddr_in addr;
            ft_memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = ((struct sockaddr_in*)&tracert.dest)->sin_addr.s_addr;

            struct icmphdr icmp;
            ft_memset(&icmp, 0, sizeof(icmp));
            icmp.type = ICMP_ECHO;
            icmp.code = 0;
            icmp.un.echo.id = getpid();
            icmp.un.echo.sequence = ttl * 10 + p;
            icmp.checksum = calculate_checksum(&icmp, sizeof(icmp));

            if (setsockopt(tracert.socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
                perror("setsockopt");
                exit_program(&tracert, 1);
            }

            struct timeval start, end;
            gettimeofday(&start, NULL);

            if (sendto(tracert.socket, &icmp, sizeof(icmp), 0, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                perror("sendto");
                exit_program(&tracert, 1);
            }

            unsigned char buf[512];
            struct sockaddr_in recv_addr;
            ft_memset(&recv_addr, 0, sizeof(recv_addr));
            socklen_t len = sizeof(recv_addr);

            struct timeval tv;
            tv.tv_sec = 1; // 1 second timeout
            tv.tv_usec = 0;

            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(tracert.socket, &readfds);

            int retval = select(tracert.socket + 1, &readfds, NULL, NULL, &tv);
            if (retval == -1) {
                perror("select()");
                continue;
            } else if (retval == 0) {
                // Timeout, no response received within 1 second
                printf("* ");
                continue;
            }

            int recv_len = recvfrom(tracert.socket, buf, sizeof(buf), 0, (struct sockaddr*)&recv_addr, &len);
            if (recv_len < 0) {
                perror("recvfrom");
                continue;
            }

            gettimeofday(&end, NULL);
            long rtt = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

            struct iphdr *ip = (struct iphdr*)buf;
            struct icmphdr *icmp_recv = (struct icmphdr*)(buf + (ip->ihl * 4));

            if (icmp_recv->type == ICMP_TIME_EXCEEDED || icmp_recv->type == ICMP_ECHOREPLY) {
                received = 1;

                if (print_hostname) {
                    getnameinfo((struct sockaddr*)&recv_addr, sizeof(recv_addr), hop_hostname, NI_MAXHOST, NULL, 0, 0);
                    inet_ntop(AF_INET, &(recv_addr.sin_addr), hop_ip, sizeof(hop_ip));
                    printf("%s (%s) ", hop_hostname[0] ? hop_hostname : hop_ip, hop_ip);
                    print_hostname = 0;

                }
                printf("%ld ms ", rtt);
				if (icmp_recv->type == ICMP_ECHOREPLY) {
					printf("\nReached destination\n");
					exit_program(&tracert, 0);
				}
            }
        }

        if (!received) {
            printf("* * *");
        }
        printf("\n");
    }

	exit_program(&tracert, 0);
	return 0;
}
