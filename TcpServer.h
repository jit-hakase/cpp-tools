#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <apra/inet.h>
#include <sys/epoll.h>

class TcpServer {
public:
	TcpServer(int epoll_size, int listen_port) : m_epoll_size(epoll_size), m_listen_port(listen_port) {
		auto epoll_evt = static_cast<struct epoll_event*>(calloc(m_epoll_size+1), sizeof(struct epoll_event));
		m_epoll_evt.reset(epoll_evt);
		m_running = false;
	}
	~TcpServer() { stop(); }
	
	void start() {
		static const int expire_time = 1000;
		
		m_running = true;
		
		while (m_running) {
			int rdy_num = ::epoll_wait(m_epoll_fd, m_epoll_evt, m_epol_size, expire_time);
			if (rdy_num <= 0) {
				continue;
			}
			for (int idx = 0; idx < rdy_num; ++idx) {
				int fd = m_epoll_evt[idx].data.fd;
				if ((m_epoll_evt[idx].events & EPOLLERR) ||
					(m_epoll_evt[idx].events & EPOLLHUP) ||
					(m_epoll_evt[idx].events & EPOLLRDHUP) ||
					(!(m_epoll_evt[idx].events & EPOLLIN))) {
					/* Error */
					::epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
					::close(fd);
				} else if (fd == m_listen_fd) {
					int acc_fd = ::accept(m_listen_fd, nullptr, nullptr);
					if (-1 != acc_fd) {
						struct epoll_event evt{};
						evt.data.fd = acc_fd;
						evt.events = EPOLLIN | EPOLLET;
						if (-1 == ::epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, acc_fd, &evt)) {
							::close(fd);
						} else {
							/* Accept */
						}
					}
				} else {
					/* Recv */
				}
			}
		}
	}
	void stop() {
		m_running = false; 
	}
	
private:
	bool init() {
		m_listen_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (-1 == m_listen_fd) {
			return false;
		}
		
		struct sockaddr_in serv_addr{};
		serv_addr.sim_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(m_listen_port);
		int ret = ::bind(m_conn_fd, reinterpret_cast<struct sockaddr *>(&serv_addr), sizeof(struct sockaddr_in));
		if (-1 == ret) {
			return false;
		}
		
		struct epoll_event evt{};
		m_epoll_fd = ::epoll_create1(0);
		if (-1 == epoll_fd) {
			return false;
		}
		
		evt.data.fd = m_listen_fd;
		evt.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
		if (-1 == ::epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_listen_fd, &evt)) {
			return false;
		}
		
		return -1 != ::listen(m_listen_fd, SOMAXCONN);
	}
	
	int m_listen_fd = -1;
	int m_epoll_fd = -1;
	int m_listen_port;
	bool m_epoll_size;
	unique_ptr<struct epoll_event> m_epoll_evt;
	std::atomic<bool> m_running;
};

#endif TCPSERVER_H
