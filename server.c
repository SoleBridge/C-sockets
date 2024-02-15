#include "server.h"
#include "sockets.h"

// the 'main' function for the server
int main(int argc, char *argv[], char *env[])
{
    // variables
    struct sockaddr_in server_addr, client_addr; // server and client addr structs
    int pid, enableReuse, client_exit_code;      // number of bytes, a dummy variable, and the exit code of the client
    int server_socket_fd, client_socket_fd;      // server and client socket file descriptors
    char line[MAX_BUFFER_SIZE];                  // message buffer

    pid = getpid(); // get the pid

    server_start(&server_socket_fd, &client_socket_fd);

    // connection loop:
    while (1)
    {
        server_accept_client(&server_socket_fd, &client_socket_fd, &client_addr);

        // create a new process to deal with client
        if ((pid = fork()) == 0) // child process (handle client)
        {
            close(server_socket_fd); // close the server socket in the child process
            printf("[%d] created to handle %s:%d\n", (pid = getpid()), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            client_exit_code = server_handle_client(argc, argv, env, &client_addr, client_socket_fd, line); // handle the client
            printf("[%d] client disconnected\n", pid);
            exit(0);
        }
        else // parent process (connection acceptor)
        {
            close(client_socket_fd); // acceptor done with client
            continue;                // go on to accept more
        }
    }
    return 0;
}

// start up the server: create socket, bind, and listen
int server_start(int *server_socket_fd, int *client_socket_fd)
{
    struct sockaddr_in server_addr; // server and client addr structs
    int pid, enableReuse;
    socklen_t addrLen; // address length

    pid = getpid(); // get the pid

    // create socket:
    if ((*server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("[%d] error: socket creation failed\n", pid);
        exit(-1);
    }

    // set socket options, server address struct:
    enableReuse = 1;
    if (setsockopt(*server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(int)) == -1)
    {
        printf("[%d] error: setting socket option\n", pid);
        exit(-1);
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DEFAULT_SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_SERVER_IP);
    // servAddr.sin_addr.s_addr = INADDR_ANY;

    // bind the server socket:
    if ((bind(*server_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) != 0)
    {
        printf("[%d] error: socket bind failed\n", pid);
        exit(-1);
    }

    // listen for connection:
    if ((listen(*server_socket_fd, 5)) != 0)
    {
        printf("[%d] error: listen failed\n", pid);
        exit(-1);
    }
    printf("[%d] listening on port %d\n", pid, DEFAULT_SERVER_PORT);
    return 0;
}

// wait, and connect to client
int server_accept_client(int *server_socket_fd, int *client_socket_fd, struct sockaddr_in *client_addr)
{
    socklen_t addrLen;  // address length
    int pid = getpid(); // process ID - for messages

    // accept a connection:
    printf("[%d] ready to accept client\n", pid);
    addrLen = sizeof(*client_addr);
    *client_socket_fd = accept(*server_socket_fd, (struct sockaddr *)client_addr, &addrLen); // Try to accept a client connection as socket descriptor cfd
    if (client_socket_fd < 0)
    {
        printf("[%d] error: accept error\n", pid);
        exit(-1);
    }
    // connection is made
    printf("[%d] accepted a client: %s:%d\n", pid, inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
}

// the function called when the server deals with a client
int server_handle_client(int argc, char *argv[], char *env[], struct sockaddr_in *client_addr, int client_socket_fd, char line[MAX_BUFFER_SIZE])
{
    int pid = getpid();
    while (1)
    {
        char *tokens[MAX_BUFFER_SIZE]; // tokens buffer
        int n_tokens = 0;

        memset(line, 0, MAX_BUFFER_SIZE);
        int n_bytes = read(client_socket_fd, line, MAX_BUFFER_SIZE); // reads line from client
        printf("[%d] message from client [%s:%d]: \n\"%s\"\n", pid, inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port), line);

        // if(!strcmp(line, "my name is")) {
        //     memset(line, 0, sizeof(line));
        //     sprintf(line, "chica chica slim shady\n");
        // }
        line_tok(line, MAX_BUFFER_SIZE, tokens, &n_tokens);
        if (n_tokens == 0)
            break;

        n_bytes = write(client_socket_fd, line, MAX_BUFFER_SIZE); // respond to client
    }
    return 1;
}
