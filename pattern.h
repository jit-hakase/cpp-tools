#ifndef PATTERN_H
#define PATTERN_H

template <typename T>
class Prototype {
public:
	void init_object(const T &prototype_object) {
		m_prototype_object = prototype_object;
	}
	T clone_object() {
		return m_prototype_object;
	}
private:
	T m_prototype_object;
};

class Factory {
public:
	template <typename T>
	static T make_object() {
		T object;
		return object;
	}
	template <typename T>
	static T* make_pointer() {
		return new T;
	}
};

template <typename T>
class Singleton {
public:
	static T* getInstance() {
		static T obj;
		return &obj;
	}
};

#endif
