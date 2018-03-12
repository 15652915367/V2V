#include "CANbus.h"


Canbus::Canbus(bool f){
	fd = socket_init("can0");//init
	//true: fliter on false: fliter off 
	if(f){
		can_filter_set(fd);
		cout<<"init obj succ bind can0 set fliter fd: "<<fd<<endl;
	}else{
		cout<<"init obj succ bind can0 no fliter fd: "<<fd<<endl;
	}

	cnt = 0;
	get_cnt = 0;
	
	hvCANData.turning_light_state_e = INACTIVE;
	hvCANData.brake_state_e = SWITCH_OFF;
	hvCANData.speed_value_state_e = VALUE_STATE_UNRELIABLE;
	hvCANData.brake_value_state_e = BRAKE_VALUE_OK;
}

Canbus::~Canbus(){
	socket_release(fd);
}

void Canbus::start(){


		if(can_recvframe(fd) != NULL){
			mutex_.lock();
			analysis(frame.can_id, frame.data, &hvCANData);
			cnt++;
			mutex_.unlock();
            usleep(1000);
		}

}

bool Canbus::getCANData(CANData &canData){
	MutexLockGuard lock(mutex_);
	if(get_cnt != cnt){
		memcpy(&canData, &hvCANData, sizeof(CANData));
        canData.if_can_valid = true;
        get_cnt = cnt;
		return true;
	}
	else{
		memset(&canData, 0, sizeof(CANData));
        canData.if_can_valid = false;
		return false;
	}
}

int Canbus::get_can_if_index(int fd, const char *interface)
{
    
    struct ifreq ifr;
    
    memset(&ifr, 0, sizeof(ifr));

    strcpy(ifr.ifr_name, interface);
    ioctl(fd, SIOCGIFINDEX, &ifr);

    return ifr.ifr_ifindex;
}

int Canbus::socket_init(const char *interface)
{
   int can_fd;
   int if_index = 0;
   struct sockaddr_can addr;
	int loopback = 0; // 0 表示关闭, 1 表示开启( 默认)
	
   can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
   if (can_fd < 0) {
       //fprintf(stderr, "Try to create can socket failed, %s...\n", strerror(errno));
       return -1;
   }
	setsockopt(can_fd, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
   if (NULL != interface)
       if_index = get_can_if_index(can_fd, interface);

   addr.can_family = AF_CAN;
   /* index 0 means to bind to all interfaces */
	addr.can_ifindex = if_index;

   bind(can_fd, (struct sockaddr *)&addr, sizeof(addr));

   return can_fd;
}

void Canbus::socket_release(int fd)
{
    close(fd);
}



void Canbus::can_filter_set(int fd)
{
    struct can_filter rfilter[2];
	//unsigned int id[8] = {0x120, 0x3F0, 0x3A0, 0x280, 0x4A0, 0x1A0, 0x660, 0x3E0};
	unsigned int id[2] = {0x280, 0x4A0};
    /* <received_can_id> & mask == can_id & mask */
    
	for(int i = 0; i< 2 ;i++){
		rfilter[i].can_id  = id[i];
    	rfilter[i].can_mask = 0xfff;
	}
	setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

}

void Canbus::can_filter_set(unsigned int id, unsigned int mask)
{
    struct can_filter rfilter;
    /* <received_can_id> & mask == can_id & mask */
	rfilter.can_id  = id;
	rfilter.can_mask = mask;
	setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
}

struct can_frame* Canbus::can_recvframe(int fd)
{	
	int nbytes = 0;
	//int flags = fcntl(fd, F_GETFL, 0);        //获取文件的flags值。
   // fcntl(fd, F_SETFL, flags | O_NONBLOCK);   //设置成非阻塞模式；
    socklen_t len = sizeof(_addr);
	memset(&frame, 0, sizeof(frame));
    if (fd > 0) {
		nbytes = read(fd, &frame, sizeof(struct can_frame));
        if (nbytes < 0) {
			return NULL;
        }
    }else{
		return NULL;
    }
	printf("recv id: 0x%x\n",frame.can_id);
    for (int i = 0; i < frame.can_dlc; i++) {
        printf("%#x ", frame.data[i]);
    }
    printf("\n");

    return frame_p;    
}




void Canbus::analysis(int head, unsigned char* data, CANData* canBusData_t)
{
	
	switch (head)
	{
	
	case 0x280:
		{
			if(data[2] == 0x2){
				canBusData_t->brake_state_e = SWITCH_ON;
				cout<<"IIIIIIIIIIIIIIIIIIIIBrake ON"<<endl;
			}
			if(data[2] == 0){
				canBusData_t->brake_state_e = SWITCH_OFF;
				cout<<"IIIIIIIIIIIIIIIIIIIIBrake OFF"<<endl;
			}
			break;
		}
	case 0x4a0:
		{
			//unsigned char turnLightSwitch = (data[0] & 0x60) >> 5;// 2 bits
			unsigned char turnLightSwitch = (data[0] & 0x03);// 2 bits
			switch(turnLightSwitch)
			{
			case 0x00:
				{
					canBusData_t->turning_light_state_e = INACTIVE;
					cout<<"IIIIIIIIIIIIIIIIIIIITurning light inactive/default"<<endl;
					break;
				}
			case 0x01:
				{
					canBusData_t->turning_light_state_e = TURN_LEFT;
					cout<<"IIIIIIIIIIIIIIIIIIIITurn Left Light"<<endl;
					break;
				}
			case 0x02:
				{
					canBusData_t->turning_light_state_e = TURN_RIGHT;
					cout<<"IIIIIIIIIIIIIIIIIIIITurn Right Light"<<endl;
					break;
				}
			case 0x03:
				{
					canBusData_t->turning_light_state_e = TURNING_LIGHT_NOT_USED;
					cout<<"IIIIIIIIIIIIIIIIIIIITurning Light not used"<<endl;
					break;
				}
			}
			break;
		}
	default:
		{
			cout<<"can't analyse this head."<<endl;
			break;
		}
	}
}


