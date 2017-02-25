#include <iostream>
#include "stack.h"

using namespace std;

int main() {
		Stack<int> st(3);
		Stack<int> st2(st);
		//Stack<int> st3 = std::move(st);
		st.push(5);	
		st.push(4);
		st.push(3);
		st.push(2);
		Stack<int> st3;
		st3 = st;
		st.dump();
		st3.dump();
		int* a = new int[3];
		for (int i = 0; i < 3; i++)
				a[i] = i;
		Stack<int> st4(a, 3);
		st4.dump();
		cout<<a[0]<<endl;
		//st.push(2);
		//st.dump();
		//Stack<int> st3(std::move(st));
}
