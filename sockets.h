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

void get_line(char *line, int line_size);                                // gets a line from stdin until '\n' is found
void line_tok(char *line, int line_size, char *tokens[], int *n_tokens); // tokenizes a line (splits line into array of tokens, seperated by ' ')

int send_msg(int socket_fd, char *msg, int line_size);    // sends a message through a socket
int recieve_msg(int socket_fd, char *msg, int line_size); // recieves a message through a socket

/*

Thoughts on a http-inspired server-client system:
    client makes requests from server, server sends responses
    server has file system containing anything needed

    to log in, client sends:
        SET\n-filename=".login"\nname="aslade",passwd="BigServer!"

Messages:
    (sections with "\n" delim)
    START LINE:
        start with request keyword or response number
    REQUEST/RESPONSE:
        then you have the arguements and other phrases
    BODY:
        have any other info associated with message


Requests (client->server):
    GET: gets file / info from server
    SET: gets file / info from client

Responses (server->client):
    1xx: Informational Responses
        Indicate received request, continuing the process.
    2xx: Successful Responses
        Indicate request received successfully, understood, and accepted.
    3xx: Redirection Responses
        Indicate that further action needs to be taken to complete the request.
    4xx: Client Error Responses
        Indicate that the client has sent an invalid request.
    5xx: Server Error Responses
        Indicate that the server failed to fulfill a valid request.
*/
