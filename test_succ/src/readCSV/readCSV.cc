
#include <fstream>
#include <vector>
#include "readCSV.h"
using namespace std;


readCSV::readCSV(const char *filename){
    fid = fopen(filename,"r");
	if(NULL != fid)
	{
		for (int i = 0; i < SIZE; i++){
			//cout<<i<<endl;
			fscanf(fid, "%ld,%lf,%lf,%lf,%d,%d", &(data.no_[i]), &(data.lng[i]), &(data.lat[i]), &(data.speed[i]), &(data.brake[i]), &(data.left_light[i]));
			//cout<<i<<endl;
		}
		fclose(fid);
	}

	
}

data_read readCSV::outputdata(){
	return data;
}

/* 
//Demo 
int main(){
	char* filename = "LTA_rv.csv";
	readcardata rd(filename);

	data_read dr = rd.outputdata();
	for(int i=0; i<30; i++){
		cout<<dr.no_[i]<<'\t'
			<<dr.lng[i]<<'\t'
			<<dr.lat[i]<<'\t'
			<<dr.speed[i]<<'\t'
			<<dr.brake[i]<<'\t'
			<<dr.left_light[i]<<endl;
		sleep(1);
	}
}*/
