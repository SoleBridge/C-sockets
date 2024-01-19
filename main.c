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

#define SERVER_IP			"127.0.0.1"	    // server IP address ( loclahost )
#define SERVER_PORT			8080            // port number
#define MAX_BUFFER_SIZE		1024			// size of buffers (lines and socket buffers)

// gets a line from stdin until '\n' is found
void get_line(char* line, int line_size) {
   	bzero(line, line_size);         // zero out line[]
   	fgets(line, line_size, stdin);	// get a line (end with \n) from stdin
   	line[strlen(line) - 1] = 0;		// kill \n at end
}

// the function called when the server deals with a client
int server_handle_client(int argc, char *argv[], char *env[], int client_socket_fd, char line[MAX_BUFFER_SIZE]) {
	int n_bytes;
	
	while (1) {
		n_bytes = read(client_socket_fd, line, MAX_BUFFER_SIZE);	// reads line from server
		if (n_bytes == 0) {                                   		// empty request
			return 0;												// 	kick client
		}
		n_bytes = write(client_socket_fd, line, MAX_BUFFER_SIZE);	// respond to client
	}
    return 1;
}

// the 'main' function for the server
int server_start_listen(int argc, char *argv[], char *env[]) {
	// variables
	struct sockaddr_in server_addr, client_addr;	// server and client addr structs
    int	pid, enableReuse, client_exit_code ;		// number of bytes, a dummy variable, and the exit code of the client
    int server_socket_fd, client_socket_fd;			// server and client socket file descriptors
    char line[MAX_BUFFER_SIZE];               		// message buffer
    socklen_t addrLen;								// address length
    
    addrLen = sizeof(client_addr);					// get the address length
    pid = getpid();									// get the pid
    
    // create socket:
    if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
    	printf("[%d] error: socket creation failed\n", pid);
    	exit(-1);
    }
    
	// set socket options, server address struct:
	enableReuse = 1;
	if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(int)) == -1) {
    	printf("[%d] error: setting socket option\n", pid);
    	exit(-1);
	}
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); 
	// servAddr.sin_addr.s_addr = INADDR_ANY;

	// bind the server socket:
	if ((bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) != 0) {
    	printf("[%d] error: socket bind failed\n", pid);
    	exit(-1);
    }
    
    // listen for connection:
    if ((listen(server_socket_fd, 5)) != 0) {
    	printf("[%d] error: listen failed\n", pid);
    	exit(-1);
    }
	printf("[%d] listening on port %d\n", pid, SERVER_PORT);
    
    // connection loop:
    while(1) {
        // accept a connection:
        printf("[%d] ready to accept client\n", pid);
        addrLen = sizeof(client_addr);
        client_socket_fd = accept(server_socket_fd,(struct sockaddr *)&client_addr, &addrLen); // Try to accept a client connection as socket descriptor cfd
        if (client_socket_fd < 0) {
        	printf("[%d] error: accept error\n", pid);
        	exit(-1);
        }
        // connection is made
        printf("[%d] accepted a client: IP=%s  PORT=%d\n", pid, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port)); 
        
        // create a new process to deal with client
        pid = fork();
        if (pid) {					// parent process (connection acceptor)
        	close(client_socket_fd); 	// acceptor done with client 
        	continue;					// go on to accept more
        } else {						// child process (handle client)
        	close(server_socket_fd);	// close the server socket in the child process
            printf("[%d] created\n", (pid = getpid()));
            client_exit_code = server_handle_client(argc, argv, env, client_socket_fd, line);	// handle the client
            if(client_exit_code == -1) {
            	printf("[%d] client gone\n", pid);
                close(client_socket_fd);
            }
            printf("[%d] exit\n", pid);
            exit(0);
        }
    }
	return 0;
}

// the 'main' function for the client
int client_main(int argc, char *argv[], char *env[]) {
	struct sockaddr_in server_addr;			// server address 
	char line[MAX_BUFFER_SIZE];				// buffer
	int server_socket_fd, connectStatus;	// server socket file descriptor and connection status

	// create socket
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd  < 0) {
    	printf("error: clients socket to server: creation failed\n");
    	exit(-1);
    }

	// set up server address struct
	bzero(&server_addr, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); 
	// saddr.sin_addr.s_addr = INADDR_ANY; 

    // connect to the server
	connectStatus = connect(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)); 
    if (connectStatus != 0) {
    	printf("error: connection with the server failed\n");
    	exit(-1);
    }
    
    // connected to server; main loop:
    while (1) {
        // get message to send to server:
        get_line(line, MAX_BUFFER_SIZE);
        
        // send message to server:
        if (write(server_socket_fd, line, MAX_BUFFER_SIZE) < 0) {
            printf("error sending data\n");
            exit(-1);
        }

        // get response from server:
        if (read(server_socket_fd, line, MAX_BUFFER_SIZE) < 0) {
            printf("error receiving response\n");
            exit(-1);
        }
        
        // deal with server response:
        printf("Server response: %s\n", line);
    }
    // clean up:
	close(server_socket_fd); // close the server socket
}

// main that chooses to start a server or a client based on CLI
int main(int argc, char *argv[], char *env[]) {
	// check CLI for 's' (server) or 'c' (client)
	if(argc < 2) {
		printf("Usage: %s <s|c>\n", argv[0]);
		exit(-1);
	}
	if (argv[1][0] == 's') {
		server_start_listen(argc, argv, env);
	} else if (argv[1][0] == 'c'){
		client_main(argc, argv, env);
	} else {
		printf("Usage: %s <s|c>\n", argv[0]);
	}
	return 0;
}
