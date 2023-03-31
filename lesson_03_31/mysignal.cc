#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
	signal(SIGCHLD, SIG_IGN);
	if (fork() == 0){
		//child
		printf("child is running, child dead: %d\n", getpid());
		sleep(3);
		exit(1);
	}
	//father
	while (1);
	return 0;
}
