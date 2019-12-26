#ifndef EPOLL_SERVER_H
#define EPOLL_SERVER_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include <functional>
#include <thread>
#include <atomic>

class EpollServer {
public:
	explicit EpollServer(uint16_t listen_port, size_t epoll_size) : 
			             m_listen_port(listen_port), m_epoll_size(epoll_size),
						 m_running(true) {
		m_events = static_cast<struct epoll_event*>(::calloc(m_epoll_size+1, sizeof(struct epoll_event)));
	}
	
	void init(const std::function<void(int)> &accept_fn,
			  const std::function<void(int)> &recv_data_fn,
			  const std::function<void(int)> &close_fn) {
		m_listen_fd = ::socket(AF_INET, SOCK_STREAM, 0);
		
		struct sockaddr_in svr_addr{};
		svr_addr.sin_family = AF_INET;
		svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		svr_addr.sin_port = htons(m_listen_port);
		::bind(m_listen_fd, reinterpret_cast<struct sockaddr*>(&svr_addr), sizeof(struct sockaddr_in));
		
		set_nonblock(m_listen_fd);
		
		m_accept_fn = accept_fn;
		m_recv_data_fn = recv_data_fn;
		m_close_fn = close_fn;
		
		m_epoll_fd = ::epoll_create1(0);
		
		struct epoll_event evt{};
		evt.data.fd = m_listen_fd;
		evt.events = EPOLLIN;
		::epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_listen_fd, &evt);
		
		::listen(m_listen_fd, SOMAXCONN);
		
		m_svr_thrd = std::thread(EpollServer::Listen, this);
	}
	
private:
	
	bool set_nonblock(int fd) {
		auto flags = ::fcntl(fd, F_GETFL, 0) | O_NONBLOCK;
		return -1 != ::fcntl(fd, F_SETFL, flags);
	}
	
	static void Listen(EpollServer *svr) {
		const int expire_time = 1000;
		while (svr->m_running) {
			int rdy_num = epoll_wait(svr->m_epoll_fd, svr->m_events, svr->m_epoll_size, expire_time);
			for (int idx = 0; idx < rdy_num; ++idx) {
				int fd = svr->m_events[idx].data.fd;
				if (svr->m_events[idx].events & EPOLLIN) {
					if (svr->m_events[idx].events & EPOLLRDHUP) {
						svr->m_close_fn(fd);
						::epoll_ctl(svr->m_epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
						close(fd);
					} else if (fd == svr->m_listen_fd) {
						int clt_fd = ::accept(svr->m_listen_fd, nullptr, nullptr);
						struct epoll_event clt_evt{};
						clt_evt.data.fd = clt_fd;
						clt_evt.events = EPOLLIN | EPOLLRDHUP;
						::epoll_ctl(svr->m_epoll_fd, EPOLL_CTL_ADD, clt_fd, &clt_evt);
						svr->m_accept_fn(fd);
					} else {
						svr->m_recv_data_fn(fd);
					}
				}
			}
		}
	}
	
	uint16_t m_listen_port = -1;
	size_t m_epoll_size = 0;
	
	int m_listen_fd = -1;
	int m_epoll_fd = -1;
	
	struct epoll_event *m_events = nullptr;
	
	std::thread m_svr_thrd;
	std::atomic<bool> m_running;
	
	std::function<void(int)> m_accept_fn;
	std::function<void(int)> m_recv_data_fn;
	std::function<void(int)> m_close_fn;
};

#endif
