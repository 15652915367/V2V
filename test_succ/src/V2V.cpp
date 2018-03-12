#include "V2V.h"

using namespace std;
#define SELF_PROTOCOL 0x8010
#define BUFFER_SIZE 1450

V2V::V2V(){
 	pathname = "/dev/ltevm";
    if ((fd = open(pathname, FLAGS/*, MODE*/)) == -1) {  
    	printf("open file error\n");  
	}
	printf("open file SUCC!\n");
}
V2V::~V2V(){
 	close(fd);cout<<"file closed\n";
}

/*	send msg by LTE-Vd 			--usage
*	 -senddata: the msg to be sent;	
*	 -size: size of msg you wish to send;		--para
*/
int  V2V::sendmsg(void* senddata, int size){
    //根据大唐提供的lte-vd说明, 以写文件的方式传输, 需要在待传输字符串之前加上一个头, 末尾补0成4的倍数, 再加上4个字节的尾部(int 类型, 告知补了多少字节的0)
    char buff[VD_BUF_SIZE];
    memset(buff, 0, VD_BUF_SIZE);
    MSG_HEAD m;
    MSG_HEAD *msg = &m;
    long pad_size = 0;
    //dst_id 和 msg_type 大唐的说明就是这么写的
	msg->dst_id = OAL_TASKID_MAC_TX;
    msg->msg_type = O_APSMAC_TX_DATA_IND;
    msg->msg_size = size;           //发送的数据长度

    pad_size = (4-(msg->msg_size%4))%4;
    msg->msg_size += (pad_size+4/*sizeof(int)*/);
    if (msg->msg_size > VD_BUF_SIZE) return -2;
//*************add asn********************
    char* ptr = (char*)buff;
	memcpy(ptr, msg, sizeof(MSG_HEAD));		ptr += sizeof(MSG_HEAD);
	memcpy(ptr, senddata, size);				ptr += size;
	memset(ptr, 0, pad_size);			        ptr += pad_size;
	memcpy(ptr, &(pad_size), 4);
//****************************************
    long length, res;
	length = msg->msg_size + sizeof(MSG_HEAD);

printf("msg_size send %ld\t",msg->msg_size);
printf("pad_size send %ld\n", pad_size);

    if(res = write(fd, buff, length) != length)  return -1;
    else return 0;
}

/*	recv msg		--usage
*   将收到的数据存进recvbuf
*/
int V2V::recvmsg(void* recvbuf){
    long length;
    char buff[VD_BUF_SIZE];
    long msg_size,pad_size = 0;

        memset(buff, 0, VD_BUF_SIZE);
        memset(recvdata, 0, VD_BUF_SIZE);

        length = read(fd, buff, sizeof(buff));

        msg_size = ((MSG_HEAD*)((char*)buff))->msg_size;
    	pad_size = *(long*)((char*)buff+sizeof(MSG_HEAD) + msg_size - sizeof(pad_size));

    
printf("msg_size recv %ld\t",msg_size);
printf("pad_size recv %ld\n", pad_size);
    if (msg_size > VD_BUF_SIZE)     return -1;

    memcpy(recvbuf, buff + sizeof(MSG_HEAD), msg_size - pad_size - sizeof(pad_size));

    //return (void*)((char*)recvdata);
    return (msg_size-pad_size-sizeof(pad_size));
}



int V2V::send(Pkt &pkt){
    char encryptedBuf[BUFFER_SIZE] = "";
    int encryptedBufLength = 0;
    int ans = CodecCrypt::encode_encrypt(&pkt, sizeof(pkt), encryptedBuf, encryptedBufLength) ;
    if (ans == -1)      return -1;
    if (encryptedBufLength > BUFFER_SIZE)       return -2;
    else if (encryptedBufLength <= 0)       return -1;

    return sendmsg(encryptedBuf, encryptedBufLength);
}

int V2V::recv(Pkt &pkt){
    char recvbuf[BUFFER_SIZE] = "";
    int recvbufLength = recvmsg(recvbuf);
    if (recvbufLength<0)    return recvbufLength;
    int code = CodecCrypt::decrypt_decode(recvbuf, recvbufLength, pkt);
    gpsTrans.transformer(pkt.payload.bsmData.lng, pkt.payload.bsmData.lat, pkt.payload.bsmData.x, pkt.payload.bsmData.y);
    return code;
}
