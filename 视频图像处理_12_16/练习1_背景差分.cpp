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
			//��һ֡����ñ���ͼ��
			frame.copyTo(bgMat);
			cnt = cnt + 1;
		}
		//��cnt=1ʱ����else cnt=0ʱ����if ÿһ֡����ǰһ֡�����
		else
		{
			//�ڶ�֡��ʼ������� ����ͼ�͵�ǰͼ���
			absdiff(frame, bgMat, subMat);
			//��ֽ����ֵ��
			threshold(subMat, bny_subMat, 50, 255, CV_THRESH_BINARY);

			cnt = 0;
			imshow("���ͼ���ֵ��", bny_subMat);
			imshow("ԭ���ͼ��", subMat);
			waitKey(30);
		}


	}
}