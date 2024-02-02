#pragma once
#include "sockets.h"

int server_handle_client(int argc, char *argv[], char *env[], int client_socket_fd, char line[MAX_BUFFER_SIZE]); // the function called when the server deals with a client
int server_start_listen(int argc, char *argv[], char *env[]);                                                    // the 'main' function for the server
