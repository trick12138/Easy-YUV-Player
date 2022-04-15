#include <iostream>
#include <graphics.h>
#include <fstream>
#include "stdafx.h"
#include "typeDef.h"
#include "cuInfo.h"
#define IFPRINTCUINFO 1

using namespace std;

void YUV2RGB(unsigned char& Y, unsigned char& U, unsigned char& V,
	vector<UChar>& R,vector<UChar>& G,vector<UChar>& B)
{
	if ((Y + (V - 128) * 1.4075) > 255)
		R.push_back(255);
	else if ((Y + (V - 128) * 1.4075) < 0)
		R.push_back(0);
	else
		R.push_back((Y + (V - 128) * 1.4075));
	if ((Y - (U - 128) * 0.3455 - (V - 128) * 0.7169) > 255)
		G.push_back(255);
	else if ((Y - (U - 128) * 0.3455 - (V - 128) * 0.7169) < 0)
		G.push_back(0);
	else
		G.push_back(Y - (U - 128) * 0.3455 - (V - 128) * 0.7169);
	if ((Y + (U - 128) * 1.779) > 255)
		B.push_back(255);
	else if ((Y + (U - 128) * 1.779) < 0)
		B.push_back(0);
	else
		B.push_back((Y + (U - 128) * 1.779));
}

int main(int argc,char** argv)
{
	//读取视频信息
	if (argc == 1)
		return 0;
	Config config(atoi(argv[1]),atoi(argv[2]),argv[3]);
	
	//创建储存视频像素信息的向量
	vector<YUVDataPix> yuvdata;

#if IFPRINTCUINFO
	//创建储存划分CU信息的向量
	vector<pair<short, short>> leftTop;											//左上角的像素
	vector<pair<short, short>> rightDown;										//右下角的像素
	//打开文件并把CU信息读取出来
	CUBitToPos(config,leftTop, rightDown);
	//一个用于读取CU信息的迭代器
	vector<pair<short, short>>::iterator leftTopIter = leftTop.begin();
	vector<pair<short, short>>::iterator rightDownIter = rightDown.begin();
#endif // IFPRINTCUINFO
	
	//初始化内存
	initVideoMemory(config, yuvdata);

	//打印到屏幕
	vector<YUVDataPix>::iterator YUVIter = yuvdata.begin();					//用于处理YUV储存的迭代器
	const int videoHight = config.getVideoHight();							//视频的高
	const int videoWidth = config.getVideoWidth();							//视频的宽
	vector<UChar> R;														//用于输出的R，G，B分量
	vector<UChar> G;
	vector<UChar> B;
	vector<UChar>::iterator RIter;											//用于输出RGB颜色的迭代器
	vector<UChar>::iterator GIter;
	vector<UChar>::iterator BIter;
	/*
		Y = 0.298R + 0.612G + 0.117B;
		U = -0.168R - 0.330G + 0.498B + 128;
		V = 0.449R - 0.435G - 0.083B + 128;
		R = Y + 1.4075( V - 128);
		G = Y - 0.3455( U - 128) - 0.7169( V - 128);
		B = Y + 1.779( U - 128);

		R   = ((Y << 8) + ((V << 8) + (V << 5) + (V << 2))) >> 8;

		G = ((Y << 8) - ((U << 6) + (U << 5) + (U << 2)) - ((V << 7) + (V << 4) + (V << 2) + V)) >> 8;  

		B = ((Y << 8) + (U << 9) + (U << 3)) >> 8;
	*/
	//用于测试的文件块写入
	//FILE* fp = fopen("./out.yuv", "wb");
	//YUV转RGBv0.1
	for (YUVIter = yuvdata.begin();YUVIter != yuvdata.end();++YUVIter)
	{
		YUV2RGB((*YUVIter).getY(), (*YUVIter).getU(), (*YUVIter).getV(), R, G, B);
	}
	vector<YUVDataPix>().swap(yuvdata);
	while (!yuvdata.empty())
	{
		YUV2RGB(yuvdata[0].getY(),yuvdata[0].getU(),yuvdata[0].getV(), R, G, B);
		yuvdata.erase(yuvdata.begin());
		yuvdata.shrink_to_fit();
	}
	initgraph(config.getVideoWidth(), config.getVideoHight());				//初始化图形界面

	for (RIter = R.begin(), GIter = G.begin(), BIter = B.begin(); RIter != R.end() ;delay_fps(30))
	{
		delay_ms(0);
		//打印视频图像
		for(int i = 0 ;i < videoHight ;++i)
			for (int j = 0; j < videoWidth; ++j)
			{
				putpixel(j, i, EGERGB(*RIter, *GIter, *BIter), nullptr);
				++RIter, ++GIter, ++BIter;
			}
		//打印CU的划分
#if IFPRINTCUINFO
		setcolor(WHITE);
		delay_ms(0);
		while ((*leftTopIter).first != -1)
		{
			rectangle((*leftTopIter).first, (*leftTopIter).second, (*rightDownIter).first, (*rightDownIter).second);
			++leftTopIter;
			++rightDownIter;
		}
		++leftTopIter;
		++rightDownIter;
#endif // IFPRINTCUINFO

		getch();
	}
	return 0;
}