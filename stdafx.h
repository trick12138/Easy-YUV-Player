#pragma once
#include <vector>
#include "typeDef.h"

using namespace std;

class YUVDataPix
{
public:
	YUVDataPix(){}
	~YUVDataPix(){}
	UChar& getY() { return Y; }
	UChar& getU() { return U; }
	UChar& getV() { return V; }
private:
	UChar Y;
	UChar U;
	UChar V;
};

class Config
{
public:
	Config(int VideoWidth, int VideoHight,char* path);
	Config(int VideoWIdth, int VideoHight, short ScanWay, short PackageWay,char*& path);
	const int& getVideoWidth();
	const int& getVideoHight();
	const short& getScanWay();
	const short& getPackageWay();
	const char*& getPath();
	const int& getLCUInWidthNum();
	const int& getLCUInHightNum();
private:
	const int VideoWidth;
	const int VideoHight;
	const short ScanWay;
	const short PackageWay;
	const char* path;
	const int LCUInWidthNum;
	const int LCUInHightNum;
};

void initVideoMemory(Config& config,vector<YUVDataPix>& yuvdata);