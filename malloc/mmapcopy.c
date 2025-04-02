#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void mmapcopy(int fd,int size){
	char *bufp;

	bufp = mmap(NULL,size,PROT_READ,MAP_PRIVATE,fd,0);
	write(STDOUT_FILENO,bufp,size);
	return ;
}

int main(int argc, char const *argv[])
{
	struct stat s;
	int fd;

	if (argc != 2){
		printf("usage: %s <filename>\n", argv[0]);
		exit(0);
	}
	fd = open(argv[1],O_RDONLY,0644);
	fstat(fd,&s);
	mmapcopy(fd,s.st_size);
	return 0;
}

