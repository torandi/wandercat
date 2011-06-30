#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/network.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "common.h"
#include "network.h"

#define CMD(str) strncmp(buffer,3,str) == 0

/**
 * Protocol:
 * [3 chars cmd][space][optional parameters]
 * Cmds:
 * mov from_x from_y to_x to_y
 */

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

/**
 * Handles network traffic.
 */
void network() {
	char buffer[1024];
	int size;
	struct sockaddr src_addr;
	socklen_t addrlen;

	if(select(sockfd+1,&readset,NULL,NULL,&tv) > 0) {
		size = recvfrom(sockfd, buffer, 1024, 0, &src_addr, &addrlen);
		if(size >= 3) {
			if(CMD("mov")) {
				int16_t from_x, from_y, to_x, to_y;
				sscanf(buffer+4,"%i %i %i %i",&from_x,&from_y, &to_x, &to_y);
				pos_cat.x = ntohs(to_x);
				pos_cat.y = ntohs(to_y);
				pos_cat_prev.x = ntohs(from_x);
				pos_cat_prev.y = ntohs(from_y);

				if(pos_cat == pos_self) {
					//Ack server
					sendto(sockfd, "ack", 3, 0, (sockaddr*) &broadcast_addr, sizeof(struct sockaddr_in));
					owner = true;
				}
			}
		} else {
			fprintf(stderr,"Recieved invalid data\n");
		}
	}
}

