#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/network.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "common.h"
#include "network.h"

static struct sockaddr_in relay_addr;
static int sockfd;
static fd_set readset;
static struct timeval tv;

void init_network() {

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0) {
		perror("network(): create socket");
		exit(1);
	}

	int one = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &one, sizeof(int)) == -1) {
		perror("network(): setsockopt SO_BROADCAST");
		exit(1);
	}

	if(setsockopt(sockfd, SOL_SOCKET, SO_NONBLOCK, &one, sizeof(int)) == -1) {
		perror("network(): setsockopt SO_NONBLOCK");
		exit(1);
	}

	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_port = htons(PORT);
	broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);	

	FD_ZERO(&readset);
	FD_SET(sockfd,&readset);

	tv.tv_sec = 0;
	tv.tv_usec = 0;
}

void network() {
	if(select(sockfd+1,&readset,NULL,NULL,&tv) > 0) {
			
	}
}
