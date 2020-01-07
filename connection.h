#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H
#include <mutex>
#include <queue>

template <typename T>
class Singleton {
public:
	static T* get_instance() {
		static T obj;
		return &obj;
	}
};

template <typename T>
class ConnectionPool {
public:
	ConnectionPool() = default;
	
	~ConnectionPool() {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		while (!m_conn_pool.empty()) {
			auto conn = m_conn_pool.front();
			m_conn_pool.pop();
			delete conn;
		}
	}
	
	void init(int num) {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		auto conn = new T;
		while (num--) {
			m_conn_pool.push(conn);
		}
	}
	
	T* get() {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		if (!m_conn_pool.empty()) {
			auto conn = m_conn_pool.front();
			m_conn_pool.pop();
			return conn;	
		} else {
			return nullptr;
		}
	}
	
	void release(T *conn) {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		if (conn != nullptr) {
			m_conn_pool.push(conn);
		}
	}
private:
	std::queue<T*> m_conn_pool;
	std::mutex m_conn_mtx;
};

template <typename T>
class Connection {
public:
	Connection() {
		while (m_conn == nullptr) {
			m_conn = Singleton<ConnectionPool<T>>::get_instance()->get();
		}
	}
	
	~Connection() {
		release();
	}
	
	T* get() {
		return m_conn;
	}
	
	void release() {
		Singleton<ConnectionPool<T>>::get_instance()->release(m_conn);
	}
	
private:
	T *m_conn = nullptr;
};

#include <iostream>

class DBA {
public:
	void f() {
		std::cout << "a" << std::endl;
	}
};

int main() {
	Singleton<ConnectionPool<DBA>>::get_instance()->init(2);
	std::cout << "start" << std::endl;
	Connection<DBA> a;
	auto x = a.get();
	x->f();
}

#endif
