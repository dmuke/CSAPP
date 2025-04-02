#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
	if (argc != 2)
		exit(0);
	struct in_addr inaddr;
	int rc;

	rc =inet_pton(AF_INET,argv[1],&inaddr);
	if (rc <= 0)
	{
		printf("error\n");
		exit(0);
	}
	printf("0x%x\n",ntohl(inaddr.s_addr) );
	return 0;
}