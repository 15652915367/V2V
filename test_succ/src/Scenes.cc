#include "math.h"
#include "Scenes.h"

using namespace std;


/*  根据一个夹角,获得一个标准的角度差(0~180)
*   举个例子: 一个角1度 另一个359度.  输入角度差358/-358度, 返回2度
*/
double ANGLE(double angle)
{
    double a = fabs(angle);
    double b = 360-a;
    return ((a<b)?a:b);
}

/*   计算出  angle1 是远车相对于本车行驶方向的偏差角, angle2 是两车行驶方向角的差, Distance 是车间距
*/
void calculate_paras(BsmData &hv_bsm_data, BsmData &rv_data, double &angle1, double &angle2, double &Distance){
    angle2 = ANGLE(hv_bsm_data.heading - rv_data.heading);

    double rp = (atan2(rv_data.x - hv_bsm_data.x , rv_data.y - hv_bsm_data.y) / PI*180);
    if (rp < 0)     rp += 360;
    angle1 = ANGLE(hv_bsm_data.heading - rp);

    Distance = sqrt(pow(hv_bsm_data.x - rv_data.x,2) + pow(hv_bsm_data.y - rv_data.y,2));
}

/*   计算出  angle1 是rsu相对于本车行驶方向的偏差角, Distance 是距离
*/
void calculate_paras(BsmData &hv_bsm_data, JamData &jam_data, double &angle1, double &Distance){

    double rp = (atan2(jam_data.x - hv_bsm_data.x , jam_data.y - hv_bsm_data.y) / PI*180);
    if (rp < 0)     rp += 360;
    angle1 = ANGLE(hv_bsm_data.heading - rp);

    Distance = sqrt(pow(hv_bsm_data.x - jam_data.x,2) + pow(hv_bsm_data.y - jam_data.y,2));
}


//判断他车在本车运行直线的右前方还是左前方
int left_or_right(BsmData hv_bsm_data, BsmData rv_bsm_data){
    //本车到他车射线的角度, 以正北为0度
    double angle_ = atan2(rv_bsm_data.x - hv_bsm_data.x, rv_bsm_data.y - hv_bsm_data.y) / PI*180;
    if (angle_ < 0)     angle_ += 360;

    double angle_diff = (angle_ - hv_bsm_data.heading);
    if (angle_diff < 0)  angle_diff += 360;
    if (angle_diff <= FORWARD_ANGLE_DIFF && angle_diff > 0)    return 1; //返回1说明 他车在本车运行直线右前方

    angle_diff = (hv_bsm_data.heading - angle_);
    if (angle_diff < 0)  angle_diff += 360;
    if (angle_diff <= FORWARD_ANGLE_DIFF && angle_diff >= 0)    return -1; //返回-1说明 他车在本车运行直线左前方(包括正前方)

    return 0;   //返回0说明 属于其他情况
}

