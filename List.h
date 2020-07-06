#pragma once

#include "Node.h"
#include "ListExceptions.h"

template<typename T>
class List {
private:
	Node<T>* headNode;
	size_t sz;
public:
	List() : headNode(nullptr), sz(0) {};
	virtual ~List();

	T head() const;
	void append(T data);
	void remove();
	size_t size() const;
	Node<T>* getHead() const;
};

template<typename T>
List<T>::~List() {
	while (headNode) {
		remove();
	}
}

template<typename T>
T List<T>::head() const {
	if (!headNode) {
		throw ListEmptyException();
	}
	return headNode->getData();
}

template<typename T>
void List<T>::append(T data) {
	auto* node = new Node<T>(data);
	node->setNext(headNode);
	headNode = node;
	sz++;
}

template<typename T>
void List<T>::remove() {
	if (!headNode) {
		throw ListEmptyException();
	}
	Node<T>* newHead = headNode->getNext();
	headNode = newHead;
	sz--;
}

template<typename T>
size_t List<T>::size() const {
	return sz;
}

template<typename T>
Node<T>* List<T>::getHead() const {
	return headNode;
}
