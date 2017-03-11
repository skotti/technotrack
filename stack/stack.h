#include <iostream>
#include <stdlib.h>
#include "assert.h"
#include "../array/array.h"
#include <initializer_list>

#ifdef NDEBUG
#define DEBUG_STACK_INFO(message, func)
#else 
#define DEBUG_STACK_INFO(message, func) std::cout<<message<<std::endl; func
#endif

#ifdef NDEBUG
#define ASSERT(cond, exp) if (!(cond)) {\
                          std::cout<<"ASSERTION FAILED"<<std::endl; \
                          std::cout<<"FILE : "<<__FILE__<<std::endl;\
                          std::cout<<"LINE : "<<__LINE__<<std::endl;\
                          std::cout<<"MESSAGE : "<<(exp)<<std::endl;\
                          abort();}
 #else
 #define ASSERT(cond, exp)
 #endif

template <typename T>
class Stack {
		typedef T 								value_type;
		typedef size_t 							size_type;
		typedef std::ptrdiff_t 					difference_type;
		typedef value_type& 					reference;
		typedef const value_type&				const_reference;
		typedef value_type* 					pointer;
		typedef const value_type*				const_pointer;

		Array<value_type>* m_data;
		int m_size;
		int m_capacity = 10;
public:

		Stack() : m_size(0){
				m_data = new Array<value_type>();
				ASSERT(m_data.ok(), "Inner array is broken after construction")
				ASSERT(ok(), "Stack is broken after default construction")
		}
		//правильно что выделила внутри , если вне то может быть new выброшены искючение указатель не уничтожится, или scope ptr
		//класс канарейка
		Stack(int capacity) :
		m_size(0) {
				ASSERT((m_capacity > 0), "Invalid capacity")
				m_capacity = capacity;
		        m_data = new Array<value_type>(capacity);
		        ASSERT(m_data.ok(), "Inner array is broken after construction")
				ASSERT(ok(), "Stack is broken after capacity construction")
				DEBUG_STACK_INFO("STACK capacity constructor", dump();)
		}
		//вместо итераторов использовать auto
		//move - используются когда временные переменные, = a+b/2 если a b классы operator= тоже может быть через move. Сделать пример с vector, там сделать распечатку из всех деструкторов и конструкторов с отступами, посмотреть где что
		Stack(const int *  data, int size) {
				ASSERT((size > 0 && data != NULL), "Invalid input data")
				m_data = new Array<value_type>(data, size);
				m_size = size;	
				m_capacity = m_size;
				ASSERT(m_data.ok(), "Inner array is broken after construction")
				ASSERT(ok(), "Stack is broken after pointer construction")
				DEBUG_STACK_INFO("STACK pointer constructor", dump();)
		}	

		Stack(std::initializer_list<value_type> data) {
				ASSERT(data.ok(), "Input data is not ok")
		        m_data = new Array<value_type>(data);//можно не указатель, а просто в try catch  b в последнем освобождать память
				m_size = data.size();
				m_capacity = m_size;
				ASSERT(m_data.ok(), "Inner array is broken after construction")
				ASSERT(ok(), "Stack is broken after list construction")
				DEBUG_STACK_INFO("STACK pointer constructor", dump();)
		}

		//тут может на равно упасть, нужно указатель в scope ptr, в обертку, после просто вытащить указатель из scope ptr и присвоить данным

		Stack(Stack<value_type>&& data):
		m_size(std::move(data.m_size)), 
		m_capacity(std::move(data.m_capacity)) {
				m_data = std::move(data.m_data);
				data.m_data = nullptr;
		}
		 
		Stack(const Stack<value_type>& data){
				ASSERT(data.ok(), "Invalid input data")
				//std::unique_ptr<T> temp(data.m_data);//std::make_unique<T[]>(data.m_size);
				m_size = data.m_size;
				m_capacity = data.m_capacity;
				m_data = new Array<value_type>(*data.m_data);
				ASSERT(m_data.ok(), "Inner array is broken after construction")
				ASSERT(ok(), "Stack is broken after list construction")
				DEBUG_STACK_INFO("STACK copy constructor", dump();)
		}

