
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>

IplImage* image = 0;
IplImage* open = 0;
IplImage* close = 0;
IplImage* gradient = 0;
IplImage* tophat = 0;
IplImage* blackhat = 0;

int radius = 0;
int radius_max = 10;
int iter = 0;
int iter_max = 10;

void trackbar_rad(int pos)
{
	radius = pos;
}

void trackbar_iter(int pos)
{
	iter = pos;
}

int main(int argc, char* argv[])
{
	char* filename = argc == 2 ? argv[1] : "Image.jpg";
	image = CvLoadImage(filename, 1);
	open = CvCloneImage(image);
	close = CvCloneImage(image);
	gradient = CvCloneImage(image);
	tophat = CvCloneImage(image);
	blackhat = CvCloneImage(image);
	assert(image != 0);

	cvNamedWindow("ORIGIN", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("OPEN", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("CLOSE", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("GRADIENT", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("TOPHAT", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("BLACKHAT", CV_WINDOW_AUTOSIZE);

	cvCreateTrackbar("Radius", "ORIGIN", &radius, radius_max, trackbar_rad);
	cvCreateTrackbar("Iteration", "ORIGIN", &iter, iter_max, trackbar_iter);

	while (true)
	{
		cvShowImage("ORIGIN", image);

		IplConvKernel* kern = cvCreateStructuringElementEx(radius*2 + 1, radius*2 + 1, radius, radius, CV_SHAPE_ELLIPSE);
		IplImage* temp = 0;
		temp = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);

		cvMorphologyEx(image, open, temp, kern, CV_MOP_OPEN, iter);
		cvMorphologyEx(image, close, temp, kern, CV_MOP_CLOSE, iter);
		cvMorphologyEx(image, gradient, temp, kern, CV_MOP_GRADIENT, iter);
		cvMorphologyEx(image, tophat, temp, kern, CV_MOP_TOPHAT, iter);
		cvMorphologyEx(image, blackhat, temp, kern, CV_MOP_BLACKHAT, iter);
		cvShowImage("OPEN", open);
		cvShowImage("CLOSE", close);
		cvShowImage("GRADIENT", gradient);
		cvShowImage("TOPHAT", tophat);
		cvShowImage("BLACKHAT", blackhat);

		cvReleaseStructuringElement(&kern);
		cvReleaseImage(&temp);

		char x = cvWaitKey(33);
		if (x == 27) break;
	}

	cvReleaseImage(&image);
	cvReleaseImage(&open);
	cvReleaseImage(&close);
	cvReleaseImage(&gradient);
	cvReleaseImage(&tophat);
	cvReleaseImage(&blackhat);
	cvDestroyAllWindows();
	return 0;
}