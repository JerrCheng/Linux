
#include "RingQueue.hpp"

void* Producer(void* arg)
{
	RingQueue<int>* rq = (RingQueue<int>*)arg;
	while (true){
		sleep(1);
		int data = rand() % 100 + 1;
		rq->Push(data);
		std::cout << "Producer: " << data << std::endl;
	}
}
void* Consumer(void* arg)
{
	RingQueue<int>* rq = (RingQueue<int>*)arg;
	while (true){
		sleep(1);
		int data = 0;
		rq->Pop(data);
		std::cout << "Consumer: " << data << std::endl;
	}
}
int main()
{
	srand((unsigned int)time(nullptr));
	pthread_t producer, consumer;
	RingQueue<int>* rq = new RingQueue<int>;
	pthread_create(&producer, nullptr, Producer, rq);
	pthread_create(&consumer, nullptr, Consumer, rq);
	
	pthread_join(producer, nullptr);
	pthread_join(consumer, nullptr);
	delete rq;
	return 0;
}

void* Producer(void* arg)
{
	RingQueue<int>* rq = (RingQueue<int>*)arg;
	while (true){
		int data = rand() % 100 + 1;
		rq->Push(data);
		std::cout << "Producer: " << data << std::endl;
	}
}
void* Consumer(void* arg)
{
	RingQueue<int>* rq = (RingQueue<int>*)arg;
	while (true){
		sleep(1);
		int data = 0;
		rq->Pop(data);
		std::cout << "Consumer: " << data << std::endl;
	}
}


void* Producer(void* arg)
{
	RingQueue<int>* rq = (RingQueue<int>*)arg;
	while (true){
		sleep(1);
		int data = rand() % 100 + 1;
		rq->Push(data);
		std::cout << "Producer: " << data << std::endl;
	}
}
void* Consumer(void* arg)
{
	RingQueue<int>* rq = (RingQueue<int>*)arg;
	while (true){
		int data = 0;
		rq->Pop(data);
		std::cout << "Consumer: " << data << std::endl;
	}
}
