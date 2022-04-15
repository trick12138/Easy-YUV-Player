#include "cuInfo.h"

int* g_BitToConvert = nullptr;
int* g_BitToPos = nullptr;

using namespace std;

void CU()
{
    //处理划分CU信息
    vector<pair<int, int>> leftTop;
    vector<pair<int, int>> rightDown;
    fstream file;
    file.open("./BestDepth.txt", ios::binary | ios::in);
    const int LCUInWidthNum = 1920 / 64 + (1920 % 64 != 0);
    const int LCUInHightNum = 1200 / 64 + (1200 % 64 != 0);
    vector<char> fileBuff;
    char lastCh = 0;
    char posCh = 0;
    int posBit = 0;

    //定义一个用于转化数据的公式,将文件中的数据转化为步长
    g_BitToConvert = (int*)malloc(sizeof(int) * 4);
    g_BitToConvert[0] = 256;
    g_BitToConvert[1] = 64;
    g_BitToConvert[2] = 16;
    g_BitToConvert[3] = 4;

    //定义一个用于转化数据的公式，将文件中的数据转化为要走的坐标
    g_BitToPos = (int*)malloc(sizeof(int) * 4);
    g_BitToPos[0] = 64;
    g_BitToPos[1] = 32;
    g_BitToPos[2] = 16;
    g_BitToPos[3] = 8;

    //定义一个用于将文件中的位置转化为具体坐标的临时数组
    int* BitToFourDepthBlocks = nullptr;
    BitToFourDepthBlocks = (int*)malloc(sizeof(int) * 4);

    //先读取这一帧的数据存到数组中
    for (int i = 0; i < LCUInHightNum * LCUInWidthNum * 256 + 1; i++)
    {
        posCh = file.get();
        fileBuff.push_back(posCh);
    }
    vector<char>::iterator theBegin = fileBuff.begin();
    vector<char>::iterator theCur = fileBuff.begin();

    //读取文件并转化为要输出CU所在图像的位置
    while (theBegin != fileBuff.end())
    {
        for (int i = 0; i < LCUInHightNum; i++)
        {
            for (int j = 0; j < LCUInWidthNum; j++)
            {
                const int LCUX = j * 64;
                const int LCUY = i * 64;
                leftTop.push_back(pair<int, int>(LCUX, LCUY));
                rightDown.push_back(pair<int, int>(LCUX + g_BitToPos[0], LCUY + g_BitToPos[0]));
                //开始以LCU为一个单位进行读取
                while (theCur - theBegin < 256)
                {
                    const int lenth = theCur - theBegin;
                    int x = LCUX;
                    int y = LCUY;
                    //将文件中的位置转化为xy的起始坐标  0 1 2 3 分别对应上一个块的左上、右上、左下、右下四个块
                    BitToFourDepthBlocks[0] = lenth / 256;          //始终为0
                    BitToFourDepthBlocks[1] = lenth / 64;
                    BitToFourDepthBlocks[2] = (lenth - BitToFourDepthBlocks[1] * 64) / 16;
                    BitToFourDepthBlocks[3] = (lenth - BitToFourDepthBlocks[1] * 64 - BitToFourDepthBlocks[2] * 16) / 4;

                    //将xy坐标还原
                    for (int i = 0; i < 4; i++)
                    {
                        x += (BitToFourDepthBlocks[i] & 1) * g_BitToPos[i];
                        y += (BitToFourDepthBlocks[i] / 2) * g_BitToPos[i];
                    }

                    leftTop.push_back(pair<int, int>(x, y));
                    rightDown.push_back(pair<int, int>(x + g_BitToPos[*theCur], y + g_BitToPos[*theCur]));
                    theCur = theCur + g_BitToConvert[*theCur];
                }
                theBegin = theCur;
            }
        }
    }

    initgraph(1920, 1200);
    size_t len = leftTop.size();
    for (size_t i = 0; i < len; i++)
    {
        rectangle(leftTop[i].first, leftTop[i].second, rightDown[i].first, rightDown[i].second);
    }

    system("pause");
    return;
}

