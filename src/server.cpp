#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>

#include <toml.hpp>

#include "logger.h"
#include "game.h"
#include "server.h"
#include "client.h"

void clear_socket_buffer(int socket_fd) {
    char buffer[1024];
    while (true) {
        ssize_t bytes_read = recv(socket_fd, buffer, sizeof(buffer), MSG_DONTWAIT);
        if (bytes_read <= 0) {
            break;
        }
    }
}

Server::Server(const std::string& config_file) {
    int new_socket;
    int addr_len = sizeof(server_addr);

    parse_config(config_file);

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr = new_sockaddr(port);

    if (server_sock < 0) {
        logger.log(ERROR, "Failed to create server socket.");
        return;
    } 
    logger.log(INFO, "Server socket was successfully created.");


    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        logger.log(ERROR, "Failed to bind to port.");
        return;
    }
    logger.log(INFO, "Port was binded successfully.");

    if (listen(server_sock, max_clients) != 0) {
        logger.log(ERROR, "Listen failed.\n");
        return;
    }
    logger.log(INFO, "Waiting for a client to connect...");

    for (int i = 0; i < max_clients; ++i) {
        new_socket = accept(server_sock, (struct sockaddr *)&server_addr, (socklen_t*)&addr_len);

        if (new_socket < 0) {
            logger.log(ERROR, "Accept failed.");
            close(server_sock);
            return;
        } 

        logger.log(INFO, "Client connected.");

        if (auth_user(new_socket) != SUCCESS) {
            logger.log(INFO, "Authentication failed.");
        }

        client_sockets[i] = new_socket;
    }
    
    handle_clients();
}

void Server::handle_clients() {
    int player_move_size = 2;
    char player_move[2];
    std::string field;
    std::string play_x = "You are playing with 'X'";
    std::string play_o = "You are playing with 'O'";
    std::pair<char, char> move;
    char move_sign[2] = {'X', 'O'};
    int valread;
    Game game;
    GameState win_state;


    send(client_sockets[0], play_x.c_str(), play_x.length(), 0);
    send(client_sockets[1], play_o.c_str(), play_o.length(), 0);

    for (;;) {
        for (int current_client = 0; current_client < max_clients; ++current_client) {
            field = game.get_field_as_string();
            send(client_sockets[current_client], field.c_str(), field.length(), 0);
            valread = read(client_sockets[current_client], player_move, player_move_size);

            if (valread <= 0) {
                logger.log(INFO, "Client disconnected.");
                break;
            }

            move.first = player_move[0] - '0';
            move.second = player_move[1] - '0';

            game.make_move(move, move_sign[current_client]);
            game.print_field();
            
            win_state = game.check_win(move);

            switch (win_state) {
                case X_WIN : {
                                 logger.log(INFO, "X win");
                                 field = "XXXXXXXXX";
                                 for (int i = 0; i < 2; ++i) {
                                     send(client_sockets[i], field.c_str(), field.length(), 0);
                                 }
                                 return;
                             }
                case O_WIN : {
                                 logger.log(INFO, "O win");
                                 field = "OOOOOOOOO";
                                 for (int i = 0; i < 2; ++i) {
                                     send(client_sockets[i], field.c_str(), field.length(), 0);
                                 }
                                 return;
                             }
                case DRAW : {
                                 logger.log(INFO, "Draw");
                                 field = "DDDDDDDDD";
                                 for (int i = 0; i < 2; ++i) {
                                     send(client_sockets[i], field.c_str(), field.length(), 0);
                                 }
                                 return;
                            }
                case UNKNOWN : break;
            }
        }
    }
}

auth_e Server::auth_user(int new_socket) {
    int buffer_size = 2048;
    char buffer[2048];

    std::string input_login;
    std::string input_password;
    const char* space_pos;

    clear_socket_buffer(new_socket);
    read(new_socket, buffer, buffer_size);

    space_pos = strchr(buffer, ' ');
    input_login.assign(buffer, space_pos - buffer);
    input_password.assign(space_pos + 1);

    logger.log(INFO, "User " + input_login + " trying to connect.");
    auto lt = users.find(input_login);
    if (lt == users.end()) {
        send(new_socket, "-1", 3, 0); //send -1 as error
        logger.log(INFO, "Incorrect login.");
        return FAIL;
    } else if (lt->second == input_password) {
        send(new_socket, "0", 2, 0);
        logger.log(INFO, "User " + input_login + " authenticated.");
        return SUCCESS;
    } else {
        send(new_socket, "-1", 3, 0); //send -1 as error
        logger.log(INFO, "Incorrect password.");
        return FAIL;
    }
}

Server::~Server() {
    logger.log(INFO, "Shutting server down.");
    close(server_sock);
}


void Server::parse_config(const std::string& config_file) {
    auto data = toml::parse(config_file, toml::spec::v(1, 1, 0));

    users = toml::find<std::map<std::string, std::string>>(data, "allowed_users");
    port = toml::find<unsigned int>(data, "port");
    std::string log_file = toml::find<std::string>(data, "log_file");
    logger.setLogFile(log_file);
}
