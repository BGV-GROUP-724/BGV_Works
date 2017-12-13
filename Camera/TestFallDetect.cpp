// TestConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <list>
using namespace std;
//#include "OpencvCommonLib.h"

#include "E:\\opencv3-2\\opencv\\build\\include\\opencv\cv.h"
#include "E:\\opencv3-2\\opencv\\build\\include\\opencv2\imgproc.hpp"
#include "E:\\opencv3-2\\opencv\\build\\include\\opencv2/video/background_segm.hpp"
#include "E:\\opencv3-2\\opencv\\build\\include\\opencv2\\highgui.hpp"
#include "E:\\opencv3-2\\opencv\\build\\include\\opencv2\\videoio.hpp"
#pragma comment(lib, "E:\\opencv3-2\\opencv\\build\\x86\\vc14\\lib\\opencv_core320.lib")
#pragma comment(lib, "E:\\opencv3-2\\opencv\\build\\x86\\vc14\\lib\\opencv_imgproc320.lib")
#pragma comment(lib, "E:\\opencv3-2\\opencv\\build\\x86\\vc14\\lib\\opencv_highgui320.lib")
#pragma comment(lib, "E:\\opencv3-2\\opencv\\build\\x86\\vc14\\lib\\opencv_video320.lib")
#pragma comment(lib, "E:\\opencv3-2\\opencv\\build\\x86\\vc14\\lib\\opencv_videoio320.lib")

using namespace cv;

class Setting
{
public:
	Setting()
	{
		debug = 1; //boolean
						//self.source = '‪E:\\大视景\\ch04_20171128131532_20171128141813.mp4'
						//self.source = '‪C:/Users/luhui/Fall-Detection/videos/ch04_20171128131532.mp4' # camera source
		source = "C:\\Users\\luhui\\Documents\\暴风转码\\ch04_20171128131532_20171128143207_baofeng.avi";
		bsMethod = 1;// # listed in bs.py
		MOG2learningRate = 0.001;
		MOG2shadow = 0;
		MOG2history = 100;
		MOG2thresh = 20;
		minArea = 150 * 150;// # minimum area to be considered as a person
		thresholdLimit = 50;
		dilationPixels = 30;
		useGaussian = 1;// # boolean
		useBw = 1;// # boolean
		useResize = 1;// # boolean
		gaussianPixels = 31;
		movementMaximum = 75;// # amount to move to still be the same person
		movementMinimum = 3;// # minimum amount to move to not trigger alarm
		movementTime = 50;// #50 # number of frames after the alarm is triggered
		location = "Viikintie 1";
		phone = "01010101010";
	}
	~Setting()
	{

	}
	bool debug;// = 1; //boolean
					//self.source = '‪E:\\大视景\\ch04_20171128131532_20171128141813.mp4'
					//self.source = '‪C:/Users/luhui/Fall-Detection/videos/ch04_20171128131532.mp4' # camera source
	std::string source;// = "C:\\Users\\luhui\\Documents\\暴风转码\\ch04_20171128131532_20171128143207_baofeng.avi";
	bool bsMethod;// = 1;// # listed in bs.py
	float MOG2learningRate;// = 0.001;
	bool MOG2shadow;// = 0;
	int MOG2history;// = 100;
	int MOG2thresh;// = 20;
	int minArea;// = 150 * 150;// # minimum area to be considered as a person
	int thresholdLimit;// = 50;
	int dilationPixels;// = 30;
	int useGaussian;// = 1;// # boolean
	int useBw;// = 1;// # boolean
	int useResize;// = 1;// # boolean
	int gaussianPixels;// = 31;
	int movementMaximum;// = 75;// # amount to move to still be the same person
	int movementMinimum;// = 3;// # minimum amount to move to not trigger alarm
	int movementTime;// = 50;// #50 # number of frames after the alarm is triggered
	std::string location;// = "Viikintie 1";
	std::string phone;// = "01010101010";
	
};

class Bs
{
public:
	Bs()
	{
		
	}
	~Bs()
	{

	}

