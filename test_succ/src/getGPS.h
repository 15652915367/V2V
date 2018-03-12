#ifndef _GETGPS_H
#define _GETGPS_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;

/*	call this method from outside for test.    --usage
* 	
*/
int getGPS_run();


/*	split the string into several parts by [c]    --usage
*	s: target string;  v: parts splited;  c: use it to split string;  	--parameter
*/
void SplitString(const string& s, vector<string>& v, const string& c);

/*	get GPS data info(lng/lat) into parameter    --usage
* 	
*	will return 0 if SUCC.   return -1 if FAILED  --return
*/
int getGPS(double &lng, double &lat);

#endif
