#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>

int tickets = 2000;
void* TicketGrabbing(void* arg)
{
	std::string name = (char*)arg;
	while (true){
		if (tickets > 0){
			usleep(1000);
			std::cout << name << " get a ticket, tickets left: " << --tickets << std::endl;
		}
		else{
			break;
		}
	}
	std::cout << name << " quit..." << std::endl;
	pthread_exit((void*)0);
}

int main()
{
	pthread_t tid1, tid2, tid3, tid4;
	pthread_create(&tid1, nullptr, TicketGrabbing, (void*)"thread 1");
	pthread_create(&tid2, nullptr, TicketGrabbing, (void*)"thread 2");
	pthread_create(&tid3, nullptr, TicketGrabbing, (void*)"thread 3");
	pthread_create(&tid4, nullptr, TicketGrabbing, (void*)"thread 4");
	
	pthread_join(tid1, nullptr);
	pthread_join(tid2, nullptr);
	pthread_join(tid3, nullptr);
	pthread_join(tid4, nullptr);
	return 0;
}
