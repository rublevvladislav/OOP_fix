#pragma once

template<class T> class Vector
{
public:
	Vector<T>() : m_capacity(0), m_size(0), m_data(nullptr) {}
	Vector<T>(unsigned int size) : m_size(size), m_capacity(size), m_data(new uint8_t[size * sizeof(T)]) {}
	Vector<T>(Vector<T> const & other) 
	{
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		m_data = new uint8_t[m_capacity * sizeof(T)];
		for (unsigned int i = 0; i < m_size; i++) 
		{
			new(m_data + i * sizeof(T)) T(other[i]);
		}
	}
	~Vector<T>() 
	{
		for (size_t i = 0; i < m_size; i++)
		{
			this->operator[](i).~T();
		}
		delete[] m_data;
	}
	T& operator[] (unsigned int const & index) const 
	{
		if (index < 0 || index >= m_size)
			throw std::runtime_error("vector out of bounds"); 
		return reinterpret_cast<T&>(m_data[index * sizeof(T)]);
	}
	void reserve(unsigned int capacity) 
	{
		if (capacity <= m_size) return;
		uint8_t* tmp = new uint8_t[capacity * sizeof(T)];
		for (unsigned int i = 0; i < m_size * sizeof(T); i++) tmp[i] = m_data[i];
		m_capacity = capacity;
		delete[] m_data;
		m_data = tmp;
	}
	void push_back(T const & element) 
	{
		if (m_capacity == 0) reserve(2);
		else if (m_size >= m_capacity) reserve(m_capacity * 2);
		new(m_data + m_size * sizeof(T)) (T)(element);
		m_size++;
	}
	void pop_back() 
	{
		if (m_size > 0) 
		{
			this->operator[](m_size - 1).~T();
			m_size--;
		}
	}
	unsigned int size() const 
	{
		return m_size;
	}
	unsigned int capacity() const 
	{
		return m_capacity;
	}
	bool empty() const 
	{
		return m_size == 0;
	}
	void print(std::ostream& out) const 
	{
		for (unsigned int i = 0; i < m_size; i++) out << reinterpret_cast<T&>(m_data[i * sizeof(T)]) << std::endl;
	}
	void clear() 
	{
		while (m_size > 0) pop_back();
		delete[] m_data;
		m_size = 0;
		m_capacity = 0;
	}
private:
	size_t m_size, m_capacity;
	uint8_t* m_data;
};