#include "HvData.h"


#define TIME_CYCLE      0.1

//HvData::HvData(char* vid){
//    memcpy(this->vid , vid, VID_SIZE);
//}


/*  设置vid
*
*/
void HvData::setVid(char* vid){
    memcpy(this->vid , vid, VID_SIZE);
}

/*	get speed  -calculate from gps.			--usage
*		cur_x: current X;   ......                                          --para
*/
double HvData::readSpeed(double &cur_x, double &cur_y){
    //拿当前的(x,y)与上一个时刻的(x,y)计算距离,再除以周期0.1s  以得到速度
	return sqrt(pow(cur_x - trace[0].x,2)+pow(cur_y - trace[0].y,2)) / TIME_CYCLE ;
}

 /*	get heading  -calculate from gps.			--usage
*	cur_x: current X;   ......                                          --para
*/
double HvData::readHeading(double &cur_x, double &cur_y){
    double x2 = 0.5*cur_x + 0.3*trace[0].x + 0.2*trace[1].x;
    double y2 = 0.5*cur_y + 0.3*trace[0].y + 0.2*trace[1].y;            //(x2,y2)新点
    double x1 = 0.3*trace[2].x + 0.3*trace[3].x + 0.4*trace[4].x;
    double y1 = 0.3*trace[2].y + 0.3*trace[3].y + 0.4*trace[4].y;       //(x1,y1)旧点
    double heading = (atan2(x2-x1 , y2-y1) / PI*180 );                      //计算角度
    if (heading < 0)    heading += 360;                                               //角度值标准化 0~360
    lane_calculate(x1, y1, x2, y2);
    return  heading;
}
 /*	get accelerate    -calculate from speed.			--usage
	*	cur_v: current speed;   ......                                          --para
	*/
double HvData::readAcce(double &cur_v){
    return (cur_v - trace[0].speed)/TIME_CYCLE;                                 //用相邻两个时刻的速度差计算加速度
}


/*	use lng&lat to update trace info.			--usage
*	nothing to say about para.					--para
*/
void HvData::update(double &lng, double &lat, CANData &canData){
    double x,y=0;
    MutexLockGuard lock(mutex_);//lock until this method ends.
    gpsTrans.transformer(lng, lat, x, y);                   //将lng lat 转换成相应的x y坐标
    double speed;
    double acce;
    double heading;
   
    //速度
    speed = readSpeed(x, y);
//  if (canData.speed_value_state_e == VALUE_STATE_OK) {//  CAN总线的速度可相信
//      speed = canData.speed_f;
//  }else{//  CAN总线的速度不可信
//      speed = readSpeed(x, y);
//  }

    //加速度
    acce = readAcce(speed);
    //方向
    heading = readHeading(x, y);
    if (first_time == true) {
        //如果是第一次写入,则将历史全用第一个数据填满,初始化
        memcpy(trace[0].vid, vid, VID_SIZE);
        trace[0].heading = 0;
        trace[0].lng = lng;
        trace[0].lat = lat;
        trace[0].x = x;
        trace[0].y = y;
        trace[0].speed = 0;
        trace[0].acce = 0;
        gettimeofday( &((trace[0].tv)), NULL );         //获取utc时间
        memcpy(&(trace[0].canData), &canData, sizeof(CANData));
        for (int i = TRACE_SIZE - 1; i > 0; i--) {
            memcpy(&(trace[i]),&(trace[0]),sizeof(BsmData));
        }
        first_time = false;
    }
    else
    {//不是第一次写入,将数据顺序后推,然后将最新的数据存入首部
        for (int i = TRACE_SIZE - 1; i > 0; i--) {
            memcpy(&(trace[i]),&(trace[i-1]),sizeof(BsmData));
        }

        memcpy(trace[0].vid, vid, VID_SIZE);
        trace[0].heading = heading;
        trace[0].lng = lng;
        trace[0].lat = lat;
        trace[0].x = x;
        trace[0].y = y;
        trace[0].speed = speed;
        trace[0].acce = acce;
        gettimeofday( &((trace[0].tv)), NULL );
        memcpy(&(trace[0].canData), &canData, sizeof(CANData));
    }

    hvToAndroid.speedE2 = (long)(trace[0].speed * 100);                 //传输时做一个取整
    hvToAndroid.headingE2 = (long)(trace[0].heading * 100);
    hvToAndroid.light = trace[0].canData.turning_light_state_e;
    hvToAndroid.brake = trace[0].canData.brake_state_e;
    // *发送给安卓/云平台)

}

BsmData HvData::getHvCurrentData(int No){   //这是默认值函数, 默认取0, 即最新数据
    return trace[No];
}

HvToAndroid HvData::getHvCurrentDataToAndroid(){
    return hvToAndroid;
}

line HvData::getLane(){
    return lane;
}

void HvData::lane_calculate(double x1 , double y1 , double x2 , double y2)
{      // Ax + By +C = 0
        lane.A_ = y2 - y1;
        lane.B_ = x1 - x2;
        lane.C_ = x2*y1 - x1*y2;

        //printf("A: %lf  B:  %lf  C:  %lf\n",hv_bsm_data.lane._A,hv_bsm_data.lane._B,hv_bsm_data.lane._C);
}

void HvData::print_data(){
    MutexLockGuard lock(mutex_);//lock until this method ends.
    printf("\n");
    printf("HV vid: \t%s\n",trace[0].vid);
//    printf("HV lng: \t%.5lf\n",trace[0].lng);
//    printf("HV lat: \t%.5lf\n",trace[0].lat);
//    printf("HV x: \t%lf\n",trace[0].x);
//    printf("HV y: \t%lf\n",trace[0].y);
    printf("HV heading: \t%lf\n",trace[0].heading);
    printf("HV speed: \t%lf\n",trace[0].speed);
    printf("HV brake: \t%d\n",trace[0].canData.brake_state_e);
    printf("HV left light: \t%d\n",trace[0].canData.turning_light_state_e);
}
