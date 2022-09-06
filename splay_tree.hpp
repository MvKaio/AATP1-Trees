#ifndef SPLAY_TREE_HPP
#define SPLAY_TREE_HPP

#include <utility>
#include <tuple>
#include <cassert>

template<typename T>
struct Node {
	T value;
	unsigned size;
	Node *parent, *left, *right;
	Node(T _value = T()) : value(_value), size(1), 
						   parent(nullptr), left(nullptr), right(nullptr) {}
	void update_size();
	void set_left(Node*& x);
	void set_right(Node*& x);
};

template<typename T>
class SplayTree {
  public:
	SplayTree();
	unsigned size();
	bool empty();
	bool insert(const T& value);
	void erase(const T& value);
	bool contains(const T& value);
	void split(const T& value, SplayTree<T>& other, bool after=false);
	void join(SplayTree<T>& other);

  private:
	Node<T> *root;
};

template<typename T>
void rotate(Node<T>*&x);

template<typename T>
void splay(Node<T>*&x);

///////// Implementation Starts Here

template<typename T>
unsigned get_size(Node<T> *node) {
	return (node == nullptr ? 0 : node->size);
}

template<typename T>
void Node<T>::update_size() {
	this->size = get_size(left) + 1 + get_size(right);
}

template<typename T>
void Node<T>::set_right(Node<T>*& x) {
	this->right = x;
	if (x != nullptr)
		x->parent = this;
	update_size();
}

template<typename T>
void Node<T>::set_left(Node<T>*& x) {
	this->left = x;
	if (x != nullptr)
		x->parent = this;
	update_size();
}

template<typename T>
void rotate(Node<T>*& x) {
	if (x == nullptr) return;
	if (x->parent == nullptr) return;
	Node<T>* pp = x->parent->parent;
	if (x == x->parent->left) {
		x->parent->set_left(x->right);
		if (x->right)
			assert(x->right->parent == x->parent);
		x->set_right(x->parent);
		x->parent = pp;
	} else {
		assert(x->parent->right == x);
		x->parent->set_right(x->left);
		x->set_left(x->parent);
		x->parent = pp;
	}
}

template<typename T>
void splay(Node<T>*& x) {
	if (x == nullptr) return;
	while (x->parent != nullptr) {
		if (x->parent->parent == nullptr) { // zig
			rotate(x);
		} else {
			bool left_child = (x->parent->left == x);
			bool left_parent = (x->parent->parent->left == x->parent);

			if (left_child == left_parent) { // zigzag
				rotate(x->parent);
				rotate(x);
			} else { // zigzig
				rotate(x);
				rotate(x);
			}
		}
	}
}

template<typename T>
SplayTree<T>::SplayTree() : root(nullptr) {}

template<typename T>
unsigned SplayTree<T>::size() {
	return (this->root == nullptr ? 0 : this->root->size);
}

template<typename T>
bool SplayTree<T>::empty() {
	return (this->root == nullptr);
}

template<typename T>
bool SplayTree<T>::insert(const T& value) {
	Node<T> *at = this->root;
	Node<T> *x = new Node<T>(value);

	if (at == nullptr) {
		this->root = x;
		return true;
	}

	while (true) {
		if (value == at->value) return false;
		if (value < at->value) {
			if (at->left == nullptr) {
				at->set_left(x);
				splay(x);
				this->root = x;
				return true;
			}
			at = at->left;
		}
		else {
			if (at->right == nullptr) {
				at->set_right(x);
				splay(x);
				this->root = x;
				return true;
			}
			at = at->right;
		}
	}
}

template<typename T>
void SplayTree<T>::erase(const T& value) {
	// TODO: Learn and implement this
	// I can only do it for leaves
}

template<typename T>
bool SplayTree<T>::contains(const T& value) {
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

#endif
