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
}
