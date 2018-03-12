#include "RvData.h"
#include "tools.h"

using namespace std;

/* 利用收到的pkt包, 来更新本车数据
*/
void RvData::update(HvData &hvdata, Pkt &pkt){
    PktEventData pktEventData;  memset(&pktEventData, 0, sizeof(PktEventData));
    
    int level = Judge(hvdata, pkt, pktEventData);       //作预警判断, 将处理后的相应数据修改pktEventData, 并且返回预警等级
    MutexLockGuard lock(mutex_);

//    if (level !=0 ) {
        for (int i = RV_HIS - 1; i > 0; i--) {                          //他车历史数据后移
            memcpy(&(trace[i]),&(trace[i-1]),sizeof(PktEventData));
        }
        trace[0] = pktEventData;
//        print_data();
        //准备发给安卓的结构体
        memcpy(rvToAndroid.vid, pktEventData.pkt.payload.bsmData.vid, VID_SIZE);
        memcpy(rvToAndroid.Warning, pktEventData.Warning, 4);
        rvToAndroid.DistanceE2 = (long)(pktEventData.Distance*100);
//    }
}

//拷贝构造函数, 现在这部分作废
RvData::RvData(const RvData& that){
    //其实就是除了noncopyable的锁以外的其他数据成员拷过来就是
    this->id = that.id;
    this->num_valid = that.num_valid;
    memcpy(this->trace, that.trace, RV_HIS*sizeof(PktEventData));
    memcpy( &rvToAndroid, &(that.rvToAndroid), sizeof(RvToAndroid));
}

//统计未过时数据,返回值为当前未过时的数据数,若为0,则有必要从map中移除该对象
int RvData::count_data_valid(){
    for (int i=0; i<RV_HIS; i++) {
        struct timeval cur_time;
        gettimeofday(&cur_time, NULL);
        if(delta_time_ms(trace[i].pkt.payload.bsmData.tv, cur_time) > OVER_TIME){       //计算时间差
            num_valid = i;
            return i;
        }
    }
    return RV_HIS;
}

int RvData::getNumValid(){
    return num_valid;
}

PktEventData RvData::getRvCurrentData(){
    return trace[0];
}

RvToAndroid RvData::getRvCurrentDataToAndroid(){
    return rvToAndroid;
}


void RvData::print_data(){
    printf("warning: ");
    printf("%d %d %d %d\n",rvToAndroid.Warning[0], rvToAndroid.Warning[1], rvToAndroid.Warning[2], rvToAndroid.Warning[3]);
    printf("DistanceE2 : \t%d\n",rvToAndroid.DistanceE2);
    if (trace[0].pkt.pkt_type == BSM_MSG) {
        printf("rv ID: \t%s\n", trace[0].pkt.payload.bsmData.vid);
//        printf("rv lng: %lf\n", trace[0].pkt.payload.bsmData.lng);
//        printf("rv lat: %lf\n", trace[0].pkt.payload.bsmData.lat);
        printf("rv heading: %lf\n", trace[0].pkt.payload.bsmData.heading);
        printf("rv speed: \t%lf\n", trace[0].pkt.payload.bsmData.speed);
        printf("rv brake: \t%d\n", trace[0].pkt.payload.bsmData.canData.brake_state_e);
        printf("rv left_light: \t%d\n", trace[0].pkt.payload.bsmData.canData.turning_light_state_e);
    }
    else if (trace[0].pkt.pkt_type == JAM_MSG) {
        printf("rsu ID: \t%s\n", trace[0].pkt.payload.jamData.rsuid);
        printf("rsu lng: \t%lf\n", trace[0].pkt.payload.jamData.lng);
        printf("rsu lat: \t%lf\n", trace[0].pkt.payload.jamData.lat);
        printf("rsu jam_level: \t%ld\n", trace[0].pkt.payload.jamData.jam_level);
    }
    /*打印出他车bsm*/
   
}
