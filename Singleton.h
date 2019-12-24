#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>

template <typename T>
class Singleton {
public:
	static T* getInstance() {
		static T obj;
		return &obj;
	}
private:
	Singleton() = delete; 
	~Singleton() = delete;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};

/*
template <typename T>
class Singleton {
public:
	static T* getInstance() {
		std::call_once(m_of, [=] {
			if (m_obj == nullptr) {
				m_obj = new T;
			}
		});
		return m_obj;
	}
private:
	Singleton() = delete; 
	~Singleton() = delete;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	
	static T* m_obj;
	static std::once_flag m_of;
};

template <typename T> std::once_flag Singleton<T>::m_of;
template <typename T> T* Singleton<T>::m_obj;
*/

#endif
