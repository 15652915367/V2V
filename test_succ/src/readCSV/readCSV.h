#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
using namespace std;

#define SIZE 30

typedef struct{
	long no_[SIZE+1];
	double lat[SIZE+1];
	double lng[SIZE+1];
	double speed[SIZE+1];
	long brake[SIZE+1];
	long left_light[SIZE+1];
}data_read;

class readCSV{
private:

	data_read data;

	FILE *fid = NULL;
public:
	readCSV(const char *filename);
	data_read outputdata();
};

