#pragma once
//获取IPC相机的画面图片,抓图
//获取IPC相机的视频，按照时间录取，指定时间点，保存成支持标准的一些格式（avi，mp4..)
//获取IPC相机的画面图片,抓图成文件
//封装成dll
//剧烈运动,tapi-example-pyrlk_optical_flow.exe





//ptrCamIP 相机IP地址
//iPort 相机端口
//ptrUsername 相机用户名
//ptrPassword 相机密码
//iChannel 相机通道号
//iType 相机的码流类型，0是主码流，1是辅码流
//ptrFileName 给定文件名
//ptrCaptureTime 指定时间，1900-01-02空格23:00:00。如果空，当前相机时间
//bUserCamSdk是否用相机的SDK，如果是0，直接用rtsp连接相机，onvif等，前期直接考虑用SDK
//iCamType相机品牌，0是海康，1是大华
//返回值，-1表示错误， 0表示正确
int cam_capture_file(char* ptrCamIP, int iPort, char* ptrUsername, char* ptrPassword, 
	int iChannel, int iType， char* ptrFileName, char* ptrCaptureTime, bool bUserCamSdk, int iCamType);
//获取IPC相机的画面图片,抓图成文件流，jpg流
//pBuffer 保存图片的buffer，调用者分配，
//iLen 调用函数者给长度，函数返回实际长度，但是如果小于，iLen返回正确的长度，函数返回值为-1
//返回值，-1表示错误， 0表示正确
int cam_capture_filestream(char* ptrCamIP, int iPort, char* ptrUsername, char* ptrPassword,
	int iChannel, int iType， char* pBuffer, int& iLen, char* ptrCaptureTime，bool bUserCamSdk, int iCamType);
//获取视频，保存文件
//ptrVideoFileName是流的文件名，根据文件后缀保存成指定的文件
int cam_save_video(char* ptrCamIP, int iPort, char* ptrUsername, char* ptrPassword,
	int iChannel, int iType， char* ptrVideoFileName， bool bUserCamSdk, int iCamType);

