//#include "treap.hpp"
#include "avl.hpp"
//#include "splay_tree.hpp"
#include <iostream>
#include <cassert>
#include <set>
#include <map>
using namespace std;

int main(int argc, char** argv) {
	srand(atoi(argv[1]));

	set<int> S;
	AVL<int> T;

	int n = 100;
	map<int, int> mp;
	for (int i = 0; i < n; i++) {
		int value = rand() % n;
		mp[value]++;

		int coin = rand() % 2; // testing only inserts

		if (coin == 0) {
			S.insert(value);
			if (value == 11 && mp[value] == 2) {}
			else T.insert(value);
			cout << "Inserted " << value << ": " << S.size() << " " << T.size() << endl;
		} else if (coin == 1) {
//			cout << "Want to erase " << value << endl;
			S.erase(value);
			T.erase(value);
//			cout << "Erased " << value << ": " << S.size() << " " << T.size() << endl;
		} else {
			//assert(S.count(value) == T.contains(value));
		}

		T.print();
	}
}
