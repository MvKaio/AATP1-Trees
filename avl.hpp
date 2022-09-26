#ifndef AVL_HPP
#define AVL_HPP

#include <utility>
#include <iostream>
#include <stdexcept>
#include <stack>

template<typename T>
class AVL {
  public:
	struct Node {
		T value;
		unsigned size;
		unsigned height;
		Node *left, *right;
		Node(T _value = T()) : value(_value), size(1), height(1), 
							   left(nullptr), right(nullptr) {}
		void update_parameters();
		void set_left(Node* x);
		void set_right(Node* x);
	};

	AVL();
	AVL(Node*);
	~AVL();
	unsigned height();
	unsigned size();
	bool empty();
	bool insert(const T& value);
	bool erase(const T& value);
	bool contains(const T& value);
	bool join_aux(Node* other);
	bool join(AVL<T>& other);
	std::pair<bool,Node*> split(const T& value);
	void print();

  private:
	static void grab_pointers(std::stack<Node*>& stk, Node* at);
	Node* rebalance(Node* p);
	Node* rotate_right(Node* p);
	Node* rotate_left(Node* p);
	Node* rotate_right_left(Node* p);
	Node* rotate_left_right(Node* p);
	Node* insert(Node* p, const T& value);
	Node* erase(Node* p, const T& value, Node* par);
	Node* join_right(Node* l, const T& k, Node* r);
	Node* join_left(Node* l, const T& k, Node* r);
	Node* join(Node* l, const T& k, Node* r);
	Node* split(Node* p, const T& value);
	void print(const std::string& prefix, Node* p, bool isLeft);
	Node *root;
};

// NODE
// ----

template<typename T>
int get_height(typename AVL<T>::Node *node) {
	return (node == nullptr ? 0 : (int) node->height);
}

template<typename T>
int get_size(typename AVL<T>::Node *node) {
	return (node == nullptr ? 0 : (int) node->size);
}

template<typename T>
void AVL<T>::Node::update_parameters() {
	this->size = 1 + get_size<T>(left) + get_size<T>(right);
	this->height = 1 + std::max(get_height<T>(left), get_height<T>(right));
}

template<typename T>
void AVL<T>::Node::set_right(AVL<T>::Node* x) {
	right = x;
	update_parameters();
}

template<typename T>
void AVL<T>::Node::set_left(AVL<T>::Node* x) {
	left = x;
	update_parameters();
}

// AVL
// ---

template<typename T>
void AVL<T>::grab_pointers(std::stack<AVL<T>::Node*>& stk, AVL<T>::Node* at) {
	if (at == nullptr) return;
	grab_pointers(stk, at->left);
	stk.push(at);
	grab_pointers(stk, at->right);
}

template<typename T>
AVL<T>::AVL() : root(nullptr) {}

template<typename T>
AVL<T>::AVL(AVL<T>::Node* at) : root(at) {}

template<typename T>
AVL<T>::~AVL() {
	std::stack<typename AVL<T>::Node*> pointers;
	grab_pointers(pointers, this->root);
	while (not pointers.empty()) {
		delete pointers.top();
		pointers.pop();
	}
}

template<typename T>
unsigned AVL<T>::height() {
	return (this->root == nullptr ? 0 : this->root->height);
}

template<typename T>
unsigned AVL<T>::size() {
	return (this->root == nullptr ? 0 : this->root->size);
}

template<typename T>
bool AVL<T>::empty() {
	return (this->root == nullptr);
}

