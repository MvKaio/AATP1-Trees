#ifndef SPLAY_TREE_HPP
#define SPLAY_TREE_HPP

#include <utility>
#include <tuple>
#include <cassert>
#include <iostream>
#include <stack>

template<typename T>
class SplayTree {
  public:
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
	Node* root;
	static void grab_pointers(std::stack<Node*>&, Node*);
};

template<typename T>
using SNode = typename SplayTree<T>::Node;

namespace __splay_helper_methods {
	template<typename T>
	unsigned get_size(SNode<T>* node);

	template<typename T>
	void rotate(SNode<T>*& x);

	template<typename T>
	void splay(SNode<T>*& x);

	template<typename T>
	SNode<T>* join_aux(SNode<T>* left, SNode<T>* right);

	template<typename T>
	SNode<T>* successor(SNode<T>* root, const T& value);
}

///////// Implementation Starts Here

template<typename T>
void SplayTree<T>::grab_pointers(std::stack<SplayTree<T>::Node*>& stk, SplayTree<T>::Node* at) {
	if (at == nullptr) return;
	grab_pointers(stk, at->left);
	stk.push(at);
	grab_pointers(stk, at->right);
}

template<typename T>
SplayTree<T>::~SplayTree() {
	std::stack<SNode<T>*> pointers;
	grab_pointers(pointers, this->root);
	while (not pointers.empty()) {
		delete pointers.top();
		pointers.pop();
	}
}

template<typename T>
unsigned __splay_helper_methods::get_size(SNode<T>* node) {
	return (node == nullptr ? 0 : node->size);
}

template<typename T>
void SplayTree<T>::Node::update_size() {
	this->size = __splay_helper_methods::get_size<T>(left) + 1 + __splay_helper_methods::get_size<T>(right);
}

template<typename T>
void SplayTree<T>::Node::set_right(SplayTree<T>::Node* x) {
	this->right = x;
	if (x != nullptr)
		x->parent = this;
	update_size();
}

template<typename T>
void SplayTree<T>::Node::set_left(SplayTree<T>::Node* x) {
	this->left = x;
	if (x != nullptr)
		x->parent = this;
	update_size();
}

template<typename T>
void __splay_helper_methods::rotate(SNode<T>*& x) {
	if (x == nullptr) return;
	if (x->parent == nullptr) return;

	SNode<T>* pp = x->parent->parent;

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
void __splay_helper_methods::splay(SNode<T>*& x) {
	if (x == nullptr) return;
	while (x->parent != nullptr) {
		if (x->parent->parent == nullptr) { // zig
			__splay_helper_methods::rotate<T>(x);
		} else {
			bool left_child = (x->parent->left == x);
			bool left_parent = (x->parent->parent->left == x->parent);

			if (left_child == left_parent) { // zigzag
				__splay_helper_methods::rotate<T>(x->parent);
				__splay_helper_methods::rotate<T>(x);
			} else { // zigzig
				__splay_helper_methods::rotate<T>(x);
				__splay_helper_methods::rotate<T>(x);
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
	SNode<T>* at = this->root;
	SNode<T>* x = new SNode<T>(value);

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
				__splay_helper_methods::splay<T>(this->root);
				return true;
			}
			at = at->left;
		}
		else {
			if (at->right == nullptr) {
				at->set_right(x);
				this->root = x;
				__splay_helper_methods::splay<T>(this->root);
				return true;
			}
			at = at->right;
		}
	}
}

template<typename T>
SNode<T>* successor(SNode<T>* root, const T& value) {
	if (root == nullptr) return nullptr;
	if (root->value > value) {
		SNode<T>* left_succ = successor(root->left, value);
		if (left_succ) return left_succ;
		else return root;
	} else {
		return successor(root->right, value);
	}
}

template<typename T>
SNode<T>* __splay_helper_methods::join_aux(SNode<T>* left, SNode<T>* right) {
	if (right == nullptr) return left;
	if (left == nullptr) return right;

	SNode<T>* at = right;
	while (at->left != nullptr) 
		at = at->left;
	SNode<T>* min_right = at->left;
	if (min_right == nullptr)
		min_right = at;
	__splay_helper_methods::splay<T>(min_right);
	min_right->set_left(left);

	return min_right;
}

template<typename T>
void SplayTree<T>::erase(const T& value) {
	SNode<T>* at = root;
	while (at != nullptr && at->value != value) {
		if (at->value == value) break;
		if (value < at->value)
			at = at->left;
		else
			at = at->right;
	}

	if (at == nullptr) return;

	__splay_helper_methods::splay<T>(at);
	if (at->left)
		at->left->parent = nullptr;
	if (at->right)
		at->right->parent = nullptr;
	this->root = __splay_helper_methods::join_aux<T>(at->left, at->right);

	delete at;
}

template<typename T>
void SplayTree<T>::join(SplayTree<T>& other) {
	this->root = __splay_helper_methods::join_aux<T>(this->root, other.root);
	other.root = nullptr;
}

template<typename T>
void SplayTree<T>::split(const T& value, SplayTree<T>& other, bool after) {
	SNode<T>* succ = successor(this->root, value);
	if (succ) {
		__splay_helper_methods::splay<T>(succ);
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
	SNode<T> *at = root;

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