/*  can数据失效时用于判断是否进入左转辅助判断逻辑
*/
bool if_LTA(HvData &hvdata, Pkt &pkt_data, PktEventData &pktEventData){
    //得到本车数据 和 收到的他车的数据
    BsmData hv_bsm_data = hvdata.getHvCurrentData();
    BsmData rv_bsm_data = pkt_data.payload.bsmData;

    double Deviation_angle; //偏差角(标准值, 绝对值)   0~180
    double delta_angle;         //行驶方向差   0~180
    double Distance;              //距离
    calculate_paras(hv_bsm_data, rv_bsm_data, Deviation_angle, delta_angle, Distance);
    printf("Deviation_angle: %lf \tdelta_angle: %lf\tDistance: %lf\n",Deviation_angle, delta_angle, Distance);
    //要进入左转辅助的判断, 有几个条件:
    //1. 他车在本车前方, 即偏差角应该在一定范围内
    if (Deviation_angle > FORWARD_ANGLE_DIFF)       return false;

    //2. 本车若要正常左转, 此时的的行驶速度必然不快, 考虑到可能的误差暂时设定在7m/s以下
    if (hv_bsm_data.speed > 7) {                            return false;}
    //3. 两车对向行驶, 考虑到误差, 暂时设行驶方向差需要大于150度
    //4. 两车距离不应太远, 暂时设距离在60米以内
    if (Distance >60 || delta_angle<150)       return false;

    //5. 他车在本车运行直线的左侧
    if (left_or_right(hvdata.getHvCurrentData(), pkt_data.payload.bsmData) != -1){
        cout<<"rv is not Left Front of hv"<<endl;
        return false;
    }
    cout<<"rv is Left Front of hv"<<endl;

    //能到达这一步逻辑的都是满足以上各项条件的

    //初始化两个计数器:  num_left, 记录历史数据中他车在本车左边的记录数; num_right, 记录历史数据中他车在本车右边的记录数 。
    int num_left = 1;
    int num_right = 0;
    //6. 本车的历史数据里他车在本车的运行直线左侧, 多于在右侧的
    for (int i = 1; i<TRACE_SIZE; i++) {
        int ans = left_or_right(hvdata.getHvCurrentData(i), pkt_data.payload.bsmData);
        switch (ans){
        case 1:
            num_right++;
            break;
        case -1:
            num_left++;
            break;
        default:
            break;
        }
    }
    
    
    cout<<"rv is Right Front of hv : "<<num_right<<endl;
    cout<<"rv is Left Front of hv : "<<num_left<<endl;

    if (num_left >= num_right){
        //如果历史数据中他车在本车左边的数据数 多于 在右边的数据数则返回true
        pktEventData.Collision_time = Distance / (hv_bsm_data.speed + pkt_data.payload.bsmData.speed);
        pktEventData.Distance = Distance;
        pktEventData.Deviation_angle = Deviation_angle;
        return true;
    }
    return false;

}

int LTA_Judge(HvData &hvdata, Pkt &pkt_data, PktEventData &pktEventData){
    BsmData hv_bsm_data = hvdata.getHvCurrentData();
    BsmData rv_bsm_data = pkt_data.payload.bsmData;
    //以下这个算法, 现弃用
    //a为heading 以正北为0度, x,y,a 均为已知量, 二元一次方程组
    //x1 + t1*v1*sin(a1) = x2 + t2*v2*sin(a2)
    //y1 + t1*v1*cos(a1) = y2 + t2*v2*cos(a2)
    //
    //hv_bsm_data.x + t1*hv_bsm_data.speed*sin(hv_bsm_data.heading) = rv_bsm_data.x + t2*rv_bsm_data.speed*sin(rv_bsm_data.heading)
    //hv_bsm_data.y + t1*hv_bsm_data.speed*cos(hv_bsm_data.heading) = rv_bsm_data.y + t2*rv_bsm_data.speed*cos(rv_bsm_data.heading)
//  double A1,B1,C1,A2,B2,C2;
//  A1 = hv_bsm_data.speed*sin(hv_bsm_data.heading);  B1 = rv_bsm_data.speed*sin(rv_bsm_data.heading)*-1;  C1 = hv_bsm_data.x - rv_bsm_data.x;
//  A2 = hv_bsm_data.speed*cos(hv_bsm_data.heading);  B2 = rv_bsm_data.speed*cos(rv_bsm_data.heading)*-1;  C2 = hv_bsm_data.y - rv_bsm_data.y;
//  double t1 = (A1*C2 - A2*C1) / (A2*B1 - A1*B2);
//  double t2 = (B1*C2 - B2*C1) / (A1*B2 - A2*B1);
    //两车开到交点的时间分别为t1, t2. 0<t<20 时认为应该进入预警的判断
//  if (t1>=0 && t2>=0 && t1<20 && t2<20 ) {
//      //满足条件后, 再根据车距和到达交点的时间差来判断预警等级
//      pktEventData.Warning[LTA] = warning_judge(fabs(t1 - t2), pktEventData.Distance);
//  }else{
//      pktEventData.Warning[LTA] = NO_ALARM;
//  }

    //利用碰撞时间和车辆距离判断预警等级
    pktEventData.Warning[LTA] = warning_judge(pktEventData.Collision_time, pktEventData.Distance);
    cout<<"warning_judge: "<<(int)pktEventData.Warning[LTA]<<endl;
    

    cout<<"Warning statue: ";
    for (int i=0; i<3; i++) {
        cout<<(int)pktEventData.Warning[i]<<"-";
    }cout<<endl;

    return pktEventData.Warning[LTA];
}



