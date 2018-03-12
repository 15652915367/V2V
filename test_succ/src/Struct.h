#ifndef    _STRUCT_H
#define    _STRUCT_H

#include <sys/time.h>    // for gettimeofday()
#include <iostream>
using namespace std;

#define VID_SIZE 17
#define RSUID_SIZE 17
#define SCENE_NUM 4

    

enum ValueState
    {
    	VALUE_STATE_OK,
    	VALUE_STATE_NOT_USED,
    	VALUE_STATE_UNRELIABLE
    };
enum BrakeValueState
    {
    	BRAKE_VALUE_OK,
    	BRAKE_VALUE_UNRELIABLE
    };
enum SwitchState
    {
    	SWITCH_OFF,
    	SWITCH_ON
    };
enum TurningLightState
    {
    	INACTIVE,
    	TURN_LEFT,
    	TURN_RIGHT,
    	TURNING_LIGHT_NOT_USED
    };
enum TurningLightFailure
    {
    	NO_DISPLAY,
    	FAILURE
    };

typedef struct CANData{
    bool if_can_valid;
    /*速度*/
    ValueState speed_value_state_e; //可信     未使用    不可信
	float speed_f;                                  //数值
    /*刹车*/
    BrakeValueState brake_value_state_e;    //可信     不可信
	SwitchState brake_state_e;                      //未刹车  刹车     //
    /*转向灯*/
    TurningLightState turning_light_state_e;                //默认       左转灯     右转灯     未使用        //
   
}CANData;

typedef struct {
    char vid[VID_SIZE];
	double lng;
	double lat;
	double x;
	double y;
    struct timeval tv;
    double heading;
	double speed;
    double acce;
    CANData canData;
}BsmData;

typedef struct {
    char rsuid[RSUID_SIZE];
	double lng;
	double lat;
	double x;
	double y;    
    struct timeval tv;
    long jam_level;//0 1 2 3 4 5

}JamData;

enum Pkt_type
    {
        BSM_MSG,
        JAM_MSG
    };

typedef union {
    BsmData bsmData;
    JamData jamData;
}Payload;

typedef struct {
    Pkt_type pkt_type;
    Payload payload;
}Pkt;


typedef struct {
    char Warning[SCENE_NUM];
    double Distance;
    double Collision_time;
    double Deviation_angle; //他车相对于本车的方向
    Pkt pkt;
}PktEventData;


//给安卓的本车部分数据结构
typedef struct {
    long speedE2;   //结尾的E2: 将原始值*100 后取整发出, 可以减少发送字节数
    long headingE2;
    char light;
    char brake;

}HvToAndroid;
//给安卓的他车部分数据结构
typedef struct {
    char vid[VID_SIZE];
    char Warning[SCENE_NUM];
    long DistanceE2;

}RvToAndroid;   //包括rsu





#endif
