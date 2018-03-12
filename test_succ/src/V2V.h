#include <iostream>
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h>  
#include<stdio.h>  
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <signal.h>
#include "CodecCrypt.h"
#include "gpsTransformer.h"

#define VD_BUF_SIZE                 4096
#define O_APSMAC_TX_EXDATA_IND      0x1009
#define O_APSMAC_TX_DATA_IND        0x100a
#define O_MACAPS_RX_DATA_IND        0x100b
#define OAL_TASKID_MAC_TX           0xe2

//定义flags:读写,文件长度戳为0  
#define FLAGS O_RDWR | O_NONBLOCK 
//创建文件的权限,用户读、写、执行、组读、执行、其他用户读、执行  
//#define MODE S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH  
typedef unsigned short u16 ;
typedef unsigned long u32 ;

typedef struct//消息头
{
	u16 msg_size;
	u16 dst_id;
	u32 msg_type;
	u32 reserved;
}MSG_HEAD;

typedef struct //v2v之间发送的bsm结构体
{
	double longitude;
	double latitude;
	double altitude;
	double length;
	double direction;
	double velocity;
	double angular_velocity;

	double xacceleration;
	double yacceleration;
}BSM;


class V2V
{
public:
	/*	prepare work, open "/dev/ltevm" for sending/recving msg		--usage
	*/
    V2V();
	~V2V();
	/*	send msg by LTE-Vd 			--usage
	*	 -senddata: the msg to be sent;	
	*	 -size: size of msg you wish to send;		--para
	*/
    int sendmsg(void* senddata, int size);//返回-1发送失败,0成功,-2overflow
	
	/*	recv msg 	--usage
	*
	*/
    int recvmsg(void* recvbuf);

    /*  这两个函数增加了asn编解码 和 aes加解密, 内部调用了sendmsg()  /  recvmsg()
    */
    int send(Pkt &pkt);
    int recv(Pkt &pkt);

private:

    GpsTransformer gpsTrans; // gps transformer
    const char* pathname;
    char recvdata[VD_BUF_SIZE];
	int fd;//文件描述符
};

