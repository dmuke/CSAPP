#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
pid_t pid,pid1,pid2;
int x = 1;

pid = fork();
if (pid == 0){
	pid1 = getpid();
	printf("child: x = %d,pid = %d\n", ++x,pid1);
	exit(0);
}

pid2 = getpid();
printf("parent: x = %d,pid = %d\n",--x,pid2 );
exit(0);

return(0);
}