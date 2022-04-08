#pragma once

#include <list>
#include <memory>

template<typename T>
class SwappableComponent {
private:
	std::shared_ptr<std::vector<T>> list;
	int index;
public:
	SwappableComponent() {
		list = std::make_shared<std::vector<T>>();
		index = 0;
	}

	std::vector<T>* getComponentList() {
		return list.get();
	}

	T getActiveComponent() {
		std::vector<T>& vec = *getComponentList();
		return vec.at(index);
	}

	void pushComponent(T t) {
		getComponentList()->push_back(t);
	}

	int getIndex() {
		return index;
	}

	void setIndex(int index) {
		this->index = index;
	}
};