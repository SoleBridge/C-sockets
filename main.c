#include "server.h"
#include "client.h"

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
