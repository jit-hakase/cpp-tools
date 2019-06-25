#include <atomic>

class SpinLock {
public:
	lock() {
		while (m_flag.test_and_set(std::memory_order_acquire)) ;
	}
	unlock() {
		m_flag.clear(std::memory_order_release);
	}
private:
	std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
};

class SpinLockGuard {
public:
	SpinLockGuard(SpinLock& spin_lock) : m_spin_lock(spin_lock) {
		m_spin_lock.lock();
	}
	~SpinLockGuard() {
		m_spin_lock.unlock();
	}
private:
	SpinLock &m_spin_lock;
};
