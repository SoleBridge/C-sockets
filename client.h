#pragma once
#include "sockets.h"
#include <ncurses.h>

int client_main(int argc, char *argv[], char *env[]);                // the 'main' function for the client
int connect_to_server(const char *SERVER_IP, const int SERVER_PORT); // function for client to connect to a server; returns socket fd, FAILS
