#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* Routine(void* arg)
{
	char* msg = (char*)arg;
	while (1){
		printf("I am %s\n", msg);
		sleep(1);
	}
}
int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, Routine, (void*)"thread 1");
	while (1){
		printf("I am main thread!\n");
		sleep(2);
	}
	return 0;
}
