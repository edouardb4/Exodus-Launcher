#pragma once

#include <list>
#include <iostream>

template<typename T>
class Queue {
private:
	std::shared_ptr<std::list<T>> queue;
public:
	Queue() {
		queue = std::make_shared<std::list<T>>();
	}
	bool isEmpty() {
		return queue->size() == 0;
	}
	void push(T t) {
		queue->push_back(t);
	}
	T peek() {
		return queue->front();
	}
	T pop() {
		T t = peek();
		queue->remove(t);
		return t;
	}
};