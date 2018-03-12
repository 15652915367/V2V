#ifndef _RVDATA_H
#define _RVDATA_H

#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Logging.h>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <string>

#include <stdio.h>

#include "Struct.h"
#include "gpsTransformer.h"
#include "Scenes.h"
//他车历史数据数量
#define RV_HIS 5
#define OVER_TIME 500

using namespace muduo;
using namespace muduo::net;
using namespace std;

/*  管理他车数据, 作为map中的元素
*/

class RvData : boost::noncopyable{
public:
	RvData(char* vid){
        id = vid;
        num_valid = 1;
        memset( trace, 0, RV_HIS*sizeof(PktEventData) );
        memset( &rvToAndroid, 0, sizeof(RvToAndroid));
    };
    //自定义拷贝构造函数, 因为数据成员中有一个是noncopyable的,无法直接套用默认的拷贝构造函数
    //因为本类的对象是要存进map里的, 不可避免会出现需要调用拷贝构造函数的场合
    RvData(const RvData& that);
	~RvData(){};

	/*	update trace info.			--usage
	*	nothing to say about para.					--para
	*/
    /*  本类的主要函数
    */
	void update(HvData &hvdata, Pkt &pkt);
    void print_data();

    PktEventData  getRvCurrentData();
    RvToAndroid getRvCurrentDataToAndroid();
    //统计未过时数据,返回值为当前未过时的数据数,若为0,则有必要从map中移除该对象
    int count_data_valid();
    int getNumValid();

private:
    /*for lock*/
    MutexLock mutex_;

    string id;
	PktEventData  trace[RV_HIS];// history
    RvToAndroid rvToAndroid;
    //未过时的数据项数,为0时有必要删除该对象
    int num_valid;
};



#endif
