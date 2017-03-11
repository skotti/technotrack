#include <iostream>
#include <initializer_list>
#include <exception>
#include <stdexcept>
#include <fstream>

#ifdef NDEBUG
#define DEBUG_INFO(message, func)
#define DEBUG_PRINT(message)
#else 
#define DEBUG_INFO(message, func) std::cout<<message<<std::endl; func
#define DEBUG_PRINT(message) std::cout<<message<<std::endl;
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


template<typename T>
class Array{
private:
	typedef T 								value_type;
	typedef size_t 							size_type;
	typedef std::ptrdiff_t 					difference_type;
	typedef value_type& 					reference;
	typedef const value_type&				const_reference;
	typedef value_type* 					pointer;
	typedef const value_type*				const_pointer;
	int m_size;
	int m_capacity = 10; 
	value_type* m_data;
public:

	//CLASS ITERATOR IS INNER CLASS
	template <typename value_type>
	class Iterator : public std::iterator<std::random_access_iterator_tag, value_type>{
		private:
			pointer m_a;
			int m_pos = 0;
		public:
			Iterator() {}
			Iterator(pointer a, int pos) :m_a(a), m_pos(pos) {}

			Iterator& operator++(int i) {
				m_pos++;
				return *this;
			}

			Iterator& operator--(int i) {
				m_pos--;
				return *this;
			}

			Iterator& operator++() {
				++m_pos;
				return *this;
			}

			Iterator& operator--() {
				--m_pos;
				return *this;
			}
			reference operator*() {return *(m_a + m_pos);}

			int at() {return m_pos;}

			value_type& operator[](int index) {return *(m_a + index);}
	};

	template <typename value_type>
	class Const_Iterator : public std::iterator<std::random_access_iterator_tag, value_type>{
		private:
			pointer m_a;
			int m_pos = 0;
		public:
			Const_Iterator() {}
			Const_Iterator(pointer a, int pos) :m_a(a), m_pos(pos) {}

			Const_Iterator& operator++(int i) {
				m_pos++;
				return *this;
			}

			Const_Iterator& operator--(int i) {
				m_pos--;
				return *this;
			}

			Const_Iterator& operator++() {
				++m_pos;
				return *this;
			}

			Const_Iterator& operator--() {
				--m_pos;
				return *this;
			}
			const_reference operator*() {return *(m_a + m_pos);}

			int at() {return m_pos;}
	};

	/*void* operator new(size_t size, size_t n) {
		std::ofstream out("ff");
		out<<"I'm here";
		void* mem = malloc(size * n);
		memset(mem, 0, size * n);
		return mem;
	}*/


	Iterator<value_type>& begin() {
		auto it = new Iterator<value_type>(m_data, 0);
		return *it;
	}

	Iterator<value_type>& end() {
		auto it = new Iterator<value_type>(m_data, m_size);
		return *it;
	}

	Array():m_size(0),
			m_data (static_cast<A*>(operator new[] (n * sizeof(A)))) {
		m_data = new value_type[m_capacity];
		DEBUG_INFO("Default constructor", dump();)
	}

	Array(int capacity) {
		ASSERT((m_capacity > 0), "Capacity is less than zero")
		m_data = new value_type[m_capacity];
		m_size = 0;
		m_capacity = capacity;
		DEBUG_INFO("Capacity constructor", dump();)
	}

	Array(const_pointer data, int size) {
		ASSERT((size > 0), "Size is less than zero")
		ASSERT((data != NULL), "Array in parameters is not initialized")
	
		m_size = size;
		m_capacity = m_size;
		m_data = new value_type[m_size];
		std::copy(&data[0], &data[size], &m_data[0]);
	
		DEBUG_INFO("Pointer constructor", dump();)
	}

	Array(std::initializer_list<value_type> list) {
		ASSERT((list.size() >0), "List is empty")
		
		m_data = new value_type[list.size()];
		m_size = list.size();
		m_capacity = m_size;
		std::copy(list.begin(), list.end(), m_data);
		
		DEBUG_INFO("List constructor", dump();)
	}

	Array(Array&& data) :
	m_size(std::move(data.m_size)),
	m_capacity(std::move(data.m_capacity)) {
	  DEBUG_INFO("Move constructor, passing data dump", data.dump();)
	
	  m_data = data.m_data;
	  data.m_data = nullptr;
	
	  DEBUG_INFO("Move constructor (after moving)", dump();)
	}

	reference at(int index) {
		DEBUG_INFO("At operator", dump();)
		if ( (index > m_size) || (index < 0)) 
			throw std::out_of_range("Out of size range");
		return m_data[index];
	}

	const_reference at(int index) const {
		DEBUG_INFO("Const at operator", dump();)
    	if ( (index > m_size) || (index < 0)) 
        	throw std::out_of_range("Out of size range");
        return m_data[index];
	}

	reference operator[] (int index) {
		DEBUG_INFO("Operator[]", dump();)
		return m_data[index];
	}

	const_reference operator[] (int index) const{
		DEBUG_INFO("Const operator[]", dump();)
		return m_data[index];
	}

