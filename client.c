#include "client.h"
#include "sockets.h"

// the 'main' function for the client
int main(int argc, char *argv[], char *env[])
{
    int server_socket_fd, ch;      // server socket file descriptor
    char line[MAX_BUFFER_SIZE];    // buffer
    char *tokens[MAX_BUFFER_SIZE]; // tokens buffer
    int n_tokens = 0;

    server_socket_fd = connect_to_server(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);

    while (1)
    {
        get_line(line, MAX_BUFFER_SIZE);
        // line_tok(line, MAX_BUFFER_SIZE, tokens, &n_tokens);
        // if (n_tokens == 0)
        // {
        //     printf("Goobye!\n");
        //     break;
        // }
        printf("Sending: \"%s\"\n", line);
        send_msg(server_socket_fd, line, MAX_BUFFER_SIZE);
        recieve_msg(server_socket_fd, line, MAX_BUFFER_SIZE);
        printf("Server response: %s\n", line);
    }

    close(server_socket_fd); // close the server socket
}

// function for client to connect to a server; returns socket fd, FAILS
int connect_to_server(const char *SERVER_IP, const int SERVER_PORT)
{
    struct sockaddr_in server_addr; // server address
    int server_socket_fd;           // server socket file descriptor

    // create socket
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd < 0)
    {
        printf("error: clients socket to server: creation failed\n");
        exit(-1);
    }

    // set up server address struct
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    // server_addr.sin_addr.s_addr = INADDR_ANY;

    // connect to the server
    if (connect(server_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        printf("error: connection with the server failed\n");
        exit(-1);
    }

    return server_socket_fd;
}
