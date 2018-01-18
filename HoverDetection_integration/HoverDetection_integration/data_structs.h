#include "iostream"
using namespace std;
typedef struct node1{
	char* camera_id;
	char* points;
	int state;
} AlarmInfo;

typedef struct node2{
	string address;
	int port;
	string username;
	string passwd;
	string dbname;
	string tablename;
} DBInfo;


typedef struct node3{
	char* cameraIp;
	int port;
	char* name;
	char* passwd;
	int channel;
	int streamType;
}CameraInfo;

typedef struct node4{
	char* cameraId;
	int height;
	int width;
	char* vpt;
	int vptThres;
	int VptOptionValid;
	int type;
	char* areaId;
	struct node4* next;
	//摄像机相关的
	char* cameraIp;
	int port;
	char* name;
	char* passwd;
	int channel;
	int streamType;
}Config;

typedef struct node{
	float x;
	float y;
}myPoint;