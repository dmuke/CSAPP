#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
	if (argc != 2)
		exit(0);
	struct in_addr inaddr;
	uint32_t addr;
	char buf[1024];

	sscanf(argv[1],"%x",&addr);
	inaddr.s_addr = htonl(addr);

	if (!inet_ntop(AF_INET,&inaddr,buf,1024))
	{
		printf("error\n");
		exit(0);
	}printf("%s\n",buf );
	return 0;
}