		Stack<value_type>& operator=(const Stack<value_type>& data) {
				if (this == &data) { return *this;}
				m_size = data.m_size;
				m_capacity = data.m_capacity;
				Stack<value_type> t(data);//copy constructor
				t.swap(*this);//swap
				return *this;
		}  

		void swap(Stack<value_type>& stack) {
			DEBUG_STACK_INFO("STACK function swap()", dump();)
			std::swap(this->m_data, stack.m_data);
			DEBUG_STACK_INFO("STACK function swap (after swapping)", dump();)
		}

		int getSize() const{
			DEBUG_STACK_INFO("STACK function getSize()", dump();)
			return m_size;
		}

		void dump() const {
				if (ok()) {std::cout<<"STACK IS IN OK STATE. PRINTING INFO ..."<<std::endl;}
				else { std::cout<<" STACK IS BROKEN. PRINTING INFO..."<<std::endl;};
				std::cout<<"STACK SIZE : "<<m_size<<std::endl;
				std::cout<<"STACK CAPACITY : "<<m_capacity<<std::endl;
				std::cout<<"DATA TYPE : "<<typeid(T).name()<<std::endl;
				std::cout<<"STACK ELEMENTS : "<<std::endl;
				std::cout<<"-----------------------------------------"<<std::endl;
				for (int i = 0; i < m_size; i++)
					std::cout <<"Element ["<<i<<"] : "<<(*m_data)[i]<<std::endl;
				std::cout<<std::endl;
				std::cout<<"-----------------------------------------"<<std::endl;
		}

		value_type pop() {
			DEBUG_STACK_INFO("STACK function pop()", dump();)
			ASSERT(ok(), "Invalid STACK before popping")
			value_type tmp = (*m_data).pop_back();
			m_size -= 1;
			ASSERT(ok(), "Invalid STACK after popping")
			DEBUG_STACK_INFO("STACK function pop() (after popping", dump();)
			return tmp;
		}

		value_type top() {
				ASSERT(ok(), "Invalid STACK in top function")
				return (*m_data).at(m_size - 1);
		}

		void push(value_type item) {
			ASSERT(ok(), "Invalid STACK before pushing")
			DEBUG_STACK_INFO("STACK function push()", dump();)
			if (m_size == m_capacity) {
					(*m_data).resize(2 * m_size);
					m_capacity = 2 * m_size;			
			}
			(*m_data).push_back(item);
			ASSERT(ok(), "Invalid STACK after pushing")
			m_size+=1;
			DEBUG_STACK_INFO("STACK function push() (after pushing)", dump();)
		}

		void resize(int size) {
				ASSERT(ok(), "Invalid STACK before resize()")
				DEBUG_STACK_INFO("STACK function resize()", dump();)
				(*m_data).resize();
				m_size = size;
				m_capacity = std::max(m_capacity, m_size);
				ASSERT(ok(), "Invalid STACK after resize()")
				DEBUG_STACK_INFO("STACK function resize() (after resize)", dump();)
		}

		~Stack() {
				std::cout<<"Destruction"<<std::endl;
				delete m_data;
				m_data = NULL;
				m_size = 0;
				m_capacity = 0;
		}

		bool ok() const {
				if (m_data == NULL) std::cout<<"NUUULLL"<<std::endl;
				if (m_size < 0) std::cout<<"LESS than zero"<<std::endl;
				if (m_capacity < 0) std::cout<<"LESS than zero cap"<<std::endl;
				if (m_size > m_capacity) std::cout<<"SIZE > CAP"<<std::endl;
				return (m_size >= 0) && (m_capacity >= 0) && (m_size <= m_capacity) && (m_data != NULL);
		}
};
