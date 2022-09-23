#ifndef SPLAY_TREE_HPP
#define SPLAY_TREE_HPP

#include <utility>
#include <tuple>
#include <cassert>
#include <iostream>
#include <stack>

template<typename T>
struct Node {
	T value;
	unsigned size;
	Node *parent, *left, *right;
	Node(T _value = T()) : value(_value), size(1), 
						   parent(nullptr), left(nullptr), right(nullptr) {}
	void update_size();
	void set_left(Node* x);
	void set_right(Node* x);
};

template<typename T>
class SplayTree {
  public:
	SplayTree();
	~SplayTree();
	unsigned size();
	bool empty();
	bool insert(const T& value);
	void erase(const T& value);
	bool contains(const T& value);
	void split(const T& value, SplayTree<T>& other, bool after=false);
	void join(SplayTree<T>& other);

  private:
	Node<T>* root;
	static void grab_pointers(std::stack<Node<T>*>&, Node<T>*);
};

template<typename T>
void rotate(Node<T>*& x);

template<typename T>
void splay(Node<T>*& x);

template<typename T>
Node<T>* successor(Node<T>* root, const T& value);

///////// Implementation Starts Here


template<typename T>
void SplayTree<T>::grab_pointers(std::stack<Node<T>*>& stk, Node<T>* at) {
	if (at == nullptr) return;
	grab_pointers(stk, at->left);
	stk.push(at);
	grab_pointers(stk, at->right);
}

template<typename T>
SplayTree<T>::~SplayTree() {
	std::stack<Node<T>*> pointers;
	grab_pointers(pointers, this->root);
	while (not pointers.empty()) {
		delete pointers.top();
		pointers.pop();
	}
}

template<typename T>
unsigned get_size(Node<T>* node) {
	return (node == nullptr ? 0 : node->size);
}

template<typename T>
void Node<T>::update_size() {
	this->size = get_size(left) + 1 + get_size(right);
}

template<typename T>
void Node<T>::set_right(Node<T>* x) {
	this->right = x;
	if (x != nullptr)
		x->parent = this;
	update_size();
}

template<typename T>
void Node<T>::set_left(Node<T>* x) {
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
		x->set_right(x->parent);
		if (pp) {
			if (pp->value > x->value)
				pp->set_left(x);
			else
				pp->set_right(x);
		} else {
			x->parent = pp;
		}
	} else {
		x->parent->set_right(x->left);
		x->set_left(x->parent);
		if (pp) {
			if (pp->value > x->value)
				pp->set_left(x);
			else
				pp->set_right(x);
		} else {
			x->parent = pp;
		}
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
	Node<T>* at = this->root;
	Node<T>* x = new Node<T>(value);

	if (at == nullptr) {
		this->root = x;
		return true;
	}

	while (true) {
		if (value == at->value) {
			delete x;
			return false;
		}
		if (value < at->value) {
			if (at->left == nullptr) {
				at->set_left(x);
				this->root = x;
				splay(this->root);
				return true;
			}
			at = at->left;
		}
		else {
			if (at->right == nullptr) {
				at->set_right(x);
				this->root = x;
				splay(this->root);
				return true;
			}
			at = at->right;
		}
	}
}

template<typename T>
Node<T>* successor(Node<T>* root, const T& value) {
	if (root == nullptr) return nullptr;
	if (root->value > value) {
		Node<T>* left_succ = successor(root->left, value);
		if (left_succ) return left_succ;
		else return root;
	} else {
		return successor(root->right, value);
	}
}

using std::cerr, std::endl;
template<typename T>
Node<T>* join_aux(Node<T>* left, Node<T>* right) {
	if (right == nullptr) return left;
	if (left == nullptr) return right;

	Node<T>* at = right;
	while (at->left != nullptr) 
		at = at->left;
	Node<T>* min_right = at->left;
	if (min_right == nullptr)
		min_right = at;
	cerr << "right -> " << get_size(right) << endl;
	splay(min_right);
	cerr << "min_right -> " << get_size(min_right) << endl;
	min_right->set_left(left);

	return min_right;
}

template<typename T>
void SplayTree<T>::erase(const T& value) {
	Node<T>* at = root;
	while (at != nullptr && at->value != value) {
		if (at->value == value) break;
		if (value < at->value)
			at = at->left;
		else
			at = at->right;
	}

	if (at == nullptr) return;

	splay(at);
	if (at->left)
		at->left->parent = nullptr;
	if (at->right)
		at->right->parent = nullptr;
	this->root = join_aux(at->left, at->right);

	std::cerr << at << std::endl;
	delete at;
}

template<typename T>
void SplayTree<T>::join(SplayTree<T>& other) {
	this->root = join_aux(this->root, other.root);
	other.root = nullptr;
}

template<typename T>
void SplayTree<T>::split(const T& value, SplayTree<T>& other, bool after) {
	Node<T>* succ = successor(this->root, value);
	if (succ) {
		splay(succ);
		other.root = succ;
		this->root = succ->left;
		if (this->root)
			this->root->parent = nullptr;
		other.root->left = nullptr;
		other.root->update_size();
	} else {
		other = SplayTree<T>();
	}
	if (after == false && this->contains(value)) {
		this->erase(value);
		other.insert(value);
	} 
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
