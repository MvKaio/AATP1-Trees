#ifndef TREAP_HPP
#define TREAP_HPP

#include <utility>
#include <tuple>
#include <chrono>
#include <random>
#include <stack>

template<typename T>
struct Node {
	T value;
	unsigned priority, size;
	Node *left, *right;
	Node(T _value = T()) : value(_value), priority(rng()), size(1), 
						   left(nullptr), right(nullptr) {}
	void update_size();
	void set_left(Node* x);
	void set_right(Node* x);

	static std::mt19937 rng;
};

template<typename T>
std::mt19937 Node<T>::rng(std::chrono::system_clock::now().time_since_epoch().count());

template<typename T>
class Treap {
  public:
	Treap();
	~Treap();
	unsigned size();
	bool empty();
	bool insert(const T& value);
	void erase(const T& value);
	bool contains(const T& value);
	void split(const T& value, Treap<T>& other, bool after=false);
	void join(Treap<T>& other);

  private:
	Node<T> *root;
	static void grab_pointers(std::stack<Node<T>*>&, Node<T>*);
};

template<typename T>
Node<T>* join_aux(Node<T> *left, Node<T> *right);

template<typename T>
std::pair<Node<T>*, Node<T>*> split_before(const T& value, Node<T> *tree);

template<typename T>
std::pair<Node<T>*, Node<T>*> split_after(const T& value, Node<T> *tree);

///////// Implementation Starts Here

template<typename T>
void Treap<T>::grab_pointers(std::stack<Node<T>*>& stk, Node<T>* at) {
	if (at == nullptr) return;
	grab_pointers(stk, at->left);
	stk.push(at);
	grab_pointers(stk, at->right);
}

template<typename T>
Treap<T>::~Treap<T>() {
	std::stack<Node<T>*> pointers;
	grab_pointers(pointers, this->root);
	while (not pointers.empty()) {
		delete pointers.top();
		pointers.pop();
	}
}

template<typename T>
unsigned get_size(Node<T> *node) {
	return (node == nullptr ? 0 : node->size);
}

template<typename T>
void Node<T>::update_size() {
	this->size = get_size(left) + 1 + get_size(right);
}

template<typename T>
void Node<T>::set_right(Node* x) {
	right = x;
	update_size();
}

template<typename T>
void Node<T>::set_left(Node* x) {
	left = x;
	update_size();
}

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

	return true;
}

template<typename T>
void Treap<T>::erase(const T& value) {
	Treap<T> singleton, other;
	this->split(value, singleton);
	singleton.split(value, other, true);
	this->join(other);
}

template<typename T>
bool Treap<T>::contains(const T& value) {
	Node<T> *at = root;

	while (at != nullptr) {
		if (value == at->value) return true;
		if (value < at->value)
			at = at->left;
		else
			at = at->right;
	}

  return false;
}

template<typename T>
std::pair<Node<T>*, Node<T>*> split_before(const T& value, Node<T> *tree) {
	if (tree == nullptr) return std::make_pair(nullptr, nullptr);
	Node<T> *left, *right;
	if (tree->value < value) {
		std::tie(left, right) = split_before(value, tree->right);
		tree->set_right(left);
		return std::make_pair(tree, right);
	} else {
		std::tie(left, right) = split_before(value, tree->left);
		tree->set_left(right);
		return std::make_pair(left, tree);
	}
}

template<typename T>
std::pair<Node<T>*, Node<T>*> split_after(const T& value, Node<T> *tree) {
	if (tree == nullptr) return std::make_pair(nullptr, nullptr);
	Node<T> *left, *right;
	if (tree->value <= value) {
		std::tie(left, right) = split_after(value, tree->right);
		tree->set_right(left);
		return std::make_pair(tree, right);
	} else {
		std::tie(left, right) = split_after(value, tree->left);
		tree->set_left(right);
		return std::make_pair(left, tree);
	}
}

template<typename T>
void Treap<T>::split(const T& value, Treap<T>& other, bool after) {
	Node<T> *left, *right;
	if (after)
		std::tie(left, right) = split_after(value, this->root);
	else
		std::tie(left, right) = split_before(value, this->root);
	this->root = left;
	other.root = right;
}

template<typename T>
Node<T>* join_aux(Node<T> *left, Node<T> *right) {
	if (left == nullptr) return right;
	if (right == nullptr) return left;
	if (left->priority > right->priority) {
		Node<T>* result = join_aux(left->right, right);
		left->set_right(result);
		return left;
	} else {
		Node<T>* result = join_aux(left, right->left);
		right->set_left(result);
		return right;
	}
}

template<typename T>
void Treap<T>::join(Treap<T>& other) {
	this->root = join_aux(this->root, other.root);
	other.root = nullptr;
}

#endif
