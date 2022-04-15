#include <iostream>
#include <fstream>
#include "stdafx.h"

using namespace std;

Config::Config(int VideoWidth, int VideoHight,char* Path)
	:VideoWidth(VideoWidth) ,VideoHight(VideoHight) ,ScanWay(S4_2_0) ,PackageWay(PLANER) ,path(Path),LCUInWidthNum(0),LCUInHightNum(0) {}
Config::Config(int VideoWidth, int VideoHight, short ScanWay, short PackageWay, char*& Path)
	:VideoWidth(VideoWidth), VideoHight(VideoHight), ScanWay(ScanWay), PackageWay(PackageWay), path(Path),LCUInWidthNum(0),LCUInHightNum(0) {}

const int& Config::getVideoWidth() { return VideoWidth; }
const int& Config::getVideoHight() { return VideoHight; }
const short& Config::getScanWay() { return ScanWay; }
const short& Config::getPackageWay() { return PackageWay; }
const char*& Config::getPath() { return path; }

const int& Config::getLCUInWidthNum()
{
	return LCUInWidthNum;
}

const int& Config::getLCUInHightNum()
{
	return LCUInHightNum;
}

void initVideoMemory(Config& config, vector<YUVDataPix>& yuvdata)
{
	UChar Y = 0;
	UChar U = -127;
	UChar V = -127;
	FILE *fp = fopen(config.getPath(), "rb");
	vector<UChar> posDY;
	vector<UChar> posDU;
	vector<UChar> posDV;
	
	YUVDataPix Pix;
	
	const int Hight = config.getVideoHight();
	const int Width = config.getVideoWidth();
	
	if (fp == NULL)
	{
		cout << "文件打开失败" << endl;
		return;
	}

	//读取文件
	vector<UChar>::iterator YIter = posDY.begin();
	vector<UChar>::iterator UIter = posDU.begin();
	vector<UChar>::iterator VIter = posDV.begin();
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

		//写入
		YIter = posDY.begin();
		UIter = posDU.begin();
		VIter = posDV.begin();
		while (YIter != posDY.end() && UIter != posDU.end() && VIter != posDV.end())
		{
			Pix.getY() = *YIter;
			Pix.getU() = *UIter;
			Pix.getV() = *VIter;
			yuvdata.push_back(Pix);
			++YIter;
			++UIter;
			++VIter;
		}
		//清空每一帧的暂存信息
		vector<UChar>().swap(posDY);
		vector<UChar>().swap(posDU);
		vector<UChar>().swap(posDV);
	}

	fclose(fp);
}