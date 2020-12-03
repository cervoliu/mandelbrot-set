#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cv.h>
#include <highgui.h>

int mSetTest(double c_re, double c_im)
{
	double re = 0, im = 0, re_new, im_new;
	for(int i = 1; i <= 200; ++i)//i-th round of iterations
	{
		re_new = re * re - im * im + c_re;
		im_new = 2 * re * im + c_im;
		re = re_new, im = im_new;
		if(re * re + im * im > 4) return i;
	}
	return 0;
}

const int IMAGE_SIZE = 800;
double delta = 4.0 / IMAGE_SIZE, SX = -2.0, SY = -2.0, sx= -2.0, sy = -2.0;
IplImage *pImg;
void paint()
{
	double x = sx;
	for(int i = 0; i < IMAGE_SIZE; ++ i, x += delta)
	{
		double y = sy;
		for(int j = 0; j < IMAGE_SIZE; ++ j, y += delta)
		{
			CvScalar sca;
			if(mSetTest(x, y) == 0)
			{
				sca.val[0] = 125;
				sca.val[1] = 25;
				sca.val[2] = 5;
				cvSet2D(pImg, j, i, sca);
			}
			else
			{
				sca.val[0] = 200;
				sca.val[1] = 200;
				sca.val[2] = 200;
				cvSet2D(pImg, j, i, sca); 
			}
		}
	}
	cvShowImage("mandelbrot", pImg);
}

int _event = -1, _x = -1, _y = -1, _flags = -1;
void on_mouse(int event, int x, int y, int flags, void *param)
{
	//printf("%d %d %d %d -- %.4lf %.4lf %.4lf %.4lf %.8lf\n",event,x,y,flags,SX,SY,sx,sy,delta);
	if(event == CV_EVENT_LBUTTONUP)
	{
		if(_event == CV_EVENT_LBUTTONDOWN)
		{
			sx = SX - delta * (x - IMAGE_SIZE / 2), sy = SY - delta * (y - IMAGE_SIZE / 2);
			paint();
		}
		else
		if(_event == CV_EVENT_MOUSEMOVE && _flags == CV_EVENT_FLAG_LBUTTON)
		{
			if(_x > x) std :: swap(_x, x);
			if(_y > y) std :: swap(_y, y);
			int t = std :: max(x - _x, y - _y);
			SX = sx + delta * _x, SY = sy + delta * _y;
			sx = SX, sy = SY;
			delta = delta * t / IMAGE_SIZE;
			paint();
		}
	}
	else
	if(event == CV_EVENT_LBUTTONDOWN) _x = x, _y = y;
	_event = event,  _flags = flags;
}
int main()
{
	pImg = cvCreateImage(/*size*/cvSize(IMAGE_SIZE, IMAGE_SIZE), /*depth*/ 8, /*nChannels*/3);
	cvNamedWindow(/*name of the window*/"mandelbrot", 1);
	paint();
	cvSetMouseCallback("mandelbrot",on_mouse,0);
	cvWaitKey(/*delay*/0);
	cvDestroyWindow("mandelbrot");
	//cvRelease(&pImg);
	return 0;
}

