#include "RvMap.h"
#include <string.h>

#define BUFFER_SIZE 1450

using namespace std;

/*  将收到的他车数据与本车数据进行预警判断, 满足条件的将结果存入map
*/
void RvMap::input(HvData &hvdata, Pkt &pkt){
    string id( pkt.payload.bsmData.vid );
    string hid( hvdata.getVid() );
    if(id == hid)  return;
    if(id.size() != VID_SIZE) return;
    if(id.substr(0,4) != "BQBY") return;
    cout<<"into map"<<endl;
    MutexLockGuard lock(mutex_);

    map<string, RvData* >::iterator dict_itr = dict.find(id);

    if(dict_itr == dict.end()) {
        //没找到此rv,创建并加入map
        RvData* rvData_p = new RvData(pkt.payload.bsmData.vid);
        rvData_p -> update(hvdata, pkt);
        dict.insert(make_pair(id, rvData_p));
    }else{
        //找到此rv, 将数据加入
        (dict_itr -> second) -> update(hvdata, pkt);
    }
}

/*  去除map里的多余项
*/
void RvMap::remove_old_data(){
//    MutexLockGuard lock(mutex_);
    map<string, RvData*>::iterator dict_itr = dict.begin();
    while ( dict_itr!= dict.end() ) {
//        cout<<"key in map: "<<dict_itr->first<<endl;

        if((dict_itr -> second) -> count_data_valid() == 0){
            //该id对应项的rv数据均已过时,应该删除
            delete(dict_itr -> second);
            //(dict_itr -> second) -> ~RvData();
            dict.erase(dict_itr++);
        }else{
            dict_itr++;
        }
    }
}

