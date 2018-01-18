#ifndef MODEL_H
#define MODEL_H
#include "cv.h"
#include <cxcore.h>
#include <highgui.h>
#include <imgproc.hpp>
#include <highgui.hpp>
#include <cvaux.h>
#include <opencv2/video/background_segm.hpp>
#include "data_structs.h"
#define  u_char unsigned char

void calc_fore(IplImage *current, IplImage *back, IplImage *fore, IplImage *abandon);

void update_currentback(IplImage *current, IplImage *curr_back);

void update_bufferedback(IplImage *curr_back, IplImage *buf_back, IplImage *abandon);
bool PointInRegion(int x, int y);
#endif // MODEL_H
