#ifndef HCPP_SERVER_HPP
#define HCPP_SERVER_HPP

#include <netinet/in.h>

class Server {
    public:
        Server();
        ~Server();
        
        void listen(in_port_t port);

    private:
        static Server* instance;
        int m_fd{-1};
        struct sockaddr_in m_address;
        int m_working{false};

        void setup_interrupt_handler();
        void interrupt_handler(int sig);
        static void static_interrupt_handler(int sig);
};

#endif
