#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/types_c.h"

using namespace std;
using namespace cv;

//------------------------------【阈值法函数声明】-------------------------------
//描述：各种阈值法函数声明
//-------------------------------------------------------------------------------
Mat OtsuAlgThreshold(Mat& image);
void myadaptive(InputArray _src, OutputArray _dst, double maxValue,
	int method, int type, int blockSize, double delta);
Mat EntropySeg(Mat src);
Mat IterationThreshold(Mat src);

//------------------------------【函数入口】--------------------------------------
//描述：函数入口main()
//-------------------------------------------------------------------------------
int main()
{
	Mat src = imread("F:\\opencv\\prj_imgSegmentation\\images\\2.jpg");
	if (src.data == 0)
	{
		printf("图片加载失败！");
		return false;
	}
	cvtColor(src, src, COLOR_RGB2GRAY);
	Mat bw1, bw2, bw3, bw4;
	bw1 = OtsuAlgThreshold(src);
	myadaptive(src, bw2, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 15, 10);
	bw3 = EntropySeg(src);
	bw4= IterationThreshold(src);
	imshow("Otsu阈值分割法", bw1);
	imshow("自适应阈值分割法", bw2);
	imshow("最大熵阈值分割法", bw3);
	imshow("迭代阈值分割法", bw4);
	waitKey(0);

}

//---------------------------------------------【Otsu阈值分割法函数定义】----------------------------
//描述：OtsuAlgThreshold()函数定义
//-------------------------------------------------------------------------------------------------
Mat OtsuAlgThreshold(Mat& image)
{
	if (image.channels() != 1)
	{
		cout << "Please input Gray-image!" << endl;
	}
	int T = 0; //Otsu算法阈值  
	double varValue = 0; //类间方差中间值保存
	double w0 = 0; //前景像素点数所占比例  
	double w1 = 0; //背景像素点数所占比例  
	double u0 = 0; //前景平均灰度  
	double u1 = 0; //背景平均灰度  
	double Histogram[256] = { 0 }; //灰度直方图，下标是灰度值，保存内容是灰度值对应的像素点总数  
	uchar* data = image.data;

	double totalNum = image.rows * image.cols; //像素总数

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if (image.at<uchar>(i, j) != 0) Histogram[data[i * image.step + j]]++;
		}
	}
	int minpos, maxpos;
	for (int i = 0; i < 255; i++)
	{
		if (Histogram[i] != 0)
		{
			minpos = i;
			break;
		}
	}
	for (int i = 255; i > 0; i--)
	{
		if (Histogram[i] != 0)
		{
			maxpos = i;
			break;
		}
	}

	for (int i = minpos; i <= maxpos; i++)
	{
		//每次遍历之前初始化各变量  
		w1 = 0;       u1 = 0;       w0 = 0;       u0 = 0;
		//***********背景各分量值计算**************************  
		for (int j = 0; j <= i; j++) //背景部分各值计算  
		{
			w1 += Histogram[j];   //背景部分像素点总数  
			u1 += j * Histogram[j]; //背景部分像素总灰度和  
		}
		if (w1 == 0) //背景部分像素点数为0时退出  
		{
			break;
		}
		u1 = u1 / w1; //背景像素平均灰度  
		w1 = w1 / totalNum; // 背景部分像素点数所占比例
		//***********背景各分量值计算**************************  

		//***********前景各分量值计算**************************  
		for (int k = i + 1; k < 255; k++)
		{
			w0 += Histogram[k];  //前景部分像素点总数  
			u0 += k * Histogram[k]; //前景部分像素总灰度和  
		}
		if (w0 == 0) //前景部分像素点数为0时退出  
		{
			break;
		}
		u0 = u0 / w0; //前景像素平均灰度  
		w0 = w0 / totalNum; // 前景部分像素点数所占比例  
		//***********前景各分量值计算**************************  

		//***********类间方差计算******************************  
		double varValueI = w0 * w1 * (u1 - u0) * (u1 - u0); //当前类间方差计算  
		if (varValue < varValueI)
		{
			varValue = varValueI;
			T = i;
		}
	}
	Mat dst;
	threshold(image, dst, T, 255, THRESH_OTSU);
	return dst;
}

