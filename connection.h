#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <mutex>
#include <queue>
#include <memory>

template <typename T>
class SimpleConnectionPool {
public:
	static SimpleConnectionPool* get_instance() {
		static SimpleConnectionPool s_conn_pool;
		return &s_conn_pool;
	}
	
	void init(int num) {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		while (num--) {
			m_conn_que.push(new T);
		}
	}
	
	T* get() {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		if (!m_conn_que.empty()) {
			auto conn = m_conn_que.front();
			m_conn_que.pop();
			return conn;
		} else {
			return nullptr;
		}
	}
	
	void release(T *conn) {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		if (conn) {
			m_conn_que.push(conn);
		}
	}
	
private:
	SimpleConnectionPool() = default;
	~SimpleConnectionPool() = default;
	
	std::queue<T*> m_conn_que;
	std::mutex m_conn_mtx;
};

template <typename T>
class SimpleConnection {
public:
	SimpleConnection() = default;
	~SimpleConnection() { release(); }
	
	T* get() {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		m_conn = SimpleConnectionPool<T>::get_instance()->get();
		return m_conn;
	}
	
	void release() {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		SimpleConnectionPool<T>::get_instance()->release(m_conn);
		m_conn = nullptr;
	}
	
private:
	T *m_conn = nullptr;
	std::mutex m_conn_mtx;
};

#endif
