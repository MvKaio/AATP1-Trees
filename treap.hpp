#ifndef TREAP_HPP
#define TREAP_HPP

template<typename T>
class Treap {
  public:
	Treap();
	unsigned size();
	bool empty();
	bool insert(const T& value);
	bool erase(const T& value);
	bool contains(const T& value);
	void split(const T& value, Treap<T>& other);
	void join(Treap<T>& other);
  private:
	struct Node {
		T value;
		Node *left, *right;
		Node(T _value = T()) : value(_value), left(nullptr), right(nullptr) {}
	};
	Node *root;
	unsigned _size;
};

template<typename T>
Treap<T>::Treap() : root(nullptr) {}

template<typename T>
unsigned Treap<T>::size() {
	return _size;
}

template<typename T>
bool Treap<T>::empty() {
	return _size == 0;
}

template<typename T>
bool Treap<T>::insert(const T& value) {
	Node *at = root;

	while (at != nullptr) {
		if (value == at->value) return false;
		if (value < at->value)
			at = at->left;
		else
			at = at->left;
	}

	at = new Node(value);
	this->_size += 1;
}

template<typename T>
bool Treap<T>::erase(const T& value) {

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
