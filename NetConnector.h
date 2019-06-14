#ifdef NET_CONNECTOR_H
#define NET_CONNECTOR_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include <string>

class NetConnector {
public:
	NetConnector(std::string ip_addr, int port, std::string conn_type) :
		m_ip_addr(std::move(ip_addr)), m_port(port), m_conn_type(std::move(conn_type)) {}
	~NetConnector() { close(); }
	
	bool init() {
		if ("TCP" == m_conn_type) {
			m_conn_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		} else if ("UDP" == m_conn_type) {
			m_conn_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		}
		return -1 != m_conn_fd;
	}
	
	bool connect() {
		struct sockaddr_in serv_addr{};
		serv_addr.sim_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(m_ip_addr.c_str());
		serv_addr.sin_port = htons(m_port);
		return -1 != ::connect(m_conn_fd, reinterpret_cast<struct sockaddr *>(&serv_addr), sizeof(struct sockaddr_in));
	}
	
	bool close() {
		return -1 == m_conn_fd ? true : -1 != ::close(m_conn_fd);
	}
	
	std::string m_ip_addr;
	int m_port;
	std::string m_conn_type;
	int m_conn_fd = -1;
};

#endif
