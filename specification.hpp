#ifndef SPECIFICATION_HPP
#define SPECIFICATION_HPP

template<typename T>
class Tree {
  public:
	unsigned size() = 0;
	bool empty() = 0;
	void insert(const T& value) = 0;
	void erase(const T& value) = 0;
	bool contains(const T& value) = 0;
	void split(const T& value, Tree<T>& other) = 0;
	Tree<T>& join(Tree<T>& left_tree, Tree<T>& right_tree) = 0;
};

template<typename T>
class AVLTree : public Tree {
};

template<typename T>
class Treap : public Tree {
};

template<typename T>
class SplayTree : public Tree {
};

#endif
