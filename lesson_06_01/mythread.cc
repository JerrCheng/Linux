#include "BlockQueue.hpp"

void* Producer(void* arg)
{
	BlockQueue<int>* bq = (BlockQueue<int>*)arg;
	//生产者不断进行生产
	while (true){
		sleep(1);
		int data = rand() % 100 + 1;
		bq->Push(data); //生产数据
		std::cout << "Producer: " << data << std::endl;
	}
}
void* Consumer(void* arg)
{
	BlockQueue<int>* bq = (BlockQueue<int>*)arg;
	//消费者不断进行消费
	while (true){
		sleep(1);
		int data = 0;
		bq->Pop(data); //消费数据
		std::cout << "Consumer: " << data << std::endl;
	}
}
int main()
{
	srand((unsigned int)time(nullptr));
	pthread_t producer, consumer;
	BlockQueue<int>* bq = new BlockQueue<int>;
	//创建生产者线程和消费者线程
	pthread_create(&producer, nullptr, Producer, bq);
	pthread_create(&consumer, nullptr, Consumer, bq);

	//join生产者线程和消费者线程
	pthread_join(producer, nullptr);
	pthread_join(consumer, nullptr);
	delete bq
	return 0;
}

void* Producer(void* arg)
{
	BlockQueue<int>* bq = (BlockQueue<int>*)arg;
	//生产者不断进行生产
	while (true){
		int data = rand() % 100 + 1;
		bq->Push(data); //生产数据
		std::cout << "Producer: " << data << std::endl;
	}
}


void* Consumer(void* arg)
{
	BlockQueue<int>* bq = (BlockQueue<int>*)arg;
	//消费者不断进行消费
	while (true){
		sleep(1);
		int data = 0;
		bq->Pop(data); //消费数据
		std::cout << "Consumer: " << data << std::endl;
	}
}


void* Producer(void* arg)
{
	BlockQueue<int>* bq = (BlockQueue<int>*)arg;
	//生产者不断进行生产
	while (true){
		sleep(1);
		int data = rand() % 100 + 1;
		bq->Push(data); //生产数据
		std::cout << "Producer: " << data << std::endl;
	}
}


void* Consumer(void* arg)
{
	BlockQueue<int>* bq = (BlockQueue<int>*)arg;
	//消费者不断进行消费
	while (true){
		int data = 0;
		bq->Pop(data); //消费数据
		std::cout << "Consumer: " << data << std::endl;
	}
}


//向阻塞队列插入数据（生产者调用）
void Push(const T& data)
{
	pthread_mutex_lock(&_mutex);
	while (IsFull()){
		//不能进行生产，直到阻塞队列可以容纳新的数据
		pthread_cond_wait(&_full, &_mutex);
	}
	_q.push(data);
	if (_q.size() >= _cap / 2){
		pthread_cond_signal(&_empty); //唤醒在empty条件变量下等待的消费者线程
	}
	pthread_mutex_unlock(&_mutex);
}


//从阻塞队列获取数据（消费者调用）
void Pop(T& data)
{
	pthread_mutex_lock(&_mutex);
	while (IsEmpty()){
		//不能进行消费，直到阻塞队列有新的数据
		pthread_cond_wait(&_empty, &_mutex);
	}
	data = _q.front();
	_q.pop();
	if (_q.size() <= _cap / 2){
		pthread_cond_signal(&_full); //唤醒在full条件变量下等待的生产者线程
	}
	pthread_mutex_unlock(&_mutex);
}


#pragma once
#include <iostream>

class Task
{
public:
	Task(int x = 0, int y = 0, int op = 0)
		: _x(x), _y(y), _op(op)
	{}
	~Task()
	{}
	void Run()
	{
		int result = 0;
		switch (_op)
		{
		case '+':
			result = _x + _y;
			break;
		case '-':
			result = _x - _y;
			break;
		case '*':
			result = _x * _y;
			break;
		case '/':
			if (_y == 0){
				std::cout << "Warning: div zero!" << std::endl;
				result = -1;
			}
			else{
				result = _x / _y;
			}
			break;
		case '%':
			if (_y == 0){
				std::cout << "Warning: mod zero!" << std::endl;
				result = -1;
			}
			else{
				result = _x % _y;
			}
			break;
		default:
			std::cout << "error operation!" << std::endl;
			break;
		}
		std::cout << _x << _op << _y << "=" << result << std::endl;
	}
private:
	int _x;
	int _y;
	char _op;
};



void* Producer(void* arg)
{
	BlockQueue<Task>* bq = (BlockQueue<Task>*)arg;
	const char* arr = "+-*/%";
	//生产者不断进行生产
	while (true){
		int x = rand() % 100;
		int y = rand() % 100;
		char op = arr[rand() % 5];
		Task t(x, y, op);
		bq->Push(t); //生产数据
		std::cout << "producer task done" << std::endl;
	}
}
void* Consumer(void* arg)
{
	BlockQueue<Task>* bq = (BlockQueue<Task>*)arg;
	//消费者不断进行消费
	while (true){
		sleep(1);
		Task t;
		bq->Pop(t); //消费数据
		t.Run(); //处理数据
	}
}
