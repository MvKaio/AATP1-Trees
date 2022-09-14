//#include "treap.hpp"
//#include "avl.hpp"
#include "splay_tree.hpp"
#include <iostream>
#include <cassert>
#include <set>
using namespace std;

int main(int argc, char** argv) {
	srand(atoi(argv[1]));

	set<int> S;
	//treap<int> T;

	int n = 100000;
	for (int i = 0; i < n; i++) {
		int value = rand() % n;

		int coin = rand() % 1; // testing only inserts

		if (coin == 0) {
			S.insert(value);
			T.insert(value);
		} else if (coin == 1) {
			S.erase(value);
			T.erase(value);
		} else {
			//assert(S.count(value) == T.contains(value));
		}

		cout << "Inserted " << value << ": " << S.size() << " " << T.size() << endl;
		assert(S.size() == T.size());
	}
}
