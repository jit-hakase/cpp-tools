#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <string>

class NetServer {
public:
	NetServer(std::string ip_addr, int port, std::string net_type, listen_number=5) : 
		m_ip_addr(std::move(ip_addr)), m_port(port), m_net_type(std::move(net_type)) {}
	
	~NetServer() {
		Close();
	}
	
	bool Init() {
		if ("TCP" == m_net_type) {
			m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
		} else if ("UDP" == m_net_type) {
			m_listen_fd = socket(AF_INET, SOCK_DGRAM, 0);
		}
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(m_port);
		sin.sin_addr.s_addr = INADDY_ANY;
		bind(m_listen_fd, (struct sockaddr*)&sin, sizeof(sin));
		
		if ("TCP" == m_net_type) {
			listen(listen_fd, listen_number);
		}
	}
	
private:
	int m_listen_fd;
	std::string m_ip_addr;
	int m_port;
	std::string m_net_type;
};
