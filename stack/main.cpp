#include <iostream>
#include "stack.h"

using namespace std;

int test1() {
		Stack<int> st(3);
		Stack<int> st2(st);
		st2.push(5);
		st2.push(4);
		st2.push(3);
		st2.push(2);
		st2.push(1);
		return st2.getSize();
}

int test2() {
		Stack<int> st(5);
		for ( int i = 0; i < 5; i++)
				st.push(i);
		Stack<int> st2(6);
		st2 = st;
		return st2.getSize();
}

int test3() {
		Stack<int> st(10);
		for (int i = 0; i < 100; i++)
				st.push(1);
		return (st.getSize() == 100) && st.ok();
}

int main() {
        assert(test1() == 5);
		cout<<"Test1 passed"<<endl;
		assert(test2() == 5);
		cout<<"Test2 passed"<<endl;
		assert(test3());
		cout<<"Test3 passed"<<endl;

		Stack<int> st = {1,2,3,4,5};
		st.dump();
}