template<typename T>
typename AVL<T>::Node* AVL<T>::rotate_left(typename AVL<T>::Node *p) {
	typename AVL<T>::Node *q = p->right;
	p->right = q->left;
	q->left = p;
	p->update_parameters();
	q->update_parameters();
	return q;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::rotate_right(typename AVL<T>::Node *p) {
	typename AVL<T>::Node *q = p->left;
	p->left = q->right;
	q->right = p;
	p->update_parameters();
	q->update_parameters();
	return q;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::rotate_left_right(typename AVL<T>::Node *p) {
	p->left = rotate_left(p->left);
	return rotate_right(p);
}

template<typename T>
typename AVL<T>::Node* AVL<T>::rotate_right_left(typename AVL<T>::Node *p) {
	p->right = rotate_right(p->right);
	return rotate_left(p);
}

template<typename T>
typename AVL<T>::Node* AVL<T>::rebalance(typename AVL<T>::Node *p) {
	if (p == nullptr) return p;
	if (get_height<T>(p->left) - get_height<T>(p->right) > 1) {
		if (get_height<T>(p->left->left) >= get_height<T>(p->left->right))
			p = rotate_right(p);
		else
			p = rotate_left_right(p);
	} else if (get_height<T>(p->right) - get_height<T>(p->left) > 1) {
		if (get_height<T>(p->right->right) >= get_height<T>(p->right->left))
			p = rotate_left(p);
		else
			p = rotate_right_left(p);
	}
	p->update_parameters();
	return p;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::insert(typename AVL<T>::Node *p, const T& value) {
	if (p == nullptr)
		p = new typename AVL<T>::Node(value);
	else if (value < p->value)
		p->left = insert(p->left, value);
	else if (value > p->value)
		p->right = insert(p->right, value);
	else
		throw std::invalid_argument("Repeated value!");
	return rebalance(p);
}

template<typename T>
bool AVL<T>::insert(const T& value) {
	try {
		root = insert(root, value);
	} catch (const std::invalid_argument& e) {
		return false;
	}
	return true;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::erase(typename AVL<T>::Node *p, const T& value, typename AVL<T>::Node* parent) {
	if (p == nullptr)
		return p;
	else if (value < p->value)
		p->left = erase(p->left, value, p);
	else if (value > p->value)
		p->right = erase(p->right, value, p);
	else {
		if (p->left == nullptr) {
			if (p->right == nullptr) {
				if (parent && parent->left == p) parent->left = nullptr;
				if (parent && parent->right == p) parent->right = nullptr;
				delete p; p = nullptr;
			} else {
				typename AVL<T>::Node *tmp = p->right;
				*p = *tmp;
				delete tmp;
			}
		} else if (p->right == nullptr) {
			typename AVL<T>::Node *tmp = p->left;
			*p = *tmp;
			delete tmp;
		} else {
			typename AVL<T>::Node *q = p->right;
			while (q->left != nullptr)
				q = q->left;
			p->value = q->value;
			p->right = erase(p->right, p->value, p);
		}
	}
	if (p == nullptr)
		return p;
	p->update_parameters();
	return rebalance(p);
}

template<typename T>
bool AVL<T>::erase(const T& value) {
	if (!contains(value)) return false;
	root = erase(root, value, nullptr);
	return true;
}

template<typename T>
bool AVL<T>::contains(const T& value) {
	typename AVL<T>::Node *p = root;
	while (p != nullptr) {
		if (value == p->value) return true;
		if (value < p->value)
			p = p->left;
		else
			p = p->right;
	}
	return false;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::join_left(typename AVL<T>::Node* tl, const T& k, typename AVL<T>::Node* tr) {
	auto [value, size, height, left, right] = *tr; delete tr;

	if (left->height <= tl->height + 1) {
		typename AVL<T>::Node *aux = new typename AVL<T>::Node(k);
		aux->set_left(tl); aux->set_right(left); aux->update_parameters();
		if (aux->height <= right->height + 1) {
			typename AVL<T>::Node *ret = new typename AVL<T>::Node(value);
			ret->set_left(aux); ret->set_right(right); ret->update_parameters();
			return ret;
		} else {
			typename AVL<T>::Node *ret = new typename AVL<T>::Node(value);
			ret->set_left(rotate_left(aux)); ret->set_right(right); ret->update_parameters();
			return rotate_right(ret);
		}
	}

	else {
		typename AVL<T>::Node *aux = join_left(tl, k, left);
		typename AVL<T>::Node *auxaux = new typename AVL<T>::Node(value);
		auxaux->set_left(aux); auxaux->set_right(right); auxaux->update_parameters();
		if (aux->height <= right->height + 1)
			return auxaux;
		else
			return rotate_right(auxaux);
	}
}

template<typename T>
typename AVL<T>::Node* AVL<T>::join_right(typename AVL<T>::Node* tl, const T& k, typename AVL<T>::Node* tr) {
	auto [value, size, height, left, right] = *tl; delete tl;

	if (right->height <= tr->height + 1) {
		typename AVL<T>::Node *aux = new typename AVL<T>::Node(k);
		aux->set_left(right); aux->set_right(tr); aux->update_parameters();
		if (aux->height <= left->height + 1) {
			typename AVL<T>::Node *ret = new typename AVL<T>::Node(value);
			ret->set_left(left); ret->set_right(aux); ret->update_parameters();
			return ret;
		} else {
			typename AVL<T>::Node *ret = new typename AVL<T>::Node(value);
			ret->set_left(left); ret->set_right(rotate_right(aux)); ret->update_parameters();
			return rotate_left(ret);
		}
	}

	else {
		typename AVL<T>::Node *aux = join_right(right, k, tr);
		typename AVL<T>::Node *auxaux = new typename AVL<T>::Node(value);
		auxaux->set_left(left); auxaux->set_right(aux); auxaux->update_parameters();
		if (aux->height <= left->height + 1)
			return auxaux;
		else
			return rotate_left(auxaux);
	}
}

template<typename T>
typename AVL<T>::Node* AVL<T>::join(typename AVL<T>::Node* tl, const T& k, typename AVL<T>::Node* tr) {
	if (tl->height > tr->height + 1)
		return join_right(tl, k, tr);
	if (tr->height > tl->height + 1)
		return join_left(tl, k, tr);
	typename AVL<T>::Node *ret = new typename AVL<T>::Node(k);
	ret->set_left(tl); ret->set_right(tr); ret->update_parameters();
	return ret;
}

template<typename T>
bool AVL<T>::join_aux(typename AVL<T>::Node *other) {
	typename AVL<T>::Node *left_max = root->right;
	while (left_max->right != nullptr)
		left_max = left_max->right;

	typename AVL<T>::Node *right_min = other->left;
	while (right_min->left != nullptr)
		right_min = right_min->left;

	if (left_max->value >= right_min->value)
		return false;
	// erase(left_max->value);
	const T value = left_max->value;
	delete left_max;
	root = join(root, value, other);
	return true;
}

template<typename T>
bool AVL<T>::join(AVL<T>& other) {
	return join_aux(other.root);
}

template<typename T>
typename AVL<T>::Node* AVL<T>::split(typename AVL<T>::Node* p, const T& k) {
	auto [value, size, height, left, right] = *p; delete p;
	if (value == k) {
		root = left;
		insert(k);
		return right;
	} else if (k < value) {
		auto [_value, _size, _height, _left, _right] = *split(left, k);
		root = _left;
		insert(k);
		return join(_right, value, right);
	} else {
		auto [_value, _size, _height, _left, _right] = *split(right, k);
		root = join(left, value, _left);
		insert(k);
		return _right;
	}
}

template<typename T>
std::pair<bool,typename AVL<T>::Node*> AVL<T>::split(const T& value) {
	if (!contains(value)) return {false, nullptr};
	return {true, split(root, value)};
}

template<typename T>
void AVL<T>:: print(const std::string& prefix, typename AVL<T>::Node* p, bool isLeft) {
	if(p != nullptr) {
        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──" );
        std::cout << p->value << std::endl;
        print(prefix + (isLeft ? "│   " : "    "), p->left, true);
        print(prefix + (isLeft ? "│   " : "    "), p->right, false);
    }
}

template<typename T>
void AVL<T>::print() {
	print("", root, false);
}

#endif
