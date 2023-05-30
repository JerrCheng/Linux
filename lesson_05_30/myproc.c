#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int tickets = 1000;
pthread_mutex_t mutex;
void* TicketGrabbing(void* arg)
{
	const char* name = (char*)arg;
	while (1){
		pthread_mutex_lock(&mutex);
		if (tickets > 0){
			usleep(100);
			printf("[%s] get a ticket, left: %d\n", name, --tickets);
			pthread_mutex_unlock(&mutex);
		}
		else{
			pthread_mutex_unlock(&mutex);
			break;
		}
	}
	printf("%s quit!\n", name);
	pthread_exit((void*)0);
}
int main()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_t t1, t2, t3, t4;
	pthread_create(&t1, NULL, TicketGrabbing, "thread 1");
	pthread_create(&t2, NULL, TicketGrabbing, "thread 2");
	pthread_create(&t3, NULL, TicketGrabbing, "thread 3");
	pthread_create(&t4, NULL, TicketGrabbing, "thread 4");

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	pthread_mutex_destroy(&mutex);
	return 0;
}
