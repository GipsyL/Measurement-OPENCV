

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <Windows.h>
using namespace cv;
using namespace std;
int y = 5;
int x = 5;
void CallBackFunc(int event, int x, int y, int flags, void* userdata);
int main(int argc, char** argv)
{
	//string path;
	string path= "X:\\Desktop\\Terme 4\\IMG_20170803_121740.jpg";
	/*if (path == NULL) {
		cin >> path;
	}*/
	namedWindow("Control", CV_WINDOW_NORMAL); //create a window called "Control"
	Mat imgOriginal;
	imgOriginal = imread(path, CV_LOAD_IMAGE_COLOR);
	namedWindow("Thresholded Image", CV_WINDOW_NORMAL);
	namedWindow("Original", CV_WINDOW_NORMAL);
	int iLowH = 0;
	int iHighH = 255;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;


	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 255); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 255);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
	
	cvCreateTrackbar("x", "Control",&x, imgOriginal.size().width -3);//Positions
	cvCreateTrackbar("y", "Control",&y, imgOriginal.size().height - 3);
	Mat imgHSV;
	while (true)
	{


		

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

																									  //morphological opening (remove small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//cout << imgThresholded.size().width<<endl;
		int countr = 0;
		for (int i = 0; i<imgThresholded.size().width; i++) {
			int countflag = 0;


			for (int t = i; t < imgThresholded.size().width; t++) {
				if ((int)imgThresholded.at<uchar>(y, t) == 255) {
					countflag++;
				}
			}
			if ((int)imgThresholded.at<uchar>(y, i) == 0) {
				countr++;
			}
			for (int c = -3; c < 4; c++) {
				imgThresholded.at<uchar>(y + c, i) = 125;
			}


		}

		float widthmm = (float)((countr * 140) / 2421.63);
		cout << "Row: " << y << " Black Pixels: " << countr << endl;
		cout << "Width: " << widthmm << " mm" << endl;
		Mat imgtmp = imread(path, CV_LOAD_IMAGE_COLOR);
		for (int i = 0; i<imgtmp.size().width; i++) {
			for (int a = 0; a < 3; a++) {
				for (int c = -3; c < 4; c++) {
					int b;
					if (a == 0) { b = 255; }
					else { b = 0; }
					imgtmp.at<Vec3b>(y +c, i)[a] = b;
				}
			}

		}

		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		int countc = 0;
		for (int j = 0; j<imgThresholded.size().height; j++) {
			int countflag = 0;

			for (int t = j; t< imgThresholded.size().height; t++) {
				if ((int)imgThresholded.at<uchar>(t, x) == 255) {
					countflag++;
				}
			}
			if ((int)imgThresholded.at<uchar>(j, x) == 0) {
				countc++;
			}
			//gray Color
			for (int c = -3; c < 4; c++) {
				imgThresholded.at<uchar>(j, x + c) = 125;
			}
		}
		float heightmm = (float)((countc * 140) / 2421.63);
		cout << "Colomn: " << x << " Black Pixels: " << countc << endl;
		cout << "Height: " << heightmm << " mm" << endl;
		//Coloring
		for (int j = 0; j<imgtmp.size().height; j++) {
			for (int a = 0; a < 3; a++) {
				for (int c = -3; c < 4; c++) {
					int b;
					if (a == 2) { b = 255; }
					else { b = 0; }
					imgtmp.at<Vec3b>(j, x +c)[a] = b;
				}
			}
		}
		imshow("Thresholded Image", imgThresholded); //show the thresholded image
		waitKey(1);
		imshow("Original", imgtmp); //show the original image
		setMouseCallback("Original", CallBackFunc, NULL);

		waitKey(0);
		Mat Loading = imread("X:\\Downloads\\Loading.jpg", CV_LOAD_IMAGE_COLOR);
		imshow("Original", Loading); //show the Loading imadge
		waitKey(1);
		imshow("Thresholded Image", Loading);
		waitKey(1);



	}

	return 0;

}
void CallBackFunc(int event, int i, int j, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		cout << "Left button of the mouse is clicked - position (" << i << ", " << j << ")" << endl;
		cout << "X:" << x << ",Y:" << y << endl;
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		cout << "Right button of the mouse is clicked - position (" << i << ", " << j << ")" << endl;
		cout << "X:" << x << ",Y:" << y << endl;
	}


	else if (event == EVENT_LBUTTONUP)
	{
		cout << "Left button of the mouse is UNclicked - position (" << i << ", " << j << ")" << endl;
		x = i;
		y = j;
		cout << "X:" << x << ",Y:" << y << endl;
		//char c = (char)13;
		
	}
}