#include <iostream>
#include <cstdio>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
void* Routine(void* arg)
{
	pthread_detach(pthread_self());
	std::cout << (char*)arg << " run..." << std::endl;
	while (true){
		pthread_cond_wait(&cond, &mutex); //阻塞在这里，直到被唤醒
		std::cout << (char*)arg << "活动..." << std::endl;
	}
}
int main()
{
	pthread_t t1, t2, t3;
	pthread_mutex_init(&mutex, nullptr);
	pthread_cond_init(&cond, nullptr);
	
	pthread_create(&t1, nullptr, Routine, (void*)"thread 1");
	pthread_create(&t2, nullptr, Routine, (void*)"thread 2");
	pthread_create(&t3, nullptr, Routine, (void*)"thread 3");
	
	while (true){
		getchar();
		pthread_cond_signal(&cond);
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}
