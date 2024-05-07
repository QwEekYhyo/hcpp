#include "../include/Server.hpp"

#include <netinet/in.h>
#include <stdexcept>

#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

Server::Server() {
    m_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (m_fd < 0)
        throw std::runtime_error(strerror(errno));
}

Server::~Server() {
    if (m_fd >= 0)
        close(m_fd);
}

void Server::listen(in_port_t port) {
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_fd, (const struct sockaddr*) &address, sizeof(address)) < 0)
        throw std::runtime_error(strerror(errno));

    if (::listen(m_fd, 4096) < 0)
        throw std::runtime_error(strerror(errno));


    struct sockaddr_in client_address = {0};
    socklen_t client_address_length;
    for (;;) {
        int client_fd =
            accept(m_fd, (struct sockaddr*) &client_address, &client_address_length);
        if (client_fd < 0) {
            perror("accept");
            exit(1);
        }

        char buf[1 << 16] = {0};
        if (read(client_fd, &buf, sizeof(buf)) < 0) {
            perror("read");
            exit(1);
        }

        char reply[] = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n";

        if (write(client_fd, &reply, sizeof(reply)) < 0) {
            perror("write");
            exit(1);
        }

        close(client_fd);
    }
}
