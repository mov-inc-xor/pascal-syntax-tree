#pragma once

template <typename T>
class Node {
private:
	T data;
	Node* next;
public:
	//Node() : next(nullptr) {}
	explicit Node(T data) : data(data), next(nullptr) {}
	virtual ~Node() = default;

	T getData();
	Node* getNext();
	void setNext(Node* nxt);
};

template <typename T>
T Node<T>::getData() {
	return data;
}

template <typename T>
Node<T>* Node<T>::getNext() {
	return next;
}

template <typename T>
void Node<T>::setNext(Node* nxt) {
	next = nxt;
}

