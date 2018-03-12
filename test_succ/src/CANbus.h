#include<sys/ioctl.h>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/if.h>
#include <linux/sockios.h>

#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Logging.h>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>

#include "Struct.h"
#define DEFAULT_CAN_INTERFACE "can0"
#define DEFAULT_SEND_TIMES 100

using namespace muduo;
using namespace muduo::net;
using namespace std;

#define ESP_ENGINE_TORQUE_REQ 0x120
#define ESP_GENERAL_STATUS_YAW_SENSOR 0x3F0
#define EMS_ENGINE_IDLE_AND_VSPEED 0x3A0
#define EMS_GENERAL_STATUS 0x280
#define BCM_GENERAL_STATUS 0x4A0
#define EMS_ENGINE_SPEED_AND_TORQUE 0x1A0
#define BCM_GENERAL_STATUS_2 0x660
#define TCU_GENERAL_STATUS 0x3E0

class Canbus{
private:
	int get_can_if_index(int fd, const char *interface);
	int fd;
  	struct sockaddr_can _addr;
    int socket_init(const char *interface);
    void socket_release(int fd);
   
    void can_filter_set(int );
    struct can_frame* can_recvframe(int fd);
	struct can_frame* frame_p;
	struct can_frame frame;
	void analysis(int , unsigned char* ,CANData*);
	CANData hvCANData;
  	MutexLock mutex_;

public:	
 
	Canbus(bool);
	~Canbus();
	void start();
	void can_filter_set(unsigned int, unsigned int);
	

	long cnt;
	long get_cnt;
    //将can数据写进参数的结构体, 同时返回can数据此时是否有效
	bool getCANData(CANData &canData);
};


