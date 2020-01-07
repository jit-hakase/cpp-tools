#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H
#include <mutex>
#include <queue>
#include <memory>

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
	~ConnectionPool() = default;
	
	void init(int num) {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		while (num--) {
			m_conn_pool.push(std::make_shared<T>());
		}
	}
	
	std::shared_ptr<T> get() {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		if (!m_conn_pool.empty()) {
			auto conn = m_conn_pool.front();
			m_conn_pool.pop();
			return conn;
		} else {
			return nullptr;
		}
	}
	
	void release(std::shared_ptr<T> conn) {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		if (conn) {
			m_conn_pool.push(conn);
		}
	}
private:
	std::queue<std::shared_ptr<T>> m_conn_pool;
	std::mutex m_conn_mtx;
};

template <typename T>
class Connection {
public:
	Connection() = default;
	
	~Connection() {
		release();
	}
	
	std::shared_ptr<T> get(int timeout = 1) {
		while (m_conn == nullptr) {
			m_conn = Singleton<ConnectionPool<T>>::get_instance()->get();
			//sleep(timeout);
		}
		return m_conn;
	}
	
	void release() {
		std::lock_guard<std::mutex> lg(m_conn_mtx);
		Singleton<ConnectionPool<T>>::get_instance()->release(m_conn);
		m_conn = nullptr;
	}
	
private:
	std::shared_ptr<T> m_conn;
	std::mutex m_conn_mtx;
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
