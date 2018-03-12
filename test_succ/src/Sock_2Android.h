#ifndef _SOCK_2ANDROID_H
#define _SOCK_2ANDROID_H

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
using namespace std;
#define PORT 8888
#define BUFFER_SIZE 1400

class Sock_2Android
{
public:
    Sock_2Android(const char* ip = "127.0.0.1",int port = PORT);
	~Sock_2Android();
    void sendmsg(void*,int);

private:
    int sock;
    //send
    struct sockaddr_in android_addr;
};

#endif /*_SOCK_2ANDROID_H*/
