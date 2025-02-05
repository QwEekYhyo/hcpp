#include "../include/Server.hpp"

#include <iostream>
#include <stdexcept>

#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

Server* Server::instance = nullptr;

Server::Server() {
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    const int enable = 1;
    if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt");
        exit(1);
    }
        

    if (m_fd < 0)
        throw std::runtime_error(strerror(errno));
    Server::instance = this;
    setup_interrupt_handler();
}

Server::~Server() {
    if (m_fd >= 0)
        close(m_fd);
}

void Server::listen(in_port_t port) {
    m_address.sin_family = AF_INET;
    m_address.sin_port = htons(port);
    m_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_fd, (const struct sockaddr*) &m_address, sizeof(m_address)) < 0)
        throw std::runtime_error(strerror(errno));

    if (::listen(m_fd, 4096) < 0)
        throw std::runtime_error(strerror(errno));


    struct sockaddr_in client_address = {0};
    socklen_t client_address_length;
    for (m_working = true; m_working;) {
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
        std::cout << buf;

        // char reply[] = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n";
        char reply[] = "HTTP/1.1 200 OK\r\nContent-Length: 15\r\nContent-Type: text/plain; charset=utf-8\r\n\r\nHello from HCPP\r\n";

        if (write(client_fd, &reply, sizeof(reply)) < 0) {
            perror("write");
            exit(1);
        }

        close(client_fd);
    }
    std::cout << "Stopped listening\r\n";
}

void Server::setup_interrupt_handler() {
    sigset_t msk_int;
    sigemptyset(&msk_int);
    sigaddset(&msk_int, SIGINT);

    struct sigaction sigact;
    sigact.sa_handler = Server::static_interrupt_handler;
    sigact.sa_mask = msk_int;

    sigaction(SIGINT, &sigact, NULL);
}

void Server::interrupt_handler(int sig) {
    if (m_fd >= 0)
        close(m_fd);
}

void Server::static_interrupt_handler(int sig) {
    Server::instance->interrupt_handler(sig);
}
