#include "sockets.h"

// gets a line from stdin until '\n' is found
void get_line(char *line, int line_size)
{
    bzero(line, line_size);        // zero out line[]
    fgets(line, line_size, stdin); // get a line (end with \n) from stdin
    line[strlen(line) - 1] = 0;    // kill \n at end
}

// sends a message through a socket
int send_msg(int socket_fd, char *msg, int line_size)
{
    if (write(socket_fd, msg, line_size) < 0)
    {
        fprintf(stderr, "error sending data\n");
        exit(-1);
    }
}

// recieves a message through a socket
int recieve_msg(int socket_fd, char *msg, int line_size)
{
    if (read(socket_fd, msg, MAX_BUFFER_SIZE) < 0)
    {
        fprintf(stderr, "error receiving response\n");
        exit(-1);
    }
}