void CUBitToPos(Config& config,vector<pair<unsigned short, unsigned short>>& leftTop, vector<pair<unsigned short, unsigned short>>& rightDown)
{
    fstream file;
    file.open("./BestDepth.txt", ios::binary | ios::in);
    const int LCUInWidthNum = config.getVideoWidth() / 64 + (1920 % 64 != 0);
    const int LCUInHightNum = config.getVideoHight() / 64 + (1200 % 64 != 0);

    vector<char> fileBuff;
    char lastCh = 0;
    char posCh = 0;
    int posBit = 0;

    //定义一个用于转化数据的公式,将文件中的数据转化为步长
    g_BitToConvert = (int*)malloc(sizeof(int) * 4);
    g_BitToConvert[0] = 256;
    g_BitToConvert[1] = 64;
    g_BitToConvert[2] = 16;
    g_BitToConvert[3] = 4;

    //定义一个用于转化数据的公式，将文件中的数据转化为要走的坐标
    g_BitToPos = (int*)malloc(sizeof(int) * 4);
    g_BitToPos[0] = 64;
    g_BitToPos[1] = 32;
    g_BitToPos[2] = 16;
    g_BitToPos[3] = 8;

    //定义一个用于将文件中的位置转化为具体坐标的临时数组
    int* BitToFourDepthBlocks = nullptr;
    BitToFourDepthBlocks = (int*)malloc(sizeof(int) * 4);

    //读取所有CU划分信息到数组
    while(file)
    {
        posCh = file.get();
        fileBuff.push_back(posCh);
    }
    vector<char>::iterator theBegin = fileBuff.begin();
    vector<char>::iterator theCur = fileBuff.begin();

    //读取文件并转化为要输出CU所在图像的位置
    while (theBegin != fileBuff.end())
    {
        for (int i = 0; i < LCUInHightNum; i++)
        {
            for (int j = 0; j < LCUInWidthNum; j++)
            {
                const int LCUX = j * 64;
                const int LCUY = i * 64;
                //leftTop.push_back(pair<int, int>(LCUX, LCUY));                        这里注释掉可以避免重复记录
                //rightDown.push_back(pair<int, int>(LCUX + g_BitToPos[0], LCUY + g_BitToPos[0]));
                //开始以LCU为一个单位进行读取
                while (theCur - theBegin < 256)
                {
                    const int lenth = theCur - theBegin;
                    int x = LCUX;
                    int y = LCUY;
                    //将文件中的位置转化为xy的起始坐标  0 1 2 3 分别对应上一个块的左上、右上、左下、右下四个块
                    BitToFourDepthBlocks[0] = lenth / 256;          //始终为0
                    BitToFourDepthBlocks[1] = lenth / 64;
                    BitToFourDepthBlocks[2] = (lenth - BitToFourDepthBlocks[1] * 64) / 16;
                    BitToFourDepthBlocks[3] = (lenth - BitToFourDepthBlocks[1] * 64 - BitToFourDepthBlocks[2] * 16) / 4;

                    //将xy坐标还原
                    for (int i = 0; i < 4; i++)
                    {
                        x += (BitToFourDepthBlocks[i] & 1) * g_BitToPos[i];
                        y += (BitToFourDepthBlocks[i] / 2) * g_BitToPos[i];
                    }

                    leftTop.push_back(pair<int, int>(x, y));
                    rightDown.push_back(pair<int, int>(x + g_BitToPos[*theCur], y + g_BitToPos[*theCur]));
                    if (*theCur == -1)
                    {
                        leftTop.push_back(pair<int, int>(-1, -1));
                        rightDown.push_back(pair<int, int>(-1, -1));
                        return;
                    }
                    theCur = theCur + g_BitToConvert[*theCur];
                }
                theBegin = theCur;
            }
        }
        leftTop.push_back(pair<int, int>(-1, -1));
        rightDown.push_back(pair<int, int>(-1, -1));
    }
}