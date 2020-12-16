#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture capVideo;
	capVideo.open(0);
	cv::Mat frame;
	cv::Mat bgMat;
	cv::Mat subMat;
	cv::Mat bny_subMat;
	int cnt = 0;

	while (1)
	{
		capVideo >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt == 0)
		{
			//第一帧，获得背景图像
			frame.copyTo(bgMat);
			cnt = cnt + 1;
		}
		//当cnt=1时进入else cnt=0时进入if 每一帧都与前一帧做差分
		else
		{
			//第二帧开始背景差分 背景图和当前图相减
			absdiff(frame, bgMat, subMat);
			//差分结果二值化
			threshold(subMat, bny_subMat, 50, 255, CV_THRESH_BINARY);

			cnt = 0;
			imshow("差分图像二值化", bny_subMat);
			imshow("原差分图像", subMat);
			waitKey(30);
		}


	}
}