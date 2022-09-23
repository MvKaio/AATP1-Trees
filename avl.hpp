#ifndef AVL_HPP
#define AVL_HPP

#include <utility>
#include <iostream>
#include <stdexcept>

template<typename T>
struct Node {
	T value;
	unsigned size;
	Node *left, *right;
	Node(T _value = T()) : value(_value), size(1), 
						   left(nullptr), right(nullptr) {}
	void update_size();
	void set_left(Node* x);
	void set_right(Node* x);
};

template<typename T>
class AVL {
  public:
	AVL();
	unsigned size();
	bool empty();
	bool insert(const T& value);
	void erase(const T& value);
	bool contains(const T& value);
	bool join(Node<T>* other);
	std::pair<bool,Node<T>*> split(const T& value);
	void print();

  private:
	Node<T>* rebalance(Node<T>* p);
	Node<T>* rotate_right(Node<T>* p);
	Node<T>* rotate_left(Node<T>* p);
	Node<T>* rotate_right_left(Node<T>* p);
	Node<T>* rotate_left_right(Node<T>* p);
	Node<T>* insert(Node<T>* p, const T& value);
	Node<T>* erase(Node<T>* p, const T& value);
	Node<T>* join(Node<T>* l, const T& k, Node<T>* r);
	Node<T>* join_right(Node<T>* l, const T& k, Node<T>* r);
	Node<T>* join_left(Node<T>* l, const T& k, Node<T>* r);
	Node<T>* split(Node<T>* p, const T& value);
	void print(const std::string& prefix, Node<T>* p, bool isLeft);
	Node<T> *root;
};

// NODE
// ----

template<typename T>
int get_size(Node<T> *node) {
	return (node == nullptr ? 0 : (int) node->size);
}