//---------------------------------------------【自适应阈值分割法定义】----------------------------
//描述：myadaptive()函数定义
//-------------------------------------------------------------------------------------------------
void myadaptive(InputArray _src, OutputArray _dst, double maxValue,
	int method, int type, int blockSize, double delta)
{
	Mat src = _src.getMat();

	CV_Assert(src.type() == CV_8UC1);
	CV_Assert(blockSize % 2 == 1 && blockSize > 1);
	Size size = src.size();

	_dst.create(size, src.type());
	Mat dst = _dst.getMat();

	if (maxValue < 0)
	{
		dst = Scalar(0);
		return;
	}

	Mat mean;
	if (src.data != dst.data)
		mean = dst;
	if (method == ADAPTIVE_THRESH_GAUSSIAN_C)
	{
		GaussianBlur(src, mean, Size(blockSize, blockSize), 0, 0, BORDER_REPLICATE);
	}
	else if (method == ADAPTIVE_THRESH_MEAN_C)
	{
		boxFilter(src, mean, src.type(), Size(blockSize, blockSize),
			Point(-1, -1), true, BORDER_REPLICATE);
	}
	else
	{
		
		CV_Error(CV_StsBadFlag, "Unknown/unsupported adaptive threshold method");
	}

	int i, j;
	uchar imaxval = saturate_cast<uchar>(maxValue);
	int idelta = type == THRESH_BINARY ? cvCeil(delta) : cvFloor(delta);
	uchar tab[768];

	if (type == THRESH_BINARY)
		for (i = 0; i < 768; i++)
			tab[i] = (uchar)(i - 255 > -idelta ? imaxval : 0);
	else if (type == THRESH_BINARY_INV)
		for (i = 0; i < 768; i++)
			tab[i] = (uchar)(i - 255 <= -idelta ? imaxval : 0);
	else
	{
		CV_Error(CV_StsBadFlag, "Unknown/unsupported threshold type");
	}

	if (src.isContinuous() && mean.isContinuous() && dst.isContinuous())
	{
		size.width *= size.height;
		size.height = 1;
	}

	for (i = 0; i < size.height; i++)
	{
		const uchar* sdata = src.data + src.step * i;
		const uchar* mdata = mean.data + mean.step * i;
		uchar* ddata = dst.data + dst.step * i;

		for (j = 0; j < size.width; j++)
			// 将[-255, 255] 映射到[0, 510]然后查表
			ddata[j] = tab[sdata[j] - mdata[j] + 255];
	}
}


//---------------------------------------------【最大熵阈值分割法定义】----------------------------
//描述：EntropySeg()函数定义
//-------------------------------------------------------------------------------------------------
Mat EntropySeg(Mat src)
{
	int tbHist[256] = { 0 };
	int index = 0;
	double Property = 0.0;
	double maxEntropy = -1.0;
	double frontEntropy = 0.0;
	double backEntropy = 0.0;
	int TotalPixel = 0;
	int nCol = src.cols * src.channels();
	for (int i = 0; i < src.rows; i++)
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; j++)
		{
			++TotalPixel;
			tbHist[pData[j]] += 1;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		double backTotal = 0;
		for (int j = 0; j < i; j++)
		{
			backTotal += tbHist[j];
		}

		for (int j = 0; j < i; j++)
		{
			if (tbHist[j] != 0)
			{
				Property = tbHist[j] / backTotal;
				backEntropy += -Property * logf((float)Property);
			}
		}

		for (int k = i; k < 256; k++)
		{
			if (tbHist[k] != 0)
			{
				Property = tbHist[k] / (TotalPixel - backTotal);
				frontEntropy += -Property * logf((float)Property);
			}
		}

		if (frontEntropy + backEntropy > maxEntropy)
		{
			maxEntropy = frontEntropy + backEntropy;
			index = i;
		}

		frontEntropy = 0.0;
		backEntropy = 0.0;
	}

	Mat dst;
	threshold(src, dst, index, 255, 0);
	return dst;
}

//---------------------------------------------【迭代阈值分割法定义】----------------------------
//描述：IterationThreshold()函数定义
//-------------------------------------------------------------------------------------------------
Mat IterationThreshold(Mat src)
{
	int width = src.cols;
	int height = src.rows;
	int hisData[256] = { 0 };
	for (int j = 0; j < height; j++)
	{
		uchar* data = src.ptr<uchar>(j);
		for (int i = 0; i < width; i++)
			hisData[data[i]]++;
	}

	int T0 = 0;
	for (int i = 0; i < 256; i++)
	{
		T0 += i * hisData[i];
	}
	T0 /= width * height;

	int T1 = 0, T2 = 0;
	int num1 = 0, num2 = 0;
	int T = 0;
	while (1)
	{
		for (int i = 0; i < T0 + 1; i++)
		{
			T1 += i * hisData[i];
			num1 += hisData[i];
		}
		if (num1 == 0)
			continue;
		for (int i = T0 + 1; i < 256; i++)
		{
			T2 += i * hisData[i];
			num2 += hisData[i];
		}
		if (num2 == 0)
			continue;

		T = (T1 / num1 + T2 / num2) / 2;

		if (T == T0)
			break;
		else
			T0 = T;
	}

	Mat dst;
	threshold(src, dst, T, 255, 0);
	return dst;
}
