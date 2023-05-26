#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

void* Routine(void* arg)
{
	char* msg = (char*)arg;
	while (1){
		printf("I am %s...pid: %d, ppid: %d, tid: %lu\n", msg, getpid(), getppid(), pthread_self());
		sleep(1);
	}
}
int main()
{
	pthread_t tid[5];
    int i;
	for (i = 0; i < 5; i++){
		char* buffer = (char*)malloc(64);
		sprintf(buffer, "thread %d", i);
		pthread_create(&tid[i], NULL, Routine, buffer);
		printf("%s tid is %lu\n", buffer, tid[i]);
	}
	while (1){
		 printf("I am main thread...pid: %d, ppid: %d, tid: %lu\n", getpid(), getppid(), pthread_self());
		 sleep(2);
	}
	return 0;
}
