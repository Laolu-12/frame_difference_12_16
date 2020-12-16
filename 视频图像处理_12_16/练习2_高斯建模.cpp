#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdlib.h>
using namespace cv;
using namespace std;

//ʵ�ָ�˹��ģ��������� 
//˼·���ȶ�ȡ��Ƶͼ���ǰ��֡��ĳ��λ�����ص㣬�����MeanMat,�ٵ��ú�����������ص�ľ�ֵ�ͷ��ͬ���Ĳ�������ÿ�����ء�֮���ٶ�ͼ���������бȽ�
//�����ҵĴ����ÿ�����ض�����һ֡ͼ�������ķѵ�ʱ��ܶ࣬���԰���Ƶͼ��Ĵ�С��Ϊԭ����10��������Ȼ����Ҫ���кü����� �����������
int main()
{
	VideoCapture cap;
	cap.open(0);
	cv::Mat frame = Mat(Size(640,480),CV_8UC3, Scalar(0));
	cv::Mat MeanMat=Mat(Size(5,1),CV_8UC1,Scalar(0));
	cv::Mat bgMat_1, bgMat_2;
	cv::Mat DataMat= Mat(Size(64, 48), CV_8UC3, Scalar(0,0,0));//�洢ÿ�����صľ�ֵ�ͷ���
	cv::Mat frame_now;
	cv::Mat dstMat = Mat(Size(64, 48), CV_8UC1, Scalar(0));
	int cnt = 0;
	int difference;
	int threshold = 10;
	double scale = 0.1;

	const int channels[1] = { 0 };
	const int histSize[1] = { 256 };
	float hranges[2] = { 0,255 };
	const float* ranges[1] = { hranges };
	Mat hist;
	Scalar convr, Mean;
	

		
	//����Ƶͼ���ǰ��֡��ĳ�����ز��� �ȱ������ص�λ�ò���
	for (int m = 0; m < 48; m++)
	{
		for (int n = 0; n < 64; n++)
		{
			while (cnt < 5)
			{
				//����֡ͼ���ĳ�����ص� �����MeanMat
				for (int i = 0; i < MeanMat.rows; i++)
				{
					for (int j = 0; j < MeanMat.cols; j++)
					{
						cap >> frame;
						Size ResImgSiz = Size(frame.cols*scale, frame.rows*scale);
						Mat matSrc = Mat(ResImgSiz, frame.type());
						resize(frame, matSrc, ResImgSiz, INTER_LINEAR);
						cvtColor(matSrc, matSrc, COLOR_BGR2GRAY);

						MeanMat.at<uchar>(i, j) = matSrc.at<uchar>(m, n);
						cnt++;
					}
				}

			}
			//��ͼ��MeanMat չ��Ϊ1�С�100�е�Mat��
			Mat MeanMat_1 = MeanMat.reshape(0, 1);

			//���ֵ�ͷ���
			meanStdDev(MeanMat_1, Mean, convr);
			//ȡ����ֵ�ͷ���
			int var_value = convr.val[0];
			int mean_value = Mean.val[0];

			DataMat.at<Vec3b>(m, n)[0] = mean_value;
			DataMat.at<Vec3b>(m, n)[1] = var_value;
			cnt = 0;
		}
	}
				//while (1)
				//{

					//�Ƚϵ�ǰ֡�ĸ����ص� ��ǰ�����Ǳ���
					cap >> frame_now;
					cvtColor(frame_now, frame_now, COLOR_BGR2GRAY);
					imshow("��Ƶͼ��", frame_now);
					for (int i = 0; i < 48; i++)
					{
						for (int j = 0; j < 64; j++)
						{
							//���㵱ǰ֡�ĸ����ص���֮ǰ�ľ�ֵ�Ĳ�
							difference = abs(frame_now.at<uchar>(i, j) - DataMat.at<Vec3b>(i,j)[0]);

							if (difference > threshold * DataMat.at<Vec3b>(i,j)[1])
								dstMat.at<uchar>(i, j) = 255;
							else
								dstMat.at<uchar>(i, j) = 0;
						}
					}

					imshow("���ͼ��", dstMat);
					waitKey(30);
				//}
	
			
				
	
}