#ifndef TREAP_HPP
#define TREAP_HPP

#include <chrono>
#include <random>

std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

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

	struct Node {
    unsigned priority;
		T value;
		Node *left, *right;
		Node(T _value = T()) : priority(rng()), value(_value), left(nullptr), right(nullptr) {}
	};

  private:
	Node *root;
	unsigned _size;
};

template<typename T>
typename Treap<T>::Node* join(typename Treap<T>::Node *left, typename Treap<T>::Node *right);

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
}

template<typename T>
bool Treap<T>::erase(const T& value) {

}

template<typename T>
bool Treap<T>::contains(const T& value) {
	Node *at = root;

	while (at != nullptr) {
		if (value == at->value) return true;
		if (value < at->value)
			at = at->left;
		else
			at = at->left;
	}

  return false;
}

template<typename T>
void Treap<T>::split(const T& value, Treap<T>& other) {

}

template<typename T>
typename Treap<T>::Node* join(typename Treap<T>::Node *left, typename Treap<T>::Node *right) {
  if (left == nullptr) return right;
  if (right == nullptr) return left;
  if (left.priority > right.priority) {
    typename Treap<T>::Node* result = join(left->right, right);
    left->right = result;
  } else {
    typename Treap<T>::Node* result = join(left, right->left);
    right->left = result;
  }
}

template<typename T>
void Treap<T>::join(Treap<T>& other) {
  join(this->root, other->root);
}


#endif
