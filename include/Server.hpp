#ifndef HCPP_SERVER_HPP
#define HCPP_SERVER_HPP

#include <netinet/in.h>

class Server {
    public:
        Server();
        ~Server();
        
        void listen(in_port_t port);

    private:
        int m_fd{-1};
};

#endif
