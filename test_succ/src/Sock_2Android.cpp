﻿#include "Sock_2Android.h"

//注意:本函数参数有默认值,在头文件的声明处
//通过127.0.0.1回环地址的socket传输 来实现程序与安卓端显示部分的数据交互
Sock_2Android::Sock_2Android(const char* ip ,int port){
        //构造函数,做好和安卓端的通信准备
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("socket() Failed!");
        }
        bzero(&android_addr,sizeof(struct sockaddr_in));

        android_addr.sin_family = AF_INET;
		android_addr.sin_addr.s_addr = inet_addr(ip);
		android_addr.sin_port = htons(port);  
}

void Sock_2Android::sendmsg(void* my_data,int size){

    if((sendto(sock,my_data,size,0,(struct sockaddr *)&android_addr,sizeof(android_addr)))<0)
    {
        perror("sendmsg Failed!");
    }
}

Sock_2Android::~Sock_2Android(){
	close(sock);
	printf("Sock_2Android closed\n");
}
