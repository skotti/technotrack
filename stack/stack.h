#include <iostream>
#include <stdlib.h>
#include "assert.h"
using namespace std;

template <typename T>
class Stack {
		T* m_data;
		int m_size;
		int m_capacity = 10;
public:
		Stack();
		Stack(const int *  data, int size);
		Stack(int size);
		Stack(const Stack<T>& data);
		Stack(Stack<T>&& data);
		T top();
		bool empty();
		int size();
		void push(T item);
		void emplace();
		T pop();
		int getSize() const;
		const T* const getData();
		void dump();
		Stack<T>& operator=(const Stack& other);
		~Stack();
};

template <class T>
Stack<T>::Stack() : m_size(0){
		m_data = new T[m_capacity];	
}

template <typename T>
Stack<T>::Stack(int capacity) :
m_size(0) {
		assert(capacity > 0);
		m_capacity = capacity;
        m_data = new T[m_capacity];
}

template <typename T>
Stack<T>::Stack(const int *  data, int size) {
		assert(size > 0 && data != NULL);
		m_data = new int[size];
		for (int i = 0; i < size; i++) 
			m_data[i] = data[i];
		m_size = size;	
		m_capacity = 2*m_size;
}	

template <typename T>
Stack<T>::Stack(Stack<T>&& data) {
		m_size = std::move(data.m_size);
		m_capacity = std::move(data.m_capacity);
		m_data = data.m_data;
		data.m_data = 0;
}

template <typename T>
Stack<T>::Stack(const Stack<T>& data){
        m_data = new T[data.m_size];
		m_size = data.m_size;
        for (int i = 0; i < data.m_size; i++) {
                m_data[i] = data.m_data[i];
	}
	m_capacity = data.m_capacity;
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& data) {
		if (this != &data) {
			delete[] m_data;
			m_size = data.m_size;
			m_data = new int[m_size];
			for (int i = 0; i < data.m_size; i++) {
                 		m_data[i] = data.m_data[i]; 
	        	}
			m_capacity = data.m_capacity;
		}
		return *this;
}  
				

template <typename T>
int Stack<T>::getSize() const{
		return m_size;
}

template <typename T>
const T* const Stack<T>::getData(){
		return m_data;
}

template <typename T>
void Stack<T>::dump() {
		cout<<"STACK SIZE : "<<m_size<<endl;
		cout<<"STACK CAPACITY : "<<m_capacity<<endl;
		cout<<"DATA TYPE : "<<typeid(T).name()<<endl;
		cout<<"STACK ELEMENTS : ";
		for (int i = 0; i < m_size; i++)
			cout <<m_data[i]<<" ";
		cout<<endl;
}

template <typename T>
T Stack<T>::pop() {
		assert(m_size > 0);
		return m_data[m_size--];
}

template <typename T>
T Stack<T>::top() {
		return m_data[m_size-1];
}

template <typename T>
void Stack<T>::push(T item) {
		if (m_size == m_capacity) {
				int* newm_data = new int[2*m_capacity];
				m_capacity *= 2;
				for (int i = 0;i < m_size; i++) 
					newm_data[i] = m_data[i];
				delete[] m_data;
				m_data = new int[2*m_capacity];
				for (int i = 0;i < m_size; i++)
             				m_data[i] = newm_data[i];
				delete[] newm_data;
				
		}
		m_data[m_size++] = item;
}

template <typename T>
Stack<T>::~Stack() {
		delete[] m_data;
		m_data = NULL;
}