/*  本函数遍历map去除多余项, 并将要传给安卓的数据写进buffer, 返回数据长度
*/
long RvMap::data_to_Transmission(HvData &hvData, char* buffer, string &dataToCloud){
    //字符流, 用于暂存要发给云平台端的数据
    stringstream ss;

    MutexLockGuard lock(mutex_);
    remove_old_data();
    //  一开始就一个本车
    long car_num = 1;
    //char buffer[BUFFER_SIZE];memset(buffer, 0, BUFFER_SIZE);
    int ptr_A = 0;      ptr_A += sizeof(long);
    //本车数据 >> 安卓端
    HvToAndroid hvToAndroid = hvData.getHvCurrentDataToAndroid();
    memcpy(buffer+ptr_A, &( hvToAndroid ), sizeof(HvToAndroid));       ptr_A += sizeof(HvToAndroid);
    //本车数据 >> 云平台端
    BsmData hvToCloud = hvData.getHvCurrentData();
    ss<<hvToCloud.vid<<','
        <<setprecision(9)<<hvToCloud.lng<<','
        <<setprecision(9)<<hvToCloud.lat<<','
        <<setprecision(9)<<hvToCloud.x<<','
        <<setprecision(9)<<hvToCloud.y<<','
        <<setprecision(9)<<hvToCloud.tv.tv_sec<<','
        <<setprecision(9)<<hvToCloud.tv.tv_usec<<','
        <<hvToCloud.heading<<','
        <<hvToCloud.speed<<','
        <<hvToCloud.acce<<','
        <<hvToCloud.canData.brake_value_state_e<<','
        <<hvToCloud.canData.brake_state_e<<','
        <<hvToCloud.canData.turning_light_state_e<<',';
    //遍历map
    map<string, RvData* >::iterator dict_itr = dict.begin();
    for (; dict_itr!= dict.end(); dict_itr++) {
        RvToAndroid rvToAndroid = (dict_itr -> second) -> getRvCurrentDataToAndroid();
        PktEventData rvToCloud = (dict_itr -> second) -> getRvCurrentData();
//      if ( (rvToAndroid.Warning[0] == 0x00)
//           && (rvToAndroid.Warning[1] == 0x00)
//           && (rvToAndroid.Warning[2] == 0x00)
//           && (rvToAndroid.Warning[3] == 0x00)  ) {/*若该rv完全没有报警, 就跳过该rv*/}
//      else{
            //car 计数+1
            car_num++;
            (dict_itr->second) -> print_data();
            //他车数据 >> 安卓端
            memcpy(buffer+ptr_A, &( rvToAndroid ), sizeof(RvToAndroid));       ptr_A += sizeof(RvToAndroid);
            //他车数据 >> 云平台端
            if (rvToCloud.pkt.pkt_type == BSM_MSG) {
                //bsm
                ss<<(long)rvToCloud.Warning[0]<<','<<(long)rvToCloud.Warning[1]<<','<<(long)rvToCloud.Warning[2]<<','<<(long)rvToCloud.Warning[3]<<','
                    <<setprecision(4)<<rvToCloud.Distance<<','
                    <<(long)rvToCloud.Collision_time<<','
                    <<setprecision(4)<<rvToCloud.Deviation_angle<<','
                    <<rvToCloud.pkt.pkt_type<<','
                    <<rvToCloud.pkt.payload.bsmData.vid<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.bsmData.lng<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.bsmData.lat<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.bsmData.x<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.bsmData.y<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.bsmData.tv.tv_sec<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.bsmData.tv.tv_usec<<','
                    <<rvToCloud.pkt.payload.bsmData.heading<<','
                    <<rvToCloud.pkt.payload.bsmData.speed<<','
                    <<rvToCloud.pkt.payload.bsmData.acce<<','
                    <<setprecision(4)<<rvToCloud.pkt.payload.bsmData.canData.brake_value_state_e<<','
                    <<setprecision(4)<<rvToCloud.pkt.payload.bsmData.canData.brake_state_e<<','
                    <<setprecision(4)<<rvToCloud.pkt.payload.bsmData.canData.turning_light_state_e<<',';
            }else{
                //jam
                ss<<(long)rvToCloud.Warning[0]<<','<<(long)rvToCloud.Warning[1]<<','<<(long)rvToCloud.Warning[2]<<','<<(long)rvToCloud.Warning[3]<<','
                    <<setprecision(4)<<rvToCloud.Distance<<','
                    <<(long)rvToCloud.Collision_time<<','
                    <<setprecision(4)<<rvToCloud.Deviation_angle<<','
                    <<rvToCloud.pkt.pkt_type<<','
                    <<rvToCloud.pkt.payload.jamData.rsuid<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.jamData.lng<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.jamData.lat<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.jamData.x<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.jamData.y<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.jamData.tv.tv_sec<<','
                    <<setprecision(9)<<rvToCloud.pkt.payload.jamData.tv.tv_usec<<','
                    <<rvToCloud.pkt.payload.jamData.jam_level<<',';
            }
//        }
    }
    //数据 >> 安卓端, 写进buffer
    memcpy(buffer, &car_num, sizeof(long));
            cout<<endl;
            for (int i = 0; i<100; i++) {
                printf("%x ",buffer[i]);
            }
            cout<<endl;

    //数据 >> 云平台端, 写进string
    ss>>dataToCloud;    ss.clear();     ss.str("");//ss 清空
    ss<<(msg_count++)<<','<<(car_num)<<',' ;
//    cout<<"*--*-*-*-*-*-*-*-*-*-*-*-*"<<ss.str()<<endl;
    dataToCloud = ss.str() + dataToCloud + '\n';
    return ptr_A;
}

/*  打印出map的内容
*/
void RvMap::print_data(){
    MutexLockGuard lock(mutex_);
    cout<<"------------------map info-----------------------\n";
    cout<<"map size: "<<dict.size()<<endl;
    cout<<"------------------map info-----------------------\n";
}

RvMap::~RvMap(){
    MutexLockGuard lock(mutex_);
//    cout<<"Ready to ~RvMap()"<<endl;
    map<string, RvData*>::iterator dict_itr = dict.begin();
    while ( dict_itr!= dict.end() ) {
            //(dict_itr -> second) -> ~RvData();
            delete(dict_itr -> second);
            dict_itr++;
            //dict.erase(dict_itr++);
    }
//    cout<<"Succ to ~RvMap()"<<endl;
}