/*  进行预警的判断, 写一个带有预警信息和原他车信息的结构体 返回预警等级
*
*/
int Judge(HvData &hvdata, Pkt &pkt_data, PktEventData &pktEventData){
    BsmData hv_bsm_data = hvdata.getHvCurrentData();

    memset(&pktEventData, 0, sizeof(pktEventData));
    memcpy(&(pktEventData.pkt), &pkt_data, sizeof(Pkt));

    if(pkt_data.pkt_type == JAM_MSG){
        //作场景4拥堵提示的判断
        cout<<"Scene4"<<endl;
        
        double Deviation_angle; //偏差角
        double Distance;              //距离

        calculate_paras(hv_bsm_data, pkt_data.payload.jamData, Deviation_angle, Distance);
        if (Deviation_angle < JAM_WARNING_ANGLE_DIFF) {//   认为rsu在obu前方
            cout<<"RSU JAM front \n";
            if (hv_bsm_data.speed >2 ) {//本车速度不应过慢, 暂定2m/s以上
                pktEventData.Collision_time = Distance / hv_bsm_data.speed;
                pktEventData.Distance = Distance;
                pktEventData.Deviation_angle = Deviation_angle;
                pktEventData.Warning[TJW] = pkt_data.payload.jamData.jam_level;
            }
        }
            cout<<"Warning statue: ";
            for (int i=0; i<3; i++) {
                cout<<pktEventData.Warning[i]<<"-";
            }cout<<endl;

        return pktEventData.Warning[TJW];

    }
    else if (pkt_data.pkt_type == BSM_MSG) {
        //其他场景
        if (hv_bsm_data.canData.if_can_valid == true  
            &&  
            hv_bsm_data.canData.turning_light_state_e == TURN_LEFT) {//若本车can数据有效且开了左转灯, 作场景2左转辅助的判断
            cout<<"Scene2"<<endl;
            //判断他车是否在本车前侧
            double Deviation_angle; //偏差角
            double delta_angle;         //行驶方向差
            double Distance;              //距离
            calculate_paras(hv_bsm_data, pkt_data.payload.bsmData, Deviation_angle, delta_angle, Distance);
            if (Deviation_angle < FORWARD_ANGLE_DIFF   &&   Distance < 60   ) {//他车在本车前方且距离60m以内
                cout<<"RV front \n";
                if (hv_bsm_data.speed < 7) {//本车若要正常左转, 此时的的行驶速度必然不快, 考虑到可能的误差暂时设定在7m/s以下
                    if (hv_bsm_data.speed > 2) {//本车速度不应过慢 暂定2m/s 以上
                        if (delta_angle > 150) { //他车与本车相向行驶
                            cout<<"RV coming! \n";
                            pktEventData.Collision_time = Distance / (hv_bsm_data.speed + pkt_data.payload.bsmData.speed);
                            pktEventData.Distance = Distance;
                            pktEventData.Deviation_angle = Deviation_angle;
                            pktEventData.Warning[LTA] = warning_judge(pktEventData.Collision_time, pktEventData.Distance);
                            cout<<"warning_judge: "<<(int)pktEventData.Warning[LTA]<<endl;
                        }
                    }
                }
            }
            printf("Deviation_angle: %lf \tdelta_angle: %lf\tDistance: %lf\n",Deviation_angle, delta_angle, Distance);

            cout<<"Warning statue: ";
            for (int i=0; i<3; i++) {
                cout<<(int)pktEventData.Warning[i]<<"-";
            }cout<<endl;
            
            return pktEventData.Warning[LTA];
        }
        //可能左转灯读取不成功, 另一种思路来判断左转辅助场景
        else if (hv_bsm_data.canData.if_can_valid == false  &&  if_LTA(hvdata, pkt_data, pktEventData) == true) {//若本车can无效, 且满足can无效时的左转辅助判断条件
            LTA_Judge(hvdata, pkt_data, pktEventData);
            return pktEventData.Warning[LTA];
        }

        else if ( (pkt_data.payload.bsmData.canData.if_can_valid == true  &&  pkt_data.payload.bsmData.canData.brake_state_e == SWITCH_ON  &&  pkt_data.payload.bsmData.acce < BRAKE_ACCE_CAN_VALID)//他车can有效, 且他车有刹车信号, 且加速度满足一定条件
                 || (pkt_data.payload.bsmData.canData.if_can_valid == false  &&  pkt_data.payload.bsmData.acce < BRAKE_ACCE_CAN_INVALID)  ) {//他车can无效且加速度到达相对更严格的一个条件,作场景3的判断

            cout<<"Scene3"<<endl;
            
            double Deviation_angle; //偏差角
                double delta_angle;         //行驶方向差
                double Distance;              //距离
                calculate_paras(hv_bsm_data, pkt_data.payload.bsmData, Deviation_angle, delta_angle, Distance);
                if (Deviation_angle < SAME_LANE_ANGLE_DIFF) {//他车在本车前方同车道
                    if (delta_angle < SAME_DIRECTION_DIFF) {//他车与本车同向行驶
                        cout << "RV front and same lane\n";
                        if (hv_bsm_data.speed > 2) {//本车速度不应过慢, 暂定2m/s以上
                            cout << "HV is not stopped\n";

                            pktEventData.Collision_time = Distance / (hv_bsm_data.speed );
                            pktEventData.Distance = Distance;
                            pktEventData.Deviation_angle = Deviation_angle;
                            pktEventData.Warning[EBW] = warning_judge(pktEventData.Collision_time, pktEventData.Distance);
                            cout<<"warning_judge: "<<(int)pktEventData.Warning[EBW]<<endl;
                        }
                    }
                }
           
                printf("Deviation_angle: %lf \tdelta_angle: %lf\tDistance: %lf\n",Deviation_angle, delta_angle, Distance);
                cout<<"Warning statue: ";
                for (int i=0; i<3; i++) {
                    cout<<(int)pktEventData.Warning[i]<<"-";
                }cout<<endl;
            //}

            return pktEventData.Warning[EBW];
        }

        else{//没有特殊的情况,作场景1的判断
            cout<<"Scene1"<<endl;
            double Deviation_angle; //偏差角
            double delta_angle;         //行驶方向差
            double Distance;              //距离
            calculate_paras(hv_bsm_data, pkt_data.payload.bsmData, Deviation_angle, delta_angle, Distance);
            if (Deviation_angle < SAME_LANE_ANGLE_DIFF) {//他车在本车前方同车道
                if (delta_angle < SAME_DIRECTION_DIFF) {//他车与本车同向行驶
                    cout<<"RV front and same lane\n";
                    if (hv_bsm_data.speed > 2) {//本车速度不应过慢, 暂定2m/s以上
                            cout << "HV is not stopped\n";
                            if (hv_bsm_data.speed > pkt_data.payload.bsmData.speed) {//当本车速度高于他车

                            pktEventData.Collision_time = Distance / (hv_bsm_data.speed - pkt_data.payload.bsmData.speed);
                            pktEventData.Distance = Distance;
                            pktEventData.Deviation_angle = Deviation_angle;
                            pktEventData.Warning[FCW] = warning_judge(pktEventData.Collision_time, pktEventData.Distance);
                        }
                    }
                }
            }
            printf("Deviation_angle: %lf \tdelta_angle: %lf\tDistance: %lf\n",Deviation_angle, delta_angle, Distance);
            cout<<"Warning statue: ";
            for (int i=0; i<3; i++) {
                cout<<(int)pktEventData.Warning[i]<<"-";
            }cout<<endl;

            return pktEventData.Warning[FCW];
        }
       
    }
}

/*预警等级的设定: 根据碰撞时间和距离共同判断
*/
int warning_judge(double collision_time,double portrait_distance_)
{
    if (collision_time < 0 ) {
        return -1;
    }
    if (portrait_distance_ < 0 ) {
        return -1;
    } 

    int t_level = 0;
    int d_level = 0;

    if (collision_time < 5) {
        t_level = 3;
    }
    else if (collision_time < 10) {
        t_level = 2;
    }
    else if (collision_time < 15) {
        t_level = 1;
    }
    else{
        t_level = 0;
    }

    if (portrait_distance_ < 5) {
        d_level = 3;
    }
    else if (portrait_distance_ < 15) {
        d_level = 2;
    }
    else if (portrait_distance_ < 30) {
        d_level = 1;
    }
    else{
        d_level = 0;
    }

    return ( (t_level > d_level) ? t_level : d_level );
}
