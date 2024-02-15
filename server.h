#pragma once
#include "sockets.h"

int server_start(int *server_socket_fd, int *client_socket_fd);                                                                                   // start up the server: create socket, bind, and listen
int server_accept_client(int *server_socket_fd, int *client_socket_fd, struct sockaddr_in *client_addr);                                          // wait, and connect to client
int server_handle_client(int argc, char *argv[], char *env[], struct sockaddr_in *client_addr, int client_socket_fd, char line[MAX_BUFFER_SIZE]); // the function called when the server deals with a client
