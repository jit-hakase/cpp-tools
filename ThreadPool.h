#include <thread>
#include <vector>

class ThreadPool {
public:
	template <typename FN>
	explicit ThreadPool(FN fn) : ThreadPool(std::thread::hardware_concurrency(), fn) {}
	
	template <typename FN>
	explicit ThreadPool(int thrd_num, FN fn) {
		m_thrd_num = thrd_num;
		for (int idx = 0; idx < m_thrd_num; ++idx) {
			m_thrds.push_back(std::thread(fn));
		}
	}
	
	~ThreadPool() {
		for (auto &thrd : m_thrds) {
			if (thrd.joinable()) {
				thrd.join();
			}
		}
	}

private:
	std::vector<std::thread> m_thrds;
	int m_thrd_num;
};
