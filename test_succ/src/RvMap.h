#include "RvData.h"

#ifndef _RVMAP_H
#define _RVMAP_H

#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Logging.h>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <map>

#include <stdio.h>
#include <sstream>
#include<iomanip>

#include "Struct.h"
#include "gpsTransformer.h"
#include "HvData.h"
#include "RvData.h"

using namespace muduo;
using namespace muduo::net;
using namespace std;

class RvMap{
public:
    RvMap():msg_count(0){};
    //由于该类中有申请堆空间, 所以需要重写一个析构函数
    ~RvMap();
    //将收到的他车数据与本车数据进行预警判断, 将结果存入map
	void input(HvData &hvdata, Pkt &pkt);
    void print_data();
    void remove_old_data();
    //整理出发送给安卓的数据
    //参数是本车数据的对象
    long data_to_Transmission(HvData &hvData, char* buffer, string &dataToCloud);

private:
    /*for lock*/
    MutexLock mutex_;
    map<string, RvData* > dict;
    long msg_count;
};



#endif
