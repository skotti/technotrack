#include <iostream>
#include <fstream>
#include "stack.h"

using namespace std;

#define CHECK(cond, message, out) if (!cond) {\
							out<<"LINE : "<<__LINE__<<endl;\
							out<<"TEST CONDITION FAILED WITH RESULT"<<endl;\
							out<<message<<endl;\
							return -1;\
							}

#define TEST_VALIDITY(cond, expr, out) if (!cond){\
								out<<expr<<endl;}
int test1(ofstream& out) {
		Stack<int> st(3);
		st.push(5);
		st.push(4);
		st.push(3);
		st.push(2);
		st.push(1);
		CHECK((st.top() == 1), "Invalid work of stack push function", out)
		Stack<int> st2(st);
		CHECK((st2.top() == 1), "Invalid work of stack copy constructor", out)
		st2.pop();
		CHECK((st2.getSize() == 4), "Invalid work of stack pop", out)
		return 1;
}

int test2(ofstream& out) {
		Stack<int> st(5);
		for ( int i = 0; i < 5; i++)
				st.push(i);
		Stack<int> st2(5);
		st2 = st;
		CHECK((st2.getSize() == 5), "Invalid work of stack copy constructor", out)
		return 1;
}

int test3(ofstream&out) {
		Stack<int> st(10);
		for (int i = 0; i < 100; i++)
				st.push(1);
		CHECK((st.getSize() == 100) && st.ok(), "Invalid work of push", out)
		return 1;
}

int main() {
        try {
			ofstream out("file");
			int result;
			result = test1(out);
			TEST_VALIDITY((result == 1), "TEST 1 HAS FAILED", out)
			result = test2(out);
			TEST_VALIDITY((result == 1), "TEST 2 HAS FAILED", out)
			result = test3(out);
			TEST_VALIDITY((result == 1), "TEST 3 HAS FAILED", out)
		} catch(exception& e) {
				cout<<"Exception occured"<<endl;
		}
}