template<typename T>
void Node<T>::update_size() {
	this->size = 1 + std::max(get_size(left), get_size(right));
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

// AVL
// ---

template<typename T>
AVL<T>::AVL() : root(nullptr) {}

template<typename T>
unsigned AVL<T>::size() {
	return (this->root == nullptr ? 0 : this->root->size);
}

template<typename T>
bool AVL<T>::empty() {
	return (this->root == nullptr);
}

template<typename T>
Node<T>* AVL<T>::rotate_left(Node<T> *p) {
	Node<T> *q = p->right;
	p->right = q->left;
	q->left = p;
	p->update_size();
	q->update_size();
	return q;
}

template<typename T>
Node<T>* AVL<T>::rotate_right(Node<T> *p) {
	Node<T> *q = p->left;
	p->left = q->right;
	q->right = p;
	p->update_size();
	q->update_size();
	return q;
}

template<typename T>
Node<T>* AVL<T>::rotate_left_right(Node<T> *p) {
	p->left = rotate_left(p->left);
	return rotate_right(p);
}

template<typename T>
Node<T>* AVL<T>::rotate_right_left(Node<T> *p) {
	p->right = rotate_right(p->right);
	return rotate_left(p);
}

template<typename T>
Node<T>* AVL<T>::rebalance(Node<T> *p) {
	if (get_size(p->right) - get_size(p->left) < -1) {
		if (get_size(p->left->left) >= get_size(p->left->right))
			p = rotate_right(p);
		else
			p = rotate_left_right(p);
	} else if (get_size(p->right) - get_size(p->left) > 1) {
		if (get_size(p->right->right) >= get_size(p->right->left))
			p = rotate_left(p);
		else
			p = rotate_right_left(p);
	}
	p->update_size();
	return p;
}

template<typename T>
Node<T>* AVL<T>::insert(Node<T> *p, const T& value) {
	if (p == nullptr)
		p = new Node<T>(value);
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
Node<T>* AVL<T>::erase(Node<T>* p, const T& value) {
	if (p == nullptr)
		return p;
	else if (value < p->value)
		p->left = erase(p->left, value);
	else if (value > p->value)
		p->right = erase(p->right, value);
	else {
		if (p->left == nullptr) {
			if (p->right == nullptr) {
				delete p; p = nullptr;
			} else {
				Node<T> *tmp = p->right;
				*p = *tmp;
				delete tmp;
			}
		} else if (p->right == nullptr) {
			Node<T> *tmp = p->left;
			*p = *tmp;
			delete tmp;
		} else {
			Node<T> *q = p->right;
			while (q->left != nullptr)
				q = q->left;
			p->value = q->value;
			erase(p->right, p->value);
		}
	}
	if (p == nullptr)
		return p;
	p->update_size();
	return rebalance(p);
}

template<typename T>
void AVL<T>::erase(const T& value) {
	root = erase(root, value);
}

template<typename T>
bool AVL<T>::contains(const T& value) {
	Node<T> *p = root;
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
Node<T>* AVL<T>::join_left(Node<T>* tl, const T& k, Node<T>* tr) {
	auto [value, size, left, right] = tr; delete tr;

	if (left->size <= tl->size + 1) {
		Node<T> *aux = new Node<T>(k);
		aux->set_left(tl); aux->set_right(left); aux->update_size();
		if (aux->size() <= right->size() + 1) {
			Node<T> *ret = new Node<T>(value);
			ret->set_left(aux); ret->set_right(right); ret->update_size();
			return ret;
		} else {
			Node<T> *ret = new Node<T>(value);
			ret->set_left(rotate_left(aux)); ret->set_right(right); ret->update_size();
			return rotate_right(ret);
		}
	}

	else {
		Node<T> *aux = join_left(tl, k, left);
		Node<T> *auxaux = new Node<T>(value);
		auxaux->set_left(aux); auxaux->set_right(right); auxaux->update_size();
		if (aux->size <= right->size + 1)
			return auxaux;
		else
			return rotate_right(auxaux);
	}
}

template<typename T>
Node<T>* AVL<T>::join_right(Node<T>* tl, const T& k, Node<T>* tr) {
	auto [value, size, left, right] = tl; delete tl;

	if (right->size <= tr->size + 1) {
		Node<T> *aux = new Node<T>(k);
		aux->set_left(right); aux->set_right(tr); aux->update_size();
		if (aux->size() <= left->size() + 1) {
			Node<T> *ret = new Node<T>(value);
			ret->set_left(left); ret->set_right(aux); ret->update_size();
			return ret;
		} else {
			Node<T> *ret = new Node<T>(value);
			ret->set_left(left); ret->set_right(rotate_right(aux)); ret->update_size();
			return rotate_left(ret);
		}
	}

	else {
		Node<T> *aux = join_right(right, k, tr);
		Node<T> *auxaux = new Node<T>(value);
		auxaux->set_left(left); auxaux->set_right(aux); auxaux->update_size();
		if (aux->size <= left->size + 1)
			return auxaux;
		else
			return rotate_left(auxaux);
	}
}

template<typename T>
Node<T>* AVL<T>::join(Node<T>* tl, const T& k, Node<T>* tr) {
	if (tl->size > tr->size + 1)
		return join_right(tl, k, tr);
	if (tr->size > tl->size + 1)
		return join_left(tl, k, tr);
	Node<T> *ret = new Node<T>(k);
	ret->set_left(tl); ret->set_right(tr); ret->update_size();
	return ret;
}

template<typename T>
bool AVL<T>::join(Node<T> *other) {
	Node<T> *left_max = root->right;
	while (left_max->right != nullptr)
		left_max = left_max->right;

	Node<T> *right_min = other->left;
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
Node<T>* AVL<T>::split(Node<T>* p, const T& k) {
	auto [value, size, left, right] = p; delete p;
	if (value == k) {
		root = left;
		insert(k);
		return right;
	} else if (k < value) {
		auto [_value, _size, _left, _right] = split(left, k);
		root = _left;
		insert(k);
		return join(_right, value, right);
	} else {
		auto [_value, _size, _left, _right] = split(right, k);
		root = join(left, value, _left);
		insert(k);
		return _right;
	}
}

template<typename T>
std::pair<bool,Node<T>*> AVL<T>::split(const T& value) {
	if (!contains(value)) return false;
	return {true, split(root, value)};
}

template<typename T>
void AVL<T>:: print(const std::string& prefix, Node<T>* p, bool isLeft) {
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
