#pragma once
#include "sockets.h"

int connect_to_server(const char *SERVER_IP, const int SERVER_PORT); // function for client to connect to a server; returns socket fd, FAILS
