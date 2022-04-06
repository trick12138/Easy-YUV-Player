#include <iostream>
#include <graphics.h>
#include <vector>
#include "stdafx.h"
#include "typeDef.h"

using namespace std;

static void YUV2RGB(unsigned char Y, unsigned char U, unsigned char V,
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
	//��ȡ��Ƶ��Ϣ
	if (argc == 1)
		return 0;
	Config config(atoi(argv[1]),atoi(argv[2]),argv[3]);
	
	//����������Ƶ���ص�����
	YUVData yuvdata;
	
	//��ʼ���ڴ�
	initVideoMemory(config, yuvdata);

	//��ӡ����Ļ
	initgraph(config.getVideoWidth(), config.getVideoHight());				//��ʼ��ͼ�ν���
	vector<YUVDataPix>::iterator YUVIter = yuvdata.getBase().begin();		//���ڴ���YUV����ĵ�����
	const int videoHight = config.getVideoHight();							//��Ƶ�ĸ�
	const int videoWidth = config.getVideoWidth();							//��Ƶ�Ŀ�
	vector<UChar> R;														//���������R��G��B����
	vector<UChar> G;
	vector<UChar> B;
	vector<UChar>::iterator RIter;											//�������RGB��ɫ�ĵ�����
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
	//���ڲ��Ե��ļ���д��
	//FILE* fp = fopen("./out.yuv", "wb");
	//��YUVת����RGB
	//for ( ; YUVIter != yuvdata.getBase().end(); ++YUVIter)
	//{
	//	R.push_back((((*YUVIter).getY() << 8) + ((*YUVIter).getV() << 8 + (*YUVIter).getV() << 5 + (*YUVIter).getV() << 2)) >> 8);
	//	G.push_back((((*YUVIter).getY() << 8) - (((*YUVIter).getU() << 6) + ((*YUVIter).getU() << 5) + (*YUVIter).getU() << 2) - ((*YUVIter).getV() << 7) + ((*YUVIter).getV() << 4) + ((*YUVIter).getV() << 2) + (*YUVIter).getV()) >> 8);
	//	B.push_back((((*YUVIter).getY() << 8) + ((*YUVIter).getU() << 9) + (*YUVIter).getU() << 3) >> 8);
	//}
	for (; YUVIter != yuvdata.getBase().end(); ++YUVIter)
	{
		YUV2RGB((*YUVIter).getY(), (*YUVIter).getU(), (*YUVIter).getV(), R, G, B);
	}

	for (RIter = R.begin(), GIter = G.begin(), BIter = B.begin(); RIter != R.end() ;delay_fps(30))
	{
		for(int i = 0 ;i < videoHight ;++i)
			for (int j = 0; j < videoWidth; ++j)
			{
				putpixel(j, i, EGERGB(*RIter, *GIter, *BIter), nullptr);
				++RIter, ++GIter, ++BIter;
			}
		delay_ms(0);
	}
	system("pause");
	return 0;
}