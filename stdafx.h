#pragma once
#include <vector>
#include "typeDef.h"

using namespace std;

class YUVDataPix
{
public:
	YUVDataPix();
	~YUVDataPix();
	UChar& getY();
	UChar& getU();
	UChar& getV();
private:
	UChar Y;
	UChar U;
	UChar V;
};

class YUVData
{
public:
	YUVData();
	~YUVData();
	vector<YUVDataPix>& getBase();
private:
	vector<YUVDataPix> YUVPix;
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
private:
	const int VideoWidth;
	const int VideoHight;
	const short ScanWay;
	const short PackageWay;
	const char* path;
};

void initVideoMemory(Config& config,YUVData& yuvdata);