#ifndef     _GPS_H
#define     _GPS_H

#include <iostream>

#include <math.h>
#define Pi 3.1415926535897932384626433832795028841971693993751
//package com.example.uitest;
//#define Pi 3.1415926535897932384626433832795028841971693993751

/*   transform GPS into Cartesian. Fisrt step "GpsTransformer gps", secondly "setLatitdue/setLongitude", thirdly "getX/getY"      --usage
*
*/
class GpsTransformer{
	//??????????54????54???80????80?WGS84???

	
	//public static boolean confirmFlag = false;
	

	private :
                double longitude;
        	double latitude;
        	double x;//x_location after transfer ??????
        	double y;//y_location after transfer
        	
        	double speed;//???????????m??
        	double angle;
            	double gpsTime;
//                string carId;
                static  const int WGS84 = 84;
                static const  int BJ54 = 54;
                static const int XIAN80 = 80;
	public :
            GpsTransformer(){}
            GpsTransformer(double longitude, double latitude, double speed, double angle) {  
            this->longitude = longitude;  
	    this->latitude = latitude;  
	    this->speed = speed;  
	    this->angle = angle;  
            }   
            /*  Just use this method.  Change "longitude/latitude" into "x/y".     --usage
            *    -"longitude/latitude" won't change in this method. "x/y" will change.     --para
            */
            void transformer(double &longitude, double &latitude, double &x, double &y){
                setLongitude(longitude);
                setLatitude(latitude);
                geodeticToCartesian();
                x=getX();
                y=getY();
            }

           // static bool confirmFlag;
//          string getCarId() {
//  	return carId;
//          }
//          void setCarId(string carId) {
//  	this->carId = carId;
//          }
            void setLongitude(double longitude) {  
		this->longitude = longitude;  
            }
            double getLongitude() {  
		return longitude;  
            }  
    
            void setLatitude(double latitude) {  
		this->latitude = latitude;  
            }
            double getLatitude() {  
	    return latitude;  
            }
	
            void setSpeed(double speed) {  
	    this->speed = speed;  
            }
            double getSpeed() {  
	    return speed;  
            }
	
            void setAngle(double angle){
		this->angle = angle;
            }
            double getAngle(){
		return angle;
            }
	
            double getX() {  
		return x;  
            } 
	
            double getY() {  
		return y;  
            } 
	
	//transfer angle to radian
            double angleToRadian(double angle){
		double radian = angle * Pi / 180;
		return radian;
            }
	//?????????0-360?
            double stdAngle(double angle){
		double stdAngle = 90 - angle;
		if (stdAngle < 0){
			stdAngle = stdAngle + 360;
		}
                return stdAngle;
            }

/*	#define BJ54 54//??54????
	#define WGS84 84//WGS84????
	#define XIAN80 80//??80????
*/
	//????????????
            void geodeticToCartesian ()
            {
        		double L,B;//???????
        		double l,L0;//????????????????????
        		double X,Y;//????
        		double f,e2 = 0,a = 0,b,e12 = 0;////???????,?????????,?????????
        		double t;//??tan B
        		double m;//??lcosB
        		double q2;//?????? ???
        		double M,N;//?????????
        		double S;//?????
        		double A0,B0,C0,D0,E0;
        		double n;//????
        		double a_1_e2;//a*(1-e*e)??
        		int zonewide=3;//3?????????6??

        		int base_type=84;    //??????????54????54???80????80?WGS84???
        		const double PI = Pi;
        		//----------???????-----------
        		L = this->longitude;//??
        		B = this->latitude;//??
        		if(WGS84 == base_type)
        		{
        			a = 6378137;//??????
        			b = 6356752.3142;
        			f = 1/298.257223563;
        			e2 = 0.0066943799013;//--?????
        			e12 = 0.00673949674227;//--?????
        		}
        		else if(BJ54 == base_type)
        		{
        			a = 6378245;
        			b = 6356863.0187730473;
        			f = 1/298.3;
        			e2 = 0.006693421622966;//--?????
        			e12 = 0.006738525414683;//--?????
        		}
        		else if(XIAN80 == base_type)
        		{
        			a = 6378140;
        			b = 6356755.2881575287;
        			f = 1/298.257;
        			e2 = 0.006694384999588;//--?????
        			e12 = 0.006739501819473;//--?????
        		}
        		//---?????? a ????b ???--------
        		//f = (a-b)/a;//--?????
        		//e2 = (a*a-b*b)/(a*a);//--?????
        		//e12 = (a*a-b*b)/(b*b);//--?????
        		a_1_e2 = a*(1-e2);
        		//-----??????
        		if (zonewide==6) 
        		{
        			n=(int)(L/6)+1;
        			L0=n*zonewide-3;
        		}
        		else
        		{
        			n=(int)((L-1.5)/3)+1;
        			L0=n*3;
        		}

        		//---?????,sin,cos ????????????
        		L0=L0*Pi/180;
        		L=L*Pi/180;
        		B=B*Pi/180;
        		l = L - L0;
        		t = tan(B);
        		q2 = e12*(cos(B)*cos(B));
        		N = a/sqrt(1-e2*sin(B)*sin(B));////????????
        		m = l*cos(B);
        		//---?????
        		//--????????????????????????????
        		double m0 = a_1_e2;
        		double m2 = 3.0 * e2 * m0 / 2.0;
        		double m4 = 5.0 * e2 * m2 / 4.0;
        		double m6 = 7.0 * e2 * m4 / 6.0;
        		double m8 = 9.0 * e2 * m6 / 8.0;
        		double a0 = m0 + m2 / 2.0 + 3.0 * m4 / 8.0 + 5.0 * m6 / 16.0 + 35.0 * m8 / 128;
        		double a2 = m2 / 2.0 + m4 / 2.0 + 15.0 * m6 / 32.0 + 7.0 * m8 / 16.0;
        		double a4 = m4 / 8.0 + 3.0 * m6 / 16.0 + 7.0 * m8 / 32.0;
        		double a6 = m6 / 32.0 + m8 / 16.0;
        		double a8 = m8 / 128.0;
        		A0 = a0;
        		B0 = a2;
        		C0 = a4;
        		D0 = a6;
        		E0 = a8;
        		S = (A0 * B - B0* sin(2.0*B)/2.0 + C0 * sin(4.0*B)/4.0 - D0 *sin(6.0*B)/6.0 + E0*sin(8.0*B)/8.0);

        		//----??????-------
        		X = S + N * t *((0.5 + ((5.0 - t * t + 9 * q2 +4 * q2 * q2)/24 + (61.0 - 58.0 * t * t + pow(t,4)) * m * m /720.0) * m * m) * m * m);//pow(t,4)?t?4??
        		Y = N * ((1 + ((1 - t * t +q2)/6.0 +(5.0 - 18.0 * t * t + pow(t,4) + 14 * q2 - 58 * q2 * t * t ) * m * m / 120.0) * m * m ) * m);
        		//----????????????500???????
        		Y = 1000000 * n + 500000 + Y;
        		this->x = Y;//???????Y?????????Y????????Y??x;
        		this->y = X;/*?????*/
        		//TRACE("x=%d , y=%d",pcc->x,pcc->y);
            }

            double getGpsTime() {
		return gpsTime;
            }

            void setGpsTime(double gpsTime) {
		this->gpsTime = gpsTime;
            }

};

//bool Gps::confirmFlag=false;
//因为高斯平面坐标系的xy轴与一般的xy轴相反，因此需要调整xy，以及角度。

#endif      /*_GPS_H*/
