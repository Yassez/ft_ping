#include <sys/socket.h>    // Sockets
#include <netinet/ip.h>    // IP header
#include <netinet/ip_icmp.h> // ICMP header
#include <arpa/inet.h>     // inet_pton(), inet_ntoa()
#include <netdb.h>

#include <unistd.h>        // sleep(), getpid()
#include <time.h>          // gettimeofday()
#include <string.h>        // memcpy(), memset()
#include <stdio.h>         // printf(), perror()
#include <stdlib.h>        // exit()
#include <errno.h>         // errno, strerror()



#define PAYLOAD_SIZE sizeof(struct timeval)

#define RECV_BUF_SIZE 65536 // Max IP packet size


void	timer()
{

}

void	build_icmp_request(struct icmp *icmp_pkt, int seq)
{
	memset(icmp_pkt, 0, sizeof (struct icmp) + PAYLOAD_SIZE);

	icmp_pkt->icmp_type = ICMP_ECHO;
	icmp_pkt->icmp_code = 0 ;
	icmp_pkt->icmp_id = getpid();
	icmp_pkt->icmp_seq = seq;
	icmp_pkt->icmp_cksum = 0;

	struct timeval send_time;
	gettimeofday(&send_time, NULL);
	memcpy(icmp_pkt->icmp_data, &send_time, PAYLOAD_SIZE);

	icmp_pkt->icmp_cksum = icmp_checksum(icmp_pkt, sizeof(struct icmp) + PAYLOAD_SIZE);

}



void	fatal_error(char *err)
{
	write(2, "err", strlen(err));
	write(2, '\n', 1);
	exit (1);
}



int main(int argc, char **argv)
{
	// INIT
	int sockraw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0)
	{
		fatal_error("socket failed");
	}

	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	int setsockopt(sockraw, SOL_SOCKET, SO_RECVTIMEO, &timeout, sizeof(timeout))
	if ((setsockopt(sockraw, SOL_SOCKET, SO_RECVTIMEO, &timeout, sizeof(timeout))) == -1)
	{
		fatal_error("setsockopt(SO_RCVTIMEO) failed");
	}


	// PARSING
	int             opt;
	struct in_addr  addr;
	struct addrinfo hints;
	struct addrinfo *results;
	char            *target;

	while ((opt = getopt(argc, argv, "v?")) != -1)
	{
		if (opt == 'v')
			printf("verbose\n");
		else if (opt == '?')
			printf("list option\n");
	}

	if (optind >= argc)
	{
		fprintf(stderr, "Missing destination\n");
		return (1);
	}

	target = argv[optind];

	if (inet_pton(AF_INET, target, &addr) == 1)
		printf("IPv4 valide\n");
	else
		printf("Pas une IPv4\n");

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;

	if (getaddrinfo(target, NULL, &hints, &results) == 0)
	{
		printf("Adresse résolvable\n");
		struct sockaddr_in *addr_in;
		addr_in = (struct sockaddr_in *)results->ai_addr;
		addr_in->sin_addr

		freeaddrinfo(results);

	}
	else
		printf("Adresse introuvable\n");




	// Buffer for ICMP packet (header + payload)
	char send_buf[sizeof(struct icmp) + PAYLOAD_SIZE];
	struct icmp *icmp_pkt = (struct icmp *)send_buf;

	// Build request (seq = 0, 1, 2, ...)
	build_icmp_request(icmp_pkt, seq);

	// Send packet
	ssize_t sent = sendto(
		sockfd,          // Socket file descriptor
		send_buf,        // Packet buffer
		sizeof(send_buf),// Packet length
		0,               // Flags
		(struct sockaddr *)&dest_addr, // Destination address
		sizeof(dest_addr)
	);

	if (sent == -1) {
		perror("sendto() failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}


	while(1)
	{

	}
	return (0);
}