	pointer data() noexcept(noexcept(value_type())) {
		DEBUG_INFO("Function data()", dump();)
		return m_data;
	}

	const_pointer data() const noexcept(noexcept(value_type())){
		DEBUG_INFO("Const function data()", dump();)
		return m_data;
	}
	
	reference front() noexcept(noexcept(value_type())) {
		DEBUG_INFO("Function front()", dump();)
		return m_data[0];
	}
	const_reference front() const noexcept(noexcept(value_type())){
		DEBUG_INFO("Const function front()", dump();)
		return m_data[0];
	}

	reference back() noexcept(noexcept(value_type())) {
		DEBUG_INFO("Function back()", dump();)
		return m_data[m_size - 1];
	}
	const_reference back() const noexcept(noexcept(value_type())){
		DEBUG_INFO("Const function back()", dump();)
		return m_data[m_size - 1];
	}

	int size() {
		DEBUG_INFO("Function size()", dump();)
		return m_size;
	}

	int max_size() {
		DEBUG_INFO("Function max_size()", dump();)
		return m_capacity;
	}

	void reserve(int size) {
		DEBUG_INFO("Function reserve()", dump();)
		ASSERT(ok(), "Cannot reserve, array is broken")
		if (size > max_size())
			throw std::length_error("Size is greater than max size");
		if (size > m_size) {
			resize(size);
		}
		DEBUG_INFO("Function reserve() (after reserving)", dump();)
		ASSERT(ok(), "Array is broken after resizing");
	}


	void push_back(reference elem) {
		DEBUG_INFO("Function push_back()", dump();)
		ASSERT(ok(), "Cannot push, array is broken")
		if ((m_size + 1) > m_capacity)
			resize(2 * m_size);
		m_data[m_size++] =  elem;
		DEBUG_INFO("Function push_back() ( after pushing)", dump();)
		ASSERT(ok(), "Array is broken after push()")
	}

	const_reference pop_back() {
		DEBUG_INFO("Function pop_back()", dump();)
		ASSERT((m_size > 0), "size is less than zero, cannot pop")
		return m_data[--m_size];
	}

	Array(const Array<value_type>& data) {
		m_size = data.m_size;
		m_capacity = data.m_capacity;
		m_data = new value_type[m_capacity];
		std::copy(&data.m_data[0], &data.m_data[m_size], &m_data[0]);
		DEBUG_INFO("Copy constructor ", dump();)
	}

	const Array<value_type>& operator=(const Array<value_type>& data) {
		ASSERT(data.ok(), "Invalid input data")
		if (this == &data) { 
			DEBUG_INFO("Operator = (eqaulity was true)", dump();) 
			return *this;
		}
		if (m_size == 0)
			m_data = new value_type[data.m_size];
		else if (m_size < data.m_size) {
			resize(data.m_size);
		}
		auto a(data);
		a.swap(*this);
		DEBUG_INFO("Operator = (after =)", dump();)
		ASSERT(ok(), "Array is broken after operator = ")
		return *this;
	}

	Array<value_type>& operator=(Array<value_type>&& data) {
		DEBUG_INFO("Move operator =", dump();)
		ASSERT(data.ok(), "Passing array is broken, cannot move")
		m_size = data.m_size;
		m_capacity = data.m_capacity;
		m_data = data.m_data;
		data.m_data = nullptr;
		DEBUG_INFO("Move operator = (after move)", dump();)
		ASSERT(ok(), "Array is broken after move =")
	}
	void swap(Array<value_type>& data) noexcept{
		DEBUG_INFO("Function swap() ", dump();)
		std::swap(data.m_data, this->m_data);
		std::swap(data.m_size, this->m_size);
		std::swap(data.m_capacity, this->m_capacity);
		DEBUG_INFO("Function swap() (after swap)", dump();)
	}

	template <typename ...Args>
	pointer emplace(int pos, Args&&... args) {
		DEBUG_INFO("Function emplace()", dump();)
		if ((m_size + 1) > m_capacity) {
			auto tmp(*this);
			resize(2 * m_size);
			std::copy(&tmp.m_data[0], &tmp.m_data[pos], &m_data[0]);
			m_data[pos] = *(new value_type(std::forward<value_type>(args)...));
			std::copy(&tmp.m_data[pos], &tmp.m_data[m_size], &m_data[pos + 1]);
		} else {
			auto tmp(*this);
			m_data[pos] = *(new value_type(std::forward<value_type>(args)...));
			std::copy(&tmp.m_data[pos], &tmp.m_data[m_size],&m_data[pos + 1]);
		}
		m_size+=1;
		DEBUG_INFO("Function emplace() (after emplace)", dump();)
		ASSERT(ok(), "Array is broken after emplace")
		return &m_data[pos];
	}
			
	void resize(int size) {
		DEBUG_INFO("Function resize()", dump();)
		ASSERT(ok(), "Array is broken, cannot be resized")
		auto tmp(*this);
		delete[] m_data;
		m_data = new value_type[size];
		m_capacity = size;
		std::copy(&tmp.m_data[0], &tmp.m_data[m_size], &m_data[0]);
		DEBUG_INFO("Function resize() (after resize)", dump();)
		ASSERT(ok(), "Array is broken after resizing")
	}

