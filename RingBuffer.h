#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#include <atomic>
#include <cstdlib>

template <typename T>
class RingBuffer {
public:
	RingBuffer(int size) noexcept : m_size(size+1), m_head(0), m_tail(0) {
		m_buffer = static_cast<T*>(::calloc(m_size+1, sizeof(T)));
	}
	
	RingBuffer(T *buffer, int size) noexcept {
		m_buffer = buffer;
	}
	
	~RingBuffer() {
		if (nullptr != m_buffer) delete m_buffer;
	}
	
	void push(const T object) {
		while ((m_tail + 1) % m_size == m_head) ;
		m_buffer[m_tail] = object;
		m_tail = (m_tail + 1) % m_size;
	}
	
	T pop() {
		while (m_head == m_tail) ;
		T object = m_buffer[m_head];
		m_head = (m_head + 1) % m_size;
		return object;
	}
	
	void clear() {
		delete m_buffer;
		m_buffer = nullptr;
		m_head = m_tail = m_size = 0;
	}
	
private:
	T *m_buffer;
	int m_size;
	std::atomic<int> m_head;
	std::atomic<int> m_tail;
};
#endif
