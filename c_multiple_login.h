#ifndef C_MULTIPLE_LOGIN
#define C_MULTIPLE_LOGIN

#include <map>
#include <mutex>
#include <cstdint>

class c_multiple_login {
public:
	bool is_login(int fd) {
		std::lock_guard<std::mutex> lg(m_user_mtx);
		return m_fd_nodes.find(fd) != m_fd_nodes.end();
	}
	
	void set_login(uint16_t client_index, uint8_t node, int fd) {
		std::lock_guard<std::mutex> lg(m_user_mtx);
		m_users[client_index] |= 1 << node;
		m_fd_nodes[fd] = std::make_pair(client_index, node);
	}
	
	void del_login(int fd) {
		std::lock_guard<std::mutex> lg(m_user_mtx);
		if (m_fd_nodes.find(fd) != m_fd_nodes.end()) {
			uint16_t client_index = m_fd_nodes[fd].first;
			uint8_t node = m_fd_nodes[fd].second;
			m_users[client_index] &= ~(1 << node);
			m_fd_nodes.erase(fd);
		}
	}
private:
	std::mutex m_user_mtx;
	std::map<uint16_t, uint8_t> m_users;
	std::map<int, std::pair<uint16_t, uint8_t>> m_fd_nodes;
};

#endif
