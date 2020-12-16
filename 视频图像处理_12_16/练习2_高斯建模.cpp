#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdlib.h>
using namespace cv;
using namespace std;

//实现高斯建模及背景差分 
//思路：先读取视频图像的前五帧的某个位置像素点，填充满MeanMat,再调用函数计算该像素点的均值和方差，同样的操作遍历每个像素。之后再读图进来，进行比较
//由于我的代码读每个像素都读了一帧图进来，耗费的时间很多，所以把视频图像的大小缩为原来的10倍（但依然可能要运行好几分钟 看不出结果）
int main()
{
	VideoCapture cap;
	cap.open(0);
	cv::Mat frame = Mat(Size(640,480),CV_8UC3, Scalar(0));
	cv::Mat MeanMat=Mat(Size(5,1),CV_8UC1,Scalar(0));
	cv::Mat bgMat_1, bgMat_2;
	cv::Mat DataMat= Mat(Size(64, 48), CV_8UC3, Scalar(0,0,0));//存储每个像素的均值和方差
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
	

		
	//对视频图像的前五帧的某个像素操作 先保持像素点位置不变
	for (int m = 0; m < 48; m++)
	{
		for (int n = 0; n < 64; n++)
		{
			while (cnt < 5)
			{
				//对五帧图像的某个像素点 填充满MeanMat
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
			//将图像MeanMat 展开为1行、100列的Mat型
			Mat MeanMat_1 = MeanMat.reshape(0, 1);

			//求均值和方差
			meanStdDev(MeanMat_1, Mean, convr);
			//取出均值和方差
			int var_value = convr.val[0];
			int mean_value = Mean.val[0];

			DataMat.at<Vec3b>(m, n)[0] = mean_value;
			DataMat.at<Vec3b>(m, n)[1] = var_value;
			cnt = 0;
		}
	}
				//while (1)
				//{

					//比较当前帧的该像素点 是前景还是背景
					cap >> frame_now;
					cvtColor(frame_now, frame_now, COLOR_BGR2GRAY);
					imshow("视频图像", frame_now);
					for (int i = 0; i < 48; i++)
					{
						for (int j = 0; j < 64; j++)
						{
							//计算当前帧的该像素点与之前的均值的差
							difference = abs(frame_now.at<uchar>(i, j) - DataMat.at<Vec3b>(i,j)[0]);

							if (difference > threshold * DataMat.at<Vec3b>(i,j)[1])
								dstMat.at<uchar>(i, j) = 255;
							else
								dstMat.at<uchar>(i, j) = 0;
						}
					}

					imshow("差分图像", dstMat);
					waitKey(30);
				//}
	
			
				
	
}