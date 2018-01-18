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

// �㷨����
extern int areaThresMin, areaThresMax; //Ŀ�������������ֵ
extern int vmin, vmax, smin;
extern char* mode; //��ƵԴ�����ͣ�����video��IPC����ģʽ
extern char* video_dir; //��Ƶ�ļ�������videoģʽ���õ�
extern int timeThres; //Ŀ���������Ƶ��ʱ�����ֵ
extern vector<myPoint> ps;//�����������ڣ������ļ���
extern int width, height; //����Ƶ֡resize��Ŀ���


//����������Ϣ
extern string ip;
extern int port;
extern string userName;
extern string password;
extern int channel;
extern int streamType;
extern char* cameraId;

//����DLL�еĺ���
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

// ��Ƶ��ص�ȫ�ֱ���(��ʱ����Ƶ��صı�������д��Vibe.cpp��)
//Vibe
extern Mat frame; //���ÿһ֡��Ƶ��RGB��
extern Mat gray; //���ÿһ֡��Ƶ���Ҷȣ�
extern Mat mask; //���ǰ�����Ľ������ɫΪǰ������ɫΪ������
extern Mat mask_bbox;
extern VideoCapture capture;
extern int frameCount;



//MOG
extern Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor  


extern VideoWriter writer;




extern char * alarmInfo;
extern ofstream out;
#endif