#include <iostream>
#include <fstream>
#include "stdafx.h"

using namespace std;

YUVDataPix::YUVDataPix(){}
YUVDataPix::~YUVDataPix(){}
UChar& YUVDataPix::getY() { return Y; }
UChar& YUVDataPix::getU() { return U; }
UChar& YUVDataPix::getV() { return V; }

YUVData::YUVData(){}
YUVData::~YUVData(){}

vector<YUVDataPix>& YUVData::getBase()
{
	return YUVPix;
}

Config::Config(int VideoWidth, int VideoHight,char* Path) :VideoWidth(VideoWidth) ,VideoHight(VideoHight) ,ScanWay(S4_2_0) ,PackageWay(PLANER) ,path(Path) {}
Config::Config(int VideoWidth, int VideoHight, short ScanWay, short PackageWay, char*& Path) :VideoWidth(VideoWidth), VideoHight(VideoHight), ScanWay(ScanWay), PackageWay(PackageWay), path(Path){}

const int& Config::getVideoWidth() { return VideoWidth; }
const int& Config::getVideoHight() { return VideoHight; }
const short& Config::getScanWay() { return ScanWay; }
const short& Config::getPackageWay() { return PackageWay; }
const char*& Config::getPath() { return path; }

void initVideoMemory(Config& config, YUVData& yuvdata)
{
	UChar Y = 0;
	UChar U = -127;
	UChar V = -127;
	FILE *fp = fopen(config.getPath(), "rb");
	vector<UChar> posDY;
	vector<UChar> posDU;
	vector<UChar> posDV;
	
	const int Hight = config.getVideoHight();
	const int Width = config.getVideoWidth();
	
	if (fp == NULL)
	{
		cout << "文件打开失败" << endl;
		return;
	}

	//读取文件
	while (!feof(fp))
	{
		//获取Y分量
		for (int i = 0; i < Hight; i++)
			for (int j = 0; j < Width; j++)
			{
				fread(&Y, sizeof(UChar), 1, fp);
				posDY.push_back((UChar)Y);
			}

		//获取U分量
		for (int i = 0; i < Hight / 2; i++)
		{
			for (int j = 0; j < Width / 2; j++)
			{
				fread(&U, sizeof(UChar), 1, fp);
				posDU.push_back((UChar)U);
				posDU.push_back((UChar)U);
			}
			posDU.insert(posDU.end(), posDU.end() - Width, posDU.end());
		}

		//获取V分量
		for (int i = 0; i < Hight / 2; i++)
		{
			for (int j = 0; j < Width / 2; j++)
			{
				fread(&V, sizeof(UChar), 1, fp);
				posDV.push_back((UChar)V);
				posDV.push_back((UChar)V);
			}
			posDV.insert(posDV.end(), posDV.end() - Width, posDV.end());
		}
	}

	//写入
	vector<UChar>::iterator YIter = posDY.begin();
	vector<UChar>::iterator UIter = posDU.begin();
	vector<UChar>::iterator VIter = posDV.begin();
	YUVDataPix Pix;
	while (YIter != posDY.end() && UIter != posDU.end() && VIter != posDV.end())
	{
		Pix.getY() = *YIter;
		Pix.getU() = *UIter;
		Pix.getV() = *VIter;
		yuvdata.getBase().push_back(Pix);
		++YIter;
		++UIter;
		++VIter;
	}
}