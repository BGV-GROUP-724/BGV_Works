#include "myHeaders.h"
// ��Ƶ��ص�ȫ�ֱ���
Mat frame; //���ÿһ֡��Ƶ��RGB��
Mat gray; //���ÿһ֡��Ƶ���Ҷȣ�
Mat mask; //���ǰ�����Ľ������ɫΪǰ������ɫΪ������
Mat mask_bbox; //��mask�Ļ����ϻ���ǰ�������bounding box
vector <Rect> targets; //��ǰ������mask��������Ƶ����ȡ���˶�Ŀ�꣬��ÿ��Ŀ���bounding box���
VideoCapture capture;
int frameCount = 0;
ViBe_BGS Vibe_Bgs; //ViBe �㷨�ı���

/*
 * ���ܣ���ȡ��Ƶ֡������ȡ����Ƶ֡����frame��gray����ȫ�ֱ�����
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
		if (!capture.read(frame_tmp)){//frame,gray�������洢��Ƶ֡��ȫ�ֱ���
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
 * ���ܣ���ʼ��Vibe�����еı���ģ��
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
 * ���ܣ����±���ģ�壬�������룬������ȫ�ֱ���mask��
 */
bool Vibe_apply(){
	if (!GetFrame())
		return false;
	Vibe_Bgs.testAndUpdate(gray);
	mask = Vibe_Bgs.getMask();
	morphologyEx(mask, mask, MORPH_OPEN, Mat());//��Ҫ�����������̬ѧ����
	return true;
}

bool ExtractTargetFromMask(){
	targets.clear();//����б��е���������

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
 *����:��mask�Ļ����ϣ�����ÿ��ǰ��Ŀ���bounding box
 */
bool MarkTarget(){
	mask.copyTo(mask_bbox);
	for (vector<Rect>::iterator iter = targets.begin(); iter != targets.end(); iter++){
		rectangle(mask_bbox, *iter, Scalar(255), 3);
	}
	return true;
}