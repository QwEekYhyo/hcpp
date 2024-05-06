#ifndef HPP_TCP_SOCKET
#define HPP_TCP_SOCKET

#include <netinet/in.h>

class TCPSocket {
    public:
        TCPSocket();
        ~TCPSocket();
        
        void listen(in_port_t port);

    private:
        int m_fd{-1};
};

#endif
