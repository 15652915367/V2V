#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <deque>


#include "main.h"

using namespace muduo;
using namespace muduo::net;
using namespace std;

//------read file for test
    #define FCW_HV "FCW_hv.csv"
    #define FCW_RV "FCW_rv.csv"
    #define LTA_HV "LTA_hv.csv"
    #define LTA_RV "LTA_rv.csv"
    #define EBW_HV "EBW_hv.csv"
    #define EBW_RV "EBW_rv.csv"
//------read file for test

/*当按下 ctrl+c 中断程序的时候作一些善后工作 
  */
void Stop(int signo) 
{
    printf("\n CTRL+C ! stop!!!\n");
    ofs.close();printf("ofs.close()\n");
    rvMap.~RvMap();printf("~RvMap\n");
    
    canbus.~Canbus();printf("~Canbus\n");
    sock_2Android.~Sock_2Android();printf("~Sock_2Android\n");
    v2v.~V2V();
    _exit(0);
}

/*  Start thread
*
*/
int main(int argc, char* argv[])
{  
    //为 ctrl+c 信号注册一个回调
    signal(SIGINT, Stop);
    //check the usage of main
    //and get id
    char ID[VID_SIZE];
    memset(ID,0,VID_SIZE);
    if (argc != 2)	{
        printf("usage: %s (id)\n",argv[0]);
        return 0;
    } else{
		memcpy(ID, argv[1], VID_SIZE);
	}
    
    int obursu;
    printf("OBU: 1      RSU:2  ");
    cin>>obursu;
    if (obursu == 2) {
        int if_warn_by_cloud;
        printf("always warn: 1      warn by cloud:2  ");
        cin>>if_warn_by_cloud;
        RsuThread(ID, if_warn_by_cloud);
        return 0;
    }else if(obursu == 1){

        hvdata.setVid(ID);

//      //(数据来源读文件,集成测试
//      int scene, hvrv;
//      const char* filename;
//      cout<<"which scene: ";
//      cin>>scene;
//      cout<<"hv: 1        rv: 2     hv/rv : ";
//      cin>>hvrv;
//
//      switch (scene) {
//      case 1:{
//              if (hvrv == 1)          filename = FCW_HV;
//              else if (hvrv == 2)     filename = FCW_RV;
//          }break;
//
//      case 2:{
//              if (hvrv == 1)          filename = LTA_HV;
//              else if (hvrv == 2)     filename = LTA_RV;
//          }break;
//
//      case 3:{
//              if (hvrv == 1)          filename = EBW_HV;
//              else if (hvrv == 2)     filename = EBW_RV;
//          }break;
//
//      default:break;
//
//      }
//      //数据来源读文件,集成测试)

        //开启两个子线程, 使用的是muduo库的函数, 写法上更简便
        //Thread hv_thread(HvThread);


        Thread readCan_thread(CANThread);
//        Thread hv_thread(boost::bind(HvThread, filename));
        Thread hv_thread(HvThread);
        Thread recv_thread(RecvThread);


      readCan_thread.start();
        hv_thread.start();
        recv_thread.start();


      readCan_thread.join();
        hv_thread.join();
        recv_thread.join();

    }


    return 0;
}
