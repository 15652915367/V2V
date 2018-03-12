#include "getGPS.h"
using namespace std;

/*	split the string into several parts by [c]    --usage
*	s: target string;  v: parts splited;  c:   --parameter
*/
void SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

/*	get GPS data info(lng/lat) into parameter.  "is>>str" BLOCK IO    --usage
* 	
*	will return 0 if SUCC.   return -1 if FAILED  --return
*/
int getGPS(double &lng, double &lat){
	ifstream is("/dev/ttyS1");
	if(is)		cout<<"open succ\n";
	else	   {
        cout<<"open  /dev/ttyS1  succ\n";
        return -1;
    }
	string str;
	while(is>>str){
		vector<string> v;
		SplitString(str, v,","); //可按多个字符来分隔;
		if(v[0] == "$GNRMC"){
			for(vector<string>::size_type i = 0; i != v.size(); ++i)
		    	cout << v[i] << " ";
			//cout << "lat: " <<v[3] << "\nlng: " << v[5] << endl;
			cout<<endl;
			lat = atof(v[3].c_str())/100.0;
			lng = atof(v[5].c_str())/100.0;
			break;
		}
		if(v[0] == "$GNGGA"){
			for(vector<string>::size_type i = 0; i != v.size(); ++i)
		    	cout << v[i] << " ";
			//cout << "lat: " <<v[3] << "\nlng: " << v[5] << endl;
			cout<<endl;
			lat = atof(v[2].c_str())/100.0;
			lng = atof(v[4].c_str())/100.0;
			break;
		}
	}
return 0;
}


int getGPS_run(){
	double lng,lat=0;
	while(1){
		getGPS(lng,lat);
		printf("lng:%lf   lat:%lf\n",lng,lat);
		usleep(10000);
	}

return 0;
}
