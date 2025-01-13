#pragma once
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "logger.h"
#include "player.h"

enum auth_e {
    SUCCESS,
    FAIL,
};

class Client {
public:
    Client(const std::string& config_file);

    ~Client();

    void parse_config(const std::string& config_file);
    void play();

private:
    int client_fd;
    unsigned int port;
    Logger logger;
    const char* server_ip;
    struct sockaddr_in server_addr;

    Player player;

    struct sockaddr_in new_sockaddr(int port, const char* server_ip) {
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) < 0) {
            logger.log(ERROR, "Invalid server addr.");
            exit(EXIT_FAILURE);
        }

        return server_addr;
    }
    auth_e authenticate();
};
