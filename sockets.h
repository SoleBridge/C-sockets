#pragma once

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <libgen.h>
#include <time.h>

#define DEFAULT_SERVER_IP "127.0.0.1" // server IP address ( loclahost )
#define DEFAULT_SERVER_PORT 8080      // port number
#define MAX_BUFFER_SIZE 1024          // size of buffers (lines and socket buffers)

void get_line(char *line, int line_size);                 // gets a line from stdin until '\n' is found
int send_msg(int socket_fd, char *msg, int line_size);    // sends a message through a socket
int recieve_msg(int socket_fd, char *msg, int line_size); // recieves a message through a socket
