#ifndef TREAP_HPP
#define TREAP_HPP

#include <utility>
#include <tuple>
#include <chrono>
#include <random>

template<typename T>
struct Node {
	T value;
	unsigned priority, size;
	Node *left, *right;
	Node(T _value = T()) : value(_value), priority(rng()), size(1), 
												 left(nullptr), right(nullptr) {}
	
	void update_size() {
		this->size = get_size(left) + 1 + get_size(right);
	}

	void set_right(Node* x) {
		right = x;
		update_size();
	}

	void set_left(Node* x) {
		right = x;
		update_size();
	}

	static std::mt19937 rng;
};

template<typename T>
std::mt19937 Node<T>::rng(std::chrono::system_clock::now().time_since_epoch().count());

template<typename T>
unsigned get_size(Node<T> *node) {
	return (node == nullptr ? 0 : node->size);
}

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
	Node<T> *root;
};

template<typename T>
Node<T> join(Node<T> *left, Node<T> *right);

template<typename T>
Treap<T>::Treap() : root(nullptr) {}

template<typename T>
unsigned Treap<T>::size() {
	return (this->root == nullptr ? 0 : this->root->size);
}

template<typename T>
bool Treap<T>::empty() {
	return (this->root == nullptr);
}

template<typename T>
bool Treap<T>::insert(const T& value) {
	if (this->contains(value)) return false;

	Treap<T> unit, other;
	this->split(value, other);
	unit.root = new Node<T>(value);

	this->join(unit);
	this->join(other);
}

template<typename T>
bool Treap<T>::erase(const T& value) {
	Treap<T> other;
	this->split(value, other);
}

template<typename T>
bool Treap<T>::contains(const T& value) {
	Node<T> *at = root;

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
std::pair<Node<T>*, Node<T>*> split(const T& value, Node<T> *tree) {
	if (tree == nullptr) return std::make_pair(nullptr, nullptr);
	Node<T> *left, *right;
	if (tree->value < value) {
		std::tie(left, right) = split(value, tree->right);
		tree->set_right(left);
		return std::make_pair(tree, right);
	} else {
		std::tie(left, right) = split(value, tree->left);
		tree->set_left(right);
		return std::make_pair(left, tree);
	}
}

template<typename T>
void Treap<T>::split(const T& value, Treap<T>& other) {
	Node<T> *left, *right;
	std::tie(left, right) = split(value, this->root);
	this->root = left;
	other.root = right;
}

template<typename T>
Node<T>* join(Node<T> *left, Node<T> *right) {
  if (left == nullptr) return right;
  if (right == nullptr) return left;
  if (left->priority > right->priority) {
		Node<T>* result = join(left->right, right);
		left->set_right(result);
  } else {
		Node<T>* result = join(left, right->left);
		right->set_left(result);
  }
}

template<typename T>
void Treap<T>::join(Treap<T>& other) {
  this->root = join(this->root, other.root);
	other.root = nullptr;
}

#endif
