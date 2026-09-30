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


void	fatal_error(char *err)
{
	write(2, "err", strlen(err));
	write(2, "\n", 1);
	exit (1);
}

uint16_t checksum(uint16_t *data, size_t size)
{
	uint32_t sum = 0;
	size_t data_len = size/2;
	for (size_t i = 0; i < data_len; i++)
		sum+= data[i];
	return ~((sum << 16 >> 16 ) + (sum >>16));
}

int main(int argc, char **argv)
{
	// PARSING
	int             opt;
	struct in_addr  addr;
	struct addrinfo hints;
	struct addrinfo *results;
	char            *target;
	struct sockaddr_in dest;

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



	if (getaddrinfo(target, NULL, &hints, &results) != 0)
	{
		fprintf(stderr, "Adresse introuvable\n");
		return (1);
	}

	dest = *(struct sockaddr_in *)results->ai_addr;

	freeaddrinfo(results);


	// INIT
	int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0)
	{
		fatal_error("socket failed");
	}

	struct icmphdr packet =
	{
		.type = ICMP_ECHO,
		.code = 0,
		.checksum = 0,
		.un = { .echo = { .id = 0 , .sequence = 0}}
	};
	packet.checksum = checksum((uint16_t *) &packet, sizeof(packet));


	if ((sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr *)&dest, sizeof(dest))) == -1)
	{
		perror("send :");
	}

	char buffer[1500];
	struct sockaddr_in reply;
	socklen_t reply_len = sizeof(reply);

	if ((recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&reply, &reply_len)) == -1)
	{
		perror("recv :");
	}
	else
	{
		printf("pong");
	}

	close(sock);
	return (0);
}
