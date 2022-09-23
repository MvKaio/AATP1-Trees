#ifndef TREAP_HPP
#define TREAP_HPP

#include <utility>
#include <tuple>
#include <chrono>
#include <random>
#include <stack>

template<typename T>
class Treap {
  public:
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
	Treap<T>::Node *root;
	static void grab_pointers(std::stack<Node*>&, Node*);
};

template<typename T>
using TNode = typename Treap<T>::Node;

template<typename T>
std::mt19937 Treap<T>::Node::rng(std::chrono::system_clock::now().time_since_epoch().count());

namespace helper_methods {
	template<typename T>
	unsigned get_size(TNode<T>* node);

	template<typename T>
	TNode<T>* join_aux(TNode<T> *left, TNode<T> *right);

	template<typename T>
	std::pair<TNode<T>*, TNode<T>*> split_before(const T& value, TNode<T> *tree);

	template<typename T>
	std::pair<TNode<T>*, TNode<T>*> split_after(const T& value, TNode<T> *tree);
}

///////// Implementation Starts Here

template<typename T>
void Treap<T>::grab_pointers(std::stack<Treap<T>::Node*>& stk, Treap<T>::Node* at) {
	if (at == nullptr) return;
	grab_pointers(stk, at->left);
	stk.push(at);
	grab_pointers(stk, at->right);
}

template<typename T>
Treap<T>::~Treap<T>() {
	std::stack<TNode<T>*> pointers;
	grab_pointers(pointers, this->root);
	while (not pointers.empty()) {
		delete pointers.top();
		pointers.pop();
	}
}

template<typename T>
unsigned helper_methods::get_size(TNode<T> *node) {
	return (node == nullptr ? 0 : node->size);
}

template<typename T>
void Treap<T>::Node::update_size() {
	this->size = helper_methods::get_size<T>(left) + 1 + helper_methods::get_size<T>(right);
}

template<typename T>
void Treap<T>::Node::set_right(Treap<T>::Node* x) {
	right = x;
	update_size();
}

template<typename T>
void Treap<T>::Node::set_left(Treap<T>::Node* x) {
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
	unit.root = new TNode<T>(value);

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
	TNode<T> *at = root;

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
std::pair<TNode<T>*, TNode<T>*> split_before(const T& value, TNode<T> *tree) {
	if (tree == nullptr) return std::make_pair(nullptr, nullptr);
	TNode<T> *left, *right;
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
std::pair<TNode<T>*, TNode<T>*> split_after(const T& value, TNode<T> *tree) {
	if (tree == nullptr) return std::make_pair(nullptr, nullptr);
	TNode<T> *left, *right;
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
	TNode<T> *left, *right;
	if (after)
		std::tie(left, right) = split_after(value, this->root);
	else
		std::tie(left, right) = split_before(value, this->root);
	this->root = left;
	other.root = right;
}

template<typename T>
TNode<T>* helper_methods::join_aux(TNode<T>* left, TNode<T>* right) {
	if (left == nullptr) return right;
	if (right == nullptr) return left;
	if (left->priority > right->priority) {
		TNode<T>* result = join_aux<T>(left->right, right);
		left->set_right(result);
		return left;
	} else {
		TNode<T>* result = join_aux<T>(left, right->left);
		right->set_left(result);
		return right;
	}
}

template<typename T>
void Treap<T>::join(Treap<T>& other) {
	this->root = helper_methods::join_aux<T>(this->root, other.root);
	other.root = nullptr;
}

#endif
