#include "myHeaders.h"
// 视频相关的全局变量
Mat frame; //存放每一帧视频（RGB）
Mat gray; //存放每一帧视频（灰度）
Mat mask; //存放前景检测的结果（白色为前景、黑色为背景）
Mat mask_bbox; //在mask的基础上画出前景对象的bounding box
vector <Rect> targets; //从前景（即mask变量）视频中提取的运动目标，由每个目标的bounding box组成
VideoCapture capture;
int frameCount = 0;
ViBe_BGS Vibe_Bgs; //ViBe 算法的变量

/*
 * 功能：获取视频帧，将获取的视频帧存入frame、gray两个全局变量中
 */
bool GetFrame(){
	if (strcmp(mode, "video") == 0){
		if (!capture.isOpened()){
			capture.open(video_dir);
			if (!capture.isOpened()){
				cout << "Open " << video_dir << " failed." << endl;
				out << "Open " << video_dir << " failed." << endl;
				return false;
			}
		}
		Mat frame_tmp;
		if (!capture.read(frame_tmp)){//frame,gray是用来存储视频帧的全局变量
			cout << "Video has completed playing" << endl;
			out << "Video has completed playing" << endl;
			return false;
		}
		resize(frame_tmp, frame, Size(width, height));
		cvtColor(frame, gray, CV_RGBA2GRAY);
		frameCount++;
		return true;	
	}
	return false;
}

/*
 * 功能：初始化Vibe变量中的背景模板
 */
bool Vibe_init(){
	if (!GetFrame())
		return false;
	Vibe_Bgs.init(gray);
	Vibe_Bgs.processFirstFrame(gray);
	cout << "Training Vibe complete!" << endl;
	out << "Training Vibe complete!" << endl;
	return true;
}
/*
 * 功能：更新背景模板，生成掩码，保存在全局变量mask中
 */
bool Vibe_apply(){
	if (!GetFrame())
		return false;
	Vibe_Bgs.testAndUpdate(gray);
	mask = Vibe_Bgs.getMask();
	morphologyEx(mask, mask, MORPH_OPEN, Mat());//需要对掩码进行形态学处理
	return true;
}

bool ExtractTargetFromMask(){
	targets.clear();//清空列表中的所有属性

	vector<vector<Point>> contours0, contours;
	vector<Vec4i> hierarchy;
	findContours(mask, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	contours.resize(contours0.size());
	for (size_t k = 0; k < contours0.size(); k++)
		approxPolyDP(Mat(contours0[k]), contours[k], 3, true);
	Mat frame_contours(frame.rows, frame.cols, CV_8UC3);
	Mat frame_bbox(frame.rows, frame.cols, CV_8UC3);
	//cout << "****Frame" << count << "****" << endl;
	for (int idx = 0; idx < contours.size(); idx++){
		Rect bbox = boundingRect(contours[idx]);
		targets.push_back(bbox);
	}
	return true;
}
/*
 *功能:在mask的基础上，画出每个前景目标的bounding box
 */
bool MarkTarget(){
	mask.copyTo(mask_bbox);
	for (vector<Rect>::iterator iter = targets.begin(); iter != targets.end(); iter++){
		rectangle(mask_bbox, *iter, Scalar(255), 3);
	}
	return true;
}