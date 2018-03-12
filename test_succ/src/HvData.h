#ifndef _HVDATA_H
#define _HVDATA_H

#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Logging.h>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>

#include <stdio.h>

#include "Struct.h"
#include "gpsTransformer.h"

#define TRACE_SIZE 10
#define PI 3.141592653589793

using namespace muduo;
using namespace muduo::net;
using namespace std;

/*  用于标识一条直线, 此版本代码未使用  
*/
typedef struct{
    double A_;
    double B_;
    double C_;
}line;

/*  其实这个类最好是采用单例模式
*   本类(HvData), 用于存储本车数据
*/
class HvData : boost::noncopyable{
public:
	HvData(){first_time = true;};
	~HvData(){};

    /*  设定本车id(17位)
    */
    void setVid(char* );

	/*	use lng&lat to update trace info.			--usage
	*	nothing to say about para.					--para
	*/
    /*  本类的主要函数
    */
	void update(double &lng, double &lat, CANData &canData);
    /*  打印本车的一些数据
    */
    void print_data();

    /*  获取本车数据(BsmData 结构体), 默认返回当前时刻数据, 可以指定返回任意一项历史数据
    */
    BsmData getHvCurrentData(int No = 0);
    /*  获取本车当前数据(HvToAndroid 结构体)
    */
    HvToAndroid getHvCurrentDataToAndroid();
    /*  获取本车当前运行直线数据(line 结构体, A_B_C_), 现未用到
    */
    line getLane();
    BsmData* getTrace(){
        return trace;
    }
    char* getVid(){
        return vid;
    }

private:
    /*	get speed    -calculate from gps.			--usage
	*	cur_x: current X;   ......                                          --para
	*/
	double readSpeed(double &cur_x, double &cur_y);
    /*	get accelerate    -calculate from speed.			--usage
	*	cur_v: current speed;   ......                                          --para
	*/
	double readAcce(double &cur_v);
    /*	get heading    -calculate from gps.			--usage
	*	cur_x: current X;   ......                                          --para
	*/
    double readHeading(double &cur_x, double &cur_y);
    void lane_calculate(double x1 , double y1 , double x2 , double y2);

    /*for lock*/
    MutexLock mutex_;
    /*  标记是否是第一次向该对象写数据
    */
    bool first_time;

    char vid[VID_SIZE];
    line lane;
    GpsTransformer gpsTrans; // gps transformer
	BsmData trace[TRACE_SIZE];// history

    HvToAndroid hvToAndroid;
};



#endif
