#ifndef _SCENES_H
#define _SCENES_H

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Struct.h"
#include "HvData.h"
using namespace std;

#define NO_ALARM    0x00
#define GREEN           0x01
#define YELLOW         0x02
#define RED               0x03

#define FCW     0
#define LTA      1
#define EBW     2
#define TJW     3

#define FORWARD_ANGLE_DIFF 80       //认为是前方的角度偏差范围
#define JAM_WARNING_ANGLE_DIFF 40       //认为是前方的角度偏差范围
#define SAME_DIRECTION_DIFF 30     //认为是同向行驶的角度偏差范围
#define SAME_LANE_ANGLE_DIFF 25     //认为是应该报警的角度偏差范围
//筛选是急刹行为的加速度阈值
#define BRAKE_ACCE_CAN_VALID      -1
#define BRAKE_ACCE_CAN_INVALID      -2

#define LANE_WIDTH 4
#define PI 3.141592653589793


double ANGLE(double angle);

/*   计算出  angle1 是远车相对于本车行驶方向的偏差角, angle2 是两车行驶方向角的差
*/
void calculate_paras(BsmData &hv_bsm_data, BsmData &rv_data, double &angle1, double &angle2, double &Distance);

/*   计算出  angle1 是rsu相对于本车行驶方向的偏差角
*/
void calculate_paras(BsmData &hv_bsm_data, JamData &jam_data, double &angle1, double &Distance);



/*   本文件的主要函数, 将本车数据, 他车数据写入处理, 最后将结构写入一个更大的结构体 pktEventData, 返回预警的等级
*/
int Judge(HvData &hv_class, Pkt &pkt_data, PktEventData &pktEventData);

/*    预警等级判断函数, 根据碰撞时间和距离判断
*/
int warning_judge(double collision_time,double portrait_distance_);

#endif
