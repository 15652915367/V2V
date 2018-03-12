#ifndef     _MAIN_H
#define     _MAIN_H

#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>

#include <iostream>
#include <deque>
#include <map>
#include <list>

#include <muduo/base/Logging.h>

#include "V2V.h"
#include "HvData.h"
#include "RvData.h"
#include "RvMap.h"
#include "getGPS.h"
#include "Struct.h"
#include "Scenes.h"
#include "Sock_2Android.h"
#include "CANbus.h"



//------read file for test
    #include "readCSV/readCSV.h"
//------read file for test
using namespace muduo;
using namespace muduo::net;
using namespace std;

#define BUF_SIZE 1450


//global
//实行v2v通信
V2V v2v;
//实行与安卓端通信
Sock_2Android sock_2Android;
//维护本车数据的类
HvData hvdata;

//阻塞读取本车CAN0 接口的数据并解析, 存进 hvCAN .
Canbus canbus(true);
//将处理完的结果暂存在map里,以备进一步的处理或发送给云平台和安卓
//将map封装进一个类
RvMap rvMap;
//写文件,给云平台的进程传数据
ofstream ofs;

/*for lock among threads beneath*/
//MutexLock thread_mutex_;



/*  This thread is for rsu to send jam msg. Only rsu use this thread.(not finish yet)
*   in fact, it is not even a subthread, just a function with a similar name.
*   if flag ==1 , always warn.  if flag == 2 , warn when cloud ask.
*/
void RsuThread(char* ID, int flag){
    //读文件,云平台的进程向文件写rsi报警数据, 由rsi读取
    fstream ifs;
    
    double timestamp = 0.0;

    cout<<"RsuThread"<<endl;
    GpsTransformer gps_Trans;
    while (true) {

        double lng, lat = 0;
        getGPS(lng,lat);//block, every 0.1s  run a time
    
//        double lng = 39.959684;
//        double lat = 116.355068;

        double x,y;
        Pkt pkt;
        memset(&pkt, 0, sizeof(Pkt));
        gps_Trans.transformer(lng, lat, x, y);
        //赋值
        memcpy(pkt.payload.jamData.rsuid, ID, RSUID_SIZE);
        pkt.pkt_type = JAM_MSG;
        pkt.payload.jamData.lng = lng;
        pkt.payload.jamData.lat = lat;
        pkt.payload.jamData.x = x;
        pkt.payload.jamData.y = y;
        gettimeofday( &(pkt.payload.jamData.tv), NULL );
        if (flag == 1) {
            pkt.payload.jamData.jam_level = 1;
        }
        else if (flag == 2) {
            ifs.open("rsu.txt", ios::in);
            if (!ifs) {
                 cout<<"open file: rsu.txt fail"<<endl;
                 return;
            }
            string str;
            while(ifs >> str) {
                cout<<str<<endl;;
                vector<string> v;
                SplitString(str, v,",");
         //       if (timestamp != atof(v[0].c_str()) ) {
                    timestamp = atof(v[0].c_str());
                    pkt.payload.jamData.jam_level = atoi(v[1].c_str());
                    cout<<"timestamp : "<<timestamp<<"level : "<<pkt.payload.jamData.jam_level<<endl;
          //      }else{
          //          cout<<"timestamp same : "<<timestamp<<endl;
          //          pkt.payload.jamData.jam_level = 0;
          //      }

                    str="";
            }
            ifs.close();
        }

        //v2v.sendmsg(&pkt, sizeof(Pkt));
        v2v.send(pkt);
        cout<<"id : \t" << pkt.payload.jamData.rsuid << endl;
        cout<<"lng : \t" << pkt.payload.jamData.lng << endl;
        cout<<"lat : \t"<<pkt.payload.jamData.lat << endl;
        cout<<"x : \t" << pkt.payload.jamData.x << endl;
        cout<<"y : \t" << pkt.payload.jamData.y << endl;
        cout<<"level : \t"<<pkt.payload.jamData.jam_level << endl;
//        sleep(1);
    }
}


void CANThread(){
    //canbus.can_filter_set(0x4a0,0xfff);
    while (1) {
        canbus.start();
        usleep(1000);
    }
}



