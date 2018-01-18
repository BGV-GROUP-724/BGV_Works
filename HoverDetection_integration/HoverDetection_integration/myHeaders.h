#ifndef MYHEADERS_H
#define MYHEADERS_H
#include <cxcore.h>
#include <highgui.h>
#include <imgproc.hpp>
#include <highgui.hpp>
#include <cvaux.h>
#include <opencv2/video/background_segm.hpp>
#include "data_structs.h"
#include "windows.h"
#include "iostream"
#include "fstream"
#include "sstream"
#include "tinystr.h"
#include "tinyxml.h"
#include "tchar.h"
#include "video/tracking.hpp"
#include "time.h"
#include "Vibe.h"

using namespace std;
using namespace cv;

bool GetParams();
bool GetConfig();
bool RunAlgorithm();
char* GetLogFileName();
char* GetCurrentDir();
char* replace(const char* str, const char* a, const char* b);
vector<char*> split(char * str, char* delimiter);
int CalcPointsDistance(Point start, Point end);

void HoverDetection();
bool GetFrame();
bool Vibe_init();
bool Vibe_apply();
bool ExtractTargetFromMask();
bool MarkTarget();

// 算法参数
extern int areaThresMin, areaThresMax; //目标区域面积的阈值
extern int vmin, vmax, smin;
extern char* mode; //视频源的类型，包括video、IPC两种模式
extern char* video_dir; //视频文件名，在video模式中用到
extern int timeThres; //目标出现在视频的时间的阈值
extern vector<myPoint> ps;//不规则区域内，坐标点的集合
extern int width, height; //将视频帧resize后的宽、高


//摄像机相关信息
extern string ip;
extern int port;
extern string userName;
extern string password;
extern int channel;
extern int streamType;
extern char* cameraId;

//加载DLL中的函数
typedef Config*(*GetConfigFromDB)(string thisExeName, DBInfo db1, DBInfo db2);
typedef bool(*GetCameraInfo)(string&, string&, string&);
typedef bool(*InitESB)();
extern InitESB InitESBFunc;
typedef bool(*SendMsg2ESB)(int type, LPCTSTR strMsg);
extern SendMsg2ESB SendMsg2ESBFunc;
typedef bool(*DestorySEB)();
typedef int(*AlarmInfo2DB_2)(char* info, DBInfo db);
extern HINSTANCE dbHandle;
extern AlarmInfo2DB_2 AlarmInfo2DB_2Func;

// 视频相关的全局变量(暂时把视频相关的变量定义写在Vibe.cpp里)
//Vibe
extern Mat frame; //存放每一帧视频（RGB）
extern Mat gray; //存放每一帧视频（灰度）
extern Mat mask; //存放前景检测的结果（白色为前景、黑色为背景）
extern Mat mask_bbox;
extern VideoCapture capture;
extern int frameCount;



//MOG
extern Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor  


extern VideoWriter writer;




extern char * alarmInfo;
extern ofstream out;
#endif