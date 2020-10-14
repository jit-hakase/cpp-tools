#include "zmq.h"
#include <cassert>

class c_zmq_sock {
public:
	// ZMQ_REP ZMQ_REQ
	void init(int mode) {
		m_ctx = zmq_ctx_new();
		assert(m_ctx != nullptr);
		
		m_pipe = zmq_socket(m_ctx, mode);
		assert(m_pipe != nullptr);
	}
	
	void bind(const char *uri) {
		int tcp_keep_alive = 1;
		int rc = zmq_setsockopt(m_pipe, ZMQ_TCP_KEEPALIVE, &tcp_keep_alive, sizeof(int));
		assert(rc == 0);
		
		int tcp_keep_idle = 1;
		rc = zmq_setsockopt(m_pipe, ZMQ_TCP_KEEPALIVE_IDLE, &tcp_keep_idle, sizeof(int));
		assert(rc == 0);
		
		rc = zmq_bind(m_pipe, uri);
		assert(rc == 0);
	}
	
	void connect(const char *uri) {
		int rc = zmq_setsockopt(m_pipe, ZMQ_SUBSCRIBE, "", 0);
		assert(rc == 0);
		
		rc = zmq_connect(m_pipe, uri);
		assert(rc == 0);
	}
	
	int recv(char *buf, size_t sz) {
		return zmq_recv(m_pipe, buf, sz, 0);
	}
	
	int send(char *buf, size_t sz) {
		return zmq_send(m_pipe, buf, sz, 0);
	}
	
	void destroy() {
		zmq_close(m_pipe);
		zmq_ctx_destroy(m_ctx);
	}
private:
	void *m_ctx = nullptr;
	void *m_pipe = nullptr;
};

int main() {
	// zmq_ctx_set(m_ctx, ZMQ_MAX_SOCKETS, 1);
	c_zmq_sock svr, clt;
	
	svr.init(ZMQ_PUB);
	svr.bind("tcp://*:5555");
	
	clt.init(ZMQ_SUB);
	clt.connect("tcp://127.0.0.1:5555");
	
	/* ... */
	return 0;
}
