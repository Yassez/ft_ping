

#include <netinet/in.h>




void	fatal_error(char *err)
{
	write(2, "err", strlen(err));
	write(2, '\n', 1);
	exit (1);
}

int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
if (sockfd < 0)
{
	fatal_error("socket");
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv)
{
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
		freeaddrinfo(results);
	}
	else
		printf("Adresse introuvable\n");

	return (0);
}