	void init(Setting& _setting)
	{
		setting = _setting;
		method = _setting.bsMethod;
		if (method == 0)
		{
			fgbg = cv::createBackgroundSubtractorMOG2(setting.MOG2history, setting.MOG2thresh, setting.MOG2shadow);
			foregroundMask = Mat();
		}
		if (method == 1)
		{
			backgroundFrame = Mat();
			frameCount = 1;
		}
	}

	void updateBackground(Mat& frame)
	{
		if (method == 0)
		{
			fgbg->apply(frame, foregroundMask, setting.MOG2learningRate);
		}
		if (method == 1)
		{
			alpha = (1.0 / frameCount);
			if (backgroundFrame.empty())
			{
				backgroundFrame = frame;
			}
			cv::addWeighted(frame, alpha, backgroundFrame, 1.0 - alpha, 0, backgroundFrame);
			frameCount = frameCount + 1;
		}
	}

	Mat compareBackground(Mat& frame)
	{
		if (method == 0)
		{
			return foregroundMask;
		}
		if (method == 1)
		{
			absdiff(backgroundFrame, frame, frameDelta);
			cv::threshold(frameDelta, foregroundMask, setting.thresholdLimit, 255, cv::ThresholdTypes::THRESH_BINARY);
			return foregroundMask;
		}
		return Mat();
	}

	void deleteBackground()
	{
		if (method == 0)
		{
			foregroundMask = Mat();
		}
		if (method == 1)
		{
			foregroundMask = Mat();
		}
	}

	void resetBackgroundIfNeeded(Mat& frame)
	{
		if (method == 0)
		{
			if (foregroundMask.empty())
			{
				fgbg->apply(frame, foregroundMask);
			}
		}
		if (method == 1)
		{
			if (backgroundFrame.empty())
			{
				updateBackground(frame);
				frameCount = 1;
			}
		}
	}

	Setting setting;
	bool method;
	cv::Ptr<BackgroundSubtractorMOG2> fgbg;
	Mat backgroundFrame;
	Mat foregroundMask;
	Mat frameDelta;
	int frameCount;
	float alpha;
};


class Person
{
public:
	Person()
	{
		x = -1;
		y = -1;
		w = -1;
		h = -1;
		movementMaximum = -1;
		movementMinimum = -1;
		movementTime = -1;
		lastmoveTime = -1;
		alert = -1;
		alarmReported = -1;
		lastseenTime = -1;
		remove = -1;
		id = -1;
	}
	Person(int _x, int _y, int _w, int _h, int _movementMaximum, int _movementMinimum, int _movementTime)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
		movementMaximum = _movementMaximum;
		movementMinimum = _movementMinimum;
		movementTime = _movementTime;
	}
	~Person()
	{

	}

	void init(int _x, int _y, int _w, int _h, int _movementMaximum, int _movementMinimum, int _movementTime)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
		movementMaximum = _movementMaximum;
		movementMinimum = _movementMinimum;
		movementTime = _movementTime;
		lastmoveTime = 0;
		alert = 0;
		alarmReported = 0;
		remove = 0;
		amount += 1;
		if (amount > 1000)
		{
			amount = 0;
		}
		id = amount;
	}

	int samePerson(int _x, int _y, int _w, int _h)
	{
		int same = 0;
		if (_x + movementMaximum > x && _x - movementMaximum < x)
		{
			if (_y + movementMaximum > y && _y - movementMaximum < y)
			{
				same = 1;
			}
		}
		return same;
	}

	void editPerson(int _x, int _y, int _w, int _h)
	{
		if (abs(_x - x) > movementMinimum || abs(_y - y) > movementMinimum || abs(_w - w) > movementMinimum || abs(_h - h))
		{
			lastmoveTime = 0;
		}
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}

	int getId()
	{
		return id;
	}

	void tick()
	{
		lastmoveTime += 1;
		lastseenTime += 1;
		if (lastmoveTime > movementTime)
		{
			alert = 1;
		}
		if (lastseenTime > 4)
		{
			remove = 1;
		}
	}

	bool getAlert()
	{
		return alert;
	}

	int getRemove()
	{
		return remove;
	}

	Setting _setting;
	int x;
	int y;
	int w;
	int h;
	int movementMaximum;
	int movementMinimum;
	int movementTime;
	int lastmoveTime;
	bool alert;
	bool alarmReported;
	int lastseenTime;
	int remove;
	static int amount;
	int id;

};

int Person::amount = 0;


class Persons
{
public:
	Persons(){}
	
	~Persons(){}

	void init(int _movementMaximum, int _movementMinimum, int _movementTime)
	{
		persons.clear();
		movementMaximum = _movementMaximum;
		movementMinimum = _movementMinimum;
		movementTime = _movementTime;
		Person::amount = 0;
	}

	Person addPerson(int _x, int _y, int _w, int _h)
	{
		Person person = familiarPerson(_x, _y, _w, _h);
		if (person.id > -1)
		{
			person.editPerson(_x, _y, _w, _h);
		}
		else
		{
			person = Person(_x, _y, _w, _h, movementMaximum, movementMinimum, movementTime);
			persons.push_back(person);
		}
		return person;
	}

	Person familiarPerson(int _x, int _y, int _w, int _h)
	{
		list<Person>::iterator it = persons.begin();
		while (it != persons.end())
		{
			if (it->samePerson(_x, _y, _w, _h))
			{
				return *it;
			}
		}
		return Person();
	}

	void tick()
	{
		list<Person>::iterator it = persons.begin();
		while (it != persons.end())
		{
			it->tick();
			if (it->getRemove())
			{
				it = persons.erase(it);
			}
		}
	}
	list<Person> persons;
	int movementMaximum;
	int movementMinimum;
	int movementTime;
};

class Video
{
public:
	void _init_(Setting& _settings)
	{
		settings = _settings;
		camera = cv::VideoCapture(settings.source);
		persons.init(settings.movementMaximum, settings.movementMinimum, settings.movementTime);
		time = getTickCount();
		start = 0;
		end = 0;
		errorcount = 0;
		alertLog.clear();
		frameCount = 1;

	}

	void showFrame()
	{
		if (settings.debug)
		{
			cv::imshow("Thresh", thresh);
		}
		cv::imshow("Feed", frame);
	}

	void nextFrame()
	{
		if (camera.grab() == false)
		{
			waitKey(0);
			destroyAllWindows();
		}
		if (camera.read(frame))
		{
			convertFrame();
		}
	}

	void destroyNow()
	{
		camera.release();
		destroyAllWindows();
	}

	bool testDestroy()
	{
		int key = cv::waitKey(1) & 0xFF;
		if (key == 'q')
		{
			destroyNow();
			return 1;
		}
		return 0;
	}

	void resetBackgroundFrame()
	{
		camera.grab();
		if (camera.read(frame))
		{
			convertFrame();
			bs.resetBackgroundIfNeeded(frame);
			persons.init(settings.movementMaximum, settings.movementMinimum, settings.movementTime);
		}
	}

	void testBackgroundFrame()
	{
		int key = cv::waitKey(1) & 0xFF;
		if (key == 'n')
		{
			bs.deleteBackground();
		}
	}

	void testSetting()
	{
		int key = cv::waitKey(1) & 0xFF;
		if (key == '0')
		{
			settings.minArea += 5;
			printf("minArea: ", settings.minArea);
		}
		if (key == '9')
		{
			settings.minArea -= 5;
			printf("minArea: ", settings.minArea);
		}
		if (key == '8')
		{
			settings.dilationPixels += 1;
			printf("dilationPixels: ", settings.dilationPixels);
		}
		if (key == '7')
		{
			settings.dilationPixels -= 1;
			printf("dilationPixels: ", settings.dilationPixels);
		}
		if (key == '6')
		{
			settings.thresholdLimit += 1;
			printf("thresholdLimit: ", settings.thresholdLimit);
		}
		if (key == '5')
		{
			settings.thresholdLimit -= 1;
			printf("thresholdLimit: ", settings.thresholdLimit);
		}
		if (key == '4')
		{
			settings.movementMaximum += 1;
			printf("movementMaximum: ", settings.movementMaximum);
		}
		if (key =='3')
		{
			settings.movementMaximum -= 1;
			printf("movementMaximum: ", settings.movementMaximum);
		}
		if (key == '2')
		{
			settings.movementMinimum += 1;
			printf("movementMinimum: ", settings.movementMinimum);
		}
		if (key == '1')
		{
			settings.movementMinimum -= 1;
			printf("movementMinimum: ", settings.movementMinimum);
		}
		if (key == 'o')
		{
			if (settings.useGaussian)
			{
				settings.useGaussian = 0;
				printf("useGaussian: off");
				resetBackgroundFrame();
			}
			else
			{
				settings.useGaussian = 1;
				printf("useGaussian: on");
				resetBackgroundFrame();
			}
		}
		if (key == '+')
		{
			settings.movementTime += 1;
			printf("movementTime: ", settings.movementTime);
		}
		if (key == 'p')
		{
			settings.movementTime -= 1;
			printf("movementTime: ", settings.movementTime);
		}
	}

	void updateBackground()
	{
		bs.updateBackground(frame);
	}
	
	void convertFrame()
	{
		//resize current frame, make it gray scale and blur it
		if (settings.useResize)
		{
			float r = 750.0 / frame.size[1];
			CvSize dim = cvSize(750, (int)(frame.size[0] * r));
			cv::resize(frame, frame, dim, INTER_AREA);
		}
		if (settings.useBw)
		{
			cvtColor(frame, frame, CV_BGR2GRAY);
		}
		if (settings.useGaussian)
		{
			cv::GaussianBlur(frame, frame, cvSize(settings.gaussianPixels, settings.gaussianPixels), 0);
		}
	}
			
	void compare()
	{
		thresh = bs.compareBackground(frame);
		cv::dilate(thresh, thresh, Mat(), cv::Point(-1, -1), settings.dilationPixels);
		vector<Mat> contours;
		cv::findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		persons.tick();
		string detectStatus = "idle";
		int iszie = contours.size();
		for (int i = 0; i < iszie; i++)
		{
			if (cv::contourArea(contours.at(i)) < settings.minArea)
			{
				continue;
			}
			CvRect rect = cv::boundingRect(contours.at(i));
			Person person = persons.addPerson(rect.x, rect.y, rect.width, rect.height);
			CvScalar color = CV_RGB(0, 255, 0);
			if (person.alert)
			{
				color = CV_RGB(0, 0, 255);
				cv::line(frame, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), color, 2);
				cv::line(frame, Point(rect.x + rect.width, rect.y), Point(rect.x, rect.y + rect.height), color, 2);
				detectStatus = "Alarm, not moving";

				if (person.alarmReported == false)
				{
					person.alarmReported = 1;
				}
				cv::rectangle(frame, rect, color, 2);
			}
		}
		if (settings.debug)
		{
			end = getTickCount();
			int64 seconds = end - start;
			float fps = round(1.0 / seconds);
			start = getTickCount();
		}
	}

	void newLightconditions()
	{
		errorcount += 1;
		if (errorcount > 10)
		{
			::_sleep(1);
			resetBackgroundFrame();
			errorcount = 0;
		}
	}

	Setting settings;
	VideoCapture camera;
	Bs bs;
	Persons persons;
	int64 time;
	int64 start;
	int64 end; 
	int errorcount;
	vector<string> alertLog;
	int frameCount;
	Mat frame;
	Mat thresh;
};

int _tmain(int argc, _TCHAR* argv[])
{
	Video video;
	Setting setting;
	video._init_(setting);
	video.nextFrame();
	video.testBackgroundFrame();

	while (1)
	{
		video.nextFrame();
		video.testBackgroundFrame();
		video.updateBackground();
		video.compare();
		video.showFrame();
		video.testSetting();
		if (video.testDestroy())
		{
			cv::waitKey(9999);
			exit(1);
		}
	}
	return 0;
}

