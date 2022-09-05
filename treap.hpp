#ifndef TREAP_HPP
#define TREAP_HPP

template<typename T>
class Treap {
  public:
	Treap();
	unsigned size();
	bool empty();
	void insert(const T& value);
	void erase(const T& value);
	bool contains(const T& value);
	void split(const T& value, Treap<T>& other);
	void join(Treap<T>& other);
  private:
	struct Node {
		T value;
		Node *left, *right;
	};
	Node *root;
};

template<typename T>
Treap<T>::Treap() {}

template<typename T>
unsigned Treap<T>::size() {

}

template<typename T>
bool Treap<T>::empty() {

}

template<typename T>
void Treap<T>::insert(const T& value) {

}

template<typename T>
void Treap<T>::erase(const T& value) {

}

template<typename T>
bool Treap<T>::contains(const T& value) {

}

template<typename T>
void Treap<T>::split(const T& value, Treap<T>& other) {

}

template<typename T>
void Treap<T>::join(Treap<T>& other) {

}

#endif