/*  This thread is to read gps(/CAN) ,  update hvData,   and send
*
*/
//void HvThread(const char* filename){
void HvThread(){
    cout<<"HvThread"<<endl;

    //(数据来源读文件,集成测试
//  readCSV rdcsv(filename);
//  data_read dr = rdcsv.outputdata();
//  int count_ = 0;
    //数据来源读文件,集成测试)

    while (true) {
//(数据来源GPS/CAN

        //暂存本车CAN数据的结构
        CANData hvCAN;
        //can数据是否可用, true可用
        bool if_can_valid = canbus.getCANData(hvCAN);

        double lng, lat = 0;
        getGPS(lng,lat);//block every 0.1s  run a time

        //MutexLockGuard lock(main_mutex_);//lock until this method ends.
        //加上CAN的读取
        hvdata.update(lng,lat,hvCAN);


//数据来源GPS/CAN)

    //(数据来源读文件,集成测试
//      hvCAN.speed_value_state_e = VALUE_STATE_OK;
//      hvCAN.speed_f = dr.speed[count_];
//      hvCAN.brake_value_state_e = BRAKE_VALUE_OK;
//      switch ((int)dr.brake[count_]) {
//      case 1:
//          hvCAN.brake_state_e = SWITCH_ON;
//          break;
//      default:
//          hvCAN.brake_state_e = SWITCH_OFF;
//          break;
//      }
//      switch ((int)dr.left_light[count_]) {
//      case 1:
//          hvCAN.turning_light_state_e = TURN_LEFT;
//          break;
//      default:
//          hvCAN.turning_light_state_e = INACTIVE;
//          break;
//      }
//
//        hvdata.update(dr.lng[count_], dr.lat[count_], hvCAN);


//      cout<<"Data read from file\t"
//              <<dr.no_[count_]<<"  \t"
//  			<<dr.lng[count_]<<"  \t"
//  			<<dr.lat[count_]<<"  \t"
//  			<<dr.speed[count_]<<"  \t"
//  			<<dr.brake[count_]<<"  \t"
//  			<<dr.left_light[count_]<<endl;
//      cout<<"Data in hvCAN\t"
//  			<<(int)hvCAN.turning_light_state_e<<"  \t"
//  			<<(int)hvCAN.brake_state_e<<endl;
//
//        hvdata.print_data();

//        if(count_ < 30)     count_++;
//        else                      exit(1);
    //数据来源读文件,集成测试)

        //取出当前时刻的本车BSM数据
        BsmData hv_bsm_Data = hvdata.getHvCurrentData();
        hvdata.print_data();
        //封装成Pkt 加标志位
        Pkt pkt;
        pkt.pkt_type = BSM_MSG;
        //LTE-V 广播
        memcpy( &(pkt.payload.bsmData), &(hv_bsm_Data ), sizeof(BsmData) );

        //v2v.sendmsg(&pkt, sizeof(pkt));
        int code = v2v.send(pkt);
        if(code  != 0)  cout<<"send failed"<<code<<endl;


        //rmMap数据打印
        rvMap.print_data();
        //初始化要发给安卓端的传输字段
        char buffer[BUF_SIZE]; memset(buffer, 0, BUF_SIZE);
        //调用rvMap的函数, 将数据填充进字段
        string dataToCloud;
        int length = rvMap.data_to_Transmission(hvdata, buffer, dataToCloud);
        //发送数据
        sock_2Android.sendmsg(buffer, length);
        //写进文件
ofs.open("bsm.txt",ios::out);
        ofs<<dataToCloud;
ofs.close();
        //(数据来源读文件,集成测试
//        sleep(1); 
        //数据来源读文件,集成测试)
    }
}




//RecvThread 中运行的任务函数, 将收到的数据包处理,判断预警并存入rvMap的容器
void pktTask(Pkt pkt){
    
    rvMap.input(hvdata, pkt);
}

/*  This thread is to recv msg,  then run the task.
*
*/
void RecvThread(){
    cout<<"RecvThread\n";
    while (true) {
        Pkt pkt_recved;
        //memcpy(&pkt_recved, v2v.recvmsg(), sizeof(Pkt));    //block 阻塞函数
        if(v2v.recv(pkt_recved) != 0)   continue;

        cout<<"*-*-*-*-*-*PKT recved\n";
//      cout<<"pkt_recved id"<<pkt_recved.payload.bsmData.vid<<endl;
//      cout<<"pkt_recved lng"<<pkt_recved.payload.bsmData.lng<<endl;
//      cout<<"pkt_recved lat"<<pkt_recved.payload.bsmData.lat<<endl;
//      cout<<"*-*-*-*-*-*PKT recved\n";
        if (pkt_recved.pkt_type == BSM_MSG) {
            cout<<"recv BSM pkt"<<endl;
            gettimeofday(&(pkt_recved.payload.bsmData.tv), NULL);
        } else if (pkt_recved.pkt_type == JAM_MSG) {
            cout<<"recv RSU pkt"<<endl;
            gettimeofday(&(pkt_recved.payload.jamData.tv), NULL);
        }
        pktTask(pkt_recved);
    }
}




#endif
