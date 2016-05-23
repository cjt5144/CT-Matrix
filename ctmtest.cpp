/* CT Matrix Test
Copyright (c) 2016 Christopher Thompson
g++ -std=c++11 -o test ctmtest.cpp -Weverything
*/
#include "ctm.h"
#include <iostream>
#include <cassert>

int main() {
	using namespace ct;
	using std::cout;
	using std::endl;

	typedef View<int> viewi;
	
	int ia0[] = {1,2,3,4,5,6};
	int ia1[] = {7,8,9,0};
	
	viewi v0 = viewi::View();
	viewi v1 = viewi::View(ia0, ia0+6, 1, 6);
	viewi v2 = viewi::View(v1);
	viewi v3 = viewi::View(ia1, ia1+4, 4, 1, ROW);
	viewi v5 = viewi::View(ia1, ia1+4, 1, 4, ROW); // NULL
	
	cout << (v3 == v0) << endl; // false
	cout << (v1 == v2) << endl; // true
	cout << (v3 == v2) << endl; // false
	cout << (v5 == v0) << endl; // true
	
	cout << v1.end() - v1.begin() << endl;
	viewi::pvc ptr = v1.begin();
	cout << ptr << endl;
	
	return 0;
}