	Array<value_type>& operator+=(const Array<value_type>& other) {
		DEBUG_INFO("Operator += ", dump();)
		ASSERT(ok(), "Current array is broken, cannot add")
		ASSERT(other.ok(), "Another array is broken, cannot add")
		
		auto size = std::min(m_size, other.m_size);
		for (int i = 0; i < size; i++) {m_data[i] += other.m_data[i];}
		
		DEBUG_INFO("Operator +=(after +=) ", dump();)
		ASSERT(ok(), "Current array is broken after add")
		ASSERT(other.ok(), "Another array is broken after add")
		
		return *this;
	}

	Array<value_type>& operator-=(Array<value_type>& other) {
		DEBUG_INFO("Operator -= ", dump();)
		ASSERT(ok(), "Current array is broken, cannot sub")
		ASSERT(other.ok(), "Another array is broken, cannot sub")
		
		auto size = std::min(m_size, other.m_size);
		for (int i = 0; i < size; i++) {m_data[i] -= other.m_data[i];}
		
		DEBUG_INFO("Operator -=(after -=)", dump();)
		ASSERT(ok(), "Current array is broken, cannot sub")
		ASSERT(other.ok(), "Another array is broken, cannot sub")
		
		return *this;
	}

	void clear() {
		DEBUG_INFO("Function clear() ", dump();)
		ASSERT(ok(), "Current array is broken, cannot clear")
		delete[] m_data;
		m_size = 0;
		m_data = new value_type[m_capacity];
		DEBUG_INFO("Function clear() (after clearing)", dump();)
		ASSERT(ok(), "Array is broken after clear")
	}

	Iterator<value_type> insert(Iterator<value_type> it, const value_type& elem) {
		DEBUG_INFO("Function insert() ", dump();)
		int pos = it.at();
		ASSERT(pos <= m_size, "Invalid position for insertion")
		if ((m_size + 1) > m_capacity) {
			auto tmp(*this);
			resize(2 * m_size);
			std::copy(&tmp.m_data[0], &tmp.m_data[pos], &m_data[0]);
			m_data[pos] = elem;
			std::copy(&tmp.m_data[pos], &tmp.m_data[m_size], &m_data[pos + 1]);
		} else {
			auto tmp(*this);
			m_data[pos] = elem;
			std::copy(&tmp.m_data[pos], &tmp.m_data[m_size],&m_data[pos + 1]);
		}
		m_size+=1;
		DEBUG_INFO("Function insert() (after insertion)", dump();)
		return begin();
	}

	Array<value_type>& operator()() {
		return *this;
	}
	void dump() const{
		if (ok()) {std::cout<<"ARRAY IS IN OK STATE. PRINTING INFO ..."<<std::endl;}
		else { std::cout<<" ARRAY IS BROKEN. PRINTING INFO..."<<std::endl;};
		std::cout<<"ARRAY SIZE : "<<m_size<<std::endl;
		std::cout<<"ARRAY CAPACITY : "<<m_capacity<<std::endl;
		std::cout<<"DATA TYPE : "<<typeid(T).name()<<std::endl;
		std::cout<<"ARRAY ELEMENTS : "<<std::endl;
		std::cout<<"-----------------------------------------"<<std::endl;
		for (int i = 0; i < m_size; i++)
			std::cout <<"Element ["<<i<<"] : "<<m_data[i]<<std::endl;
		std::cout<<std::endl;
		std::cout<<"-----------------------------------------"<<std::endl;
		std::cout<<std::endl;
	}

	bool ok() const{
		return (m_data!= NULL) && (m_size >= 0) && (m_capacity >= 0) && (m_size <= m_capacity);
	}

	~Array() {
		DEBUG_PRINT("DESTRUCTOR is called")
		m_data = 0;
		m_capacity = 0;
		delete[] m_data;
		m_data = nullptr;
	}
};

template <typename T>
Array<T> operator+(Array<T> a1, Array<T>& a2) {
	DEBUG_INFO("Operator + with move", a1.dump(); a2.dump();)
	std::cout<<"After copy construction"<<std::endl;
	a1+=a2;
	DEBUG_INFO("Operator + with move (after add)", a1.dump(); a2.dump();)
	return a1;
}

template <typename T>
Array<T> operator-(Array<T> a1, Array<T>& a2) {
	DEBUG_INFO("Operator + with move", a1.dump(); a2.dump();)
	std::cout<<"After copy construction"<<std::endl;
	a1-=a2;
	DEBUG_INFO("Operator + with move (after add)", a1.dump(); a2.dump();)
	return a1;
}




//make own exception (конструктор big exception)
//пользовательские ассерты превращаются в exception
//std clog
//soft assert

//код ошибки возвращать,  завимсимости от этого throw или что то еще
//ok с разным уровнем жесткости - типа ok с ассертом и ok с throw
//писать адрес объекта ( типа delete был вызван или нет)