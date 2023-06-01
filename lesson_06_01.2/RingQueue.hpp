#pragma once

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>

#define NUM 8

template<class T>
class RingQueue
{
private:
	//P操作
	void P(sem_t& s)
	{
		sem_wait(&s);
	}
	//V操作
	void V(sem_t& s)
	{
	    sem_post(&s);
	}
public:
	RingQueue(int cap = NUM)
		: _cap(cap), _p_pos(0), _c_pos(0)
	{
		_q.resize(_cap);
		sem_init(&_blank_sem, 0, _cap); //blank_sem初始值设置为环形队列的容量
		sem_init(&_data_sem, 0, 0); //data_sem初始值设置为0
	}
	~RingQueue()
	{
	sem_destroy(&_blank_sem);
	sem_destroy(&_data_sem);
	}
	//向环形队列插入数据（生产者调用）
	void Push(const T& data)
	{
		P(_blank_sem); //生产者关注空间资源
		_q[_p_pos] = data;
		V(_data_sem); //生产

		//更新下一次生产的位置
		_p_pos++;
		_p_pos %= _cap;
	}
	//从环形队列获取数据（消费者调用）
	void Pop(T& data)
	{
		P(_data_sem); //消费者关注数据资源
		data = _q[_c_pos];
		V(_blank_sem);

		//更新下一次消费的位置
		_c_pos++;
		_c_pos %= _cap;
	}
private:
	std::vector<T> _q; //环形队列
	int _cap; //环形队列的容量上限
	int _p_pos; //生产位置
	int _c_pos; //消费位置
	sem_t _blank_sem; //描述空间资源
	sem_t _data_sem; //描述数据资源
};
