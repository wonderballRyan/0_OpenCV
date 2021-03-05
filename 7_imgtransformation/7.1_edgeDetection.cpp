#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

void canny()
{
	//载入原图
	Mat src = imread("F:\\opencv\\images\\1.jpg");//工程目录下应该有一张名为1.jpg的照片
	Mat src1 = src.clone();

	//显示原始图
	imshow("【原始图】Canny边缘检测", src);

	//高阶的canny用法，转成灰度图，降噪，用canny，最后将得到的边缘作为掩码，拷贝原图到效果图上，得到彩色的边缘图
	//-----------------------------------------------------------------------------
	Mat dst, edge, gray;

	//【1】创建与src同类型和大小的矩阵（dst）
	dst.create(src1.size(), src1.type());

	//【2】将原图像转换为灰度图像
	cvtColor(src1, gray, COLOR_BGR2GRAY);

	//【3】先使用3*3内核来降噪
	blur(gray, edge, Size(3, 3));

	//【4】运行Canny算子
	Canny(edge, edge, 3, 9, 3);

	//【5】将g_dstImage内的所有元素设置为0
	dst = Scalar::all(0);

	//【6】使用Canny算子输出的边缘图g_cannyDetectedEdges作为掩码，来将原图g_srcImage拷到目标图g_dstImage中
	src1.copyTo(dst, edge);

	//【7】显示效果图
	imshow("【效果图】Canny边缘检测", dst);
}

void sobel()
{
	//【0】创建grad_x和grad_y矩阵
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y, dst;

	//【1】载入原始图
	Mat src = imread("F:\\opencv\\images\\1.jpg");//工程目录下应该有一张名为1.jpg的素材图

	//【2】显示原始图
	imshow("【原始图】sobel边缘检测", src);

	//【3】求X方向梯度
	Sobel(src, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	imshow("【效果图】X方向Sobel", abs_grad_x);

	//【4】求Y方向梯度
	Sobel(src, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	imshow("【效果图】Y方向Sobel", abs_grad_y);

	//【5】合并梯度（近似）
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
	imshow("【效果图】整体方向Sobel", dst);
}

void laplacian()
{
	//【0】变量的定义
	Mat src, src_gray, dst,abs_dst;

	//【1】载入原始图
	src = imread("F:\\opencv\\images\\1.jpg");

	//【2】显示原始图
	imshow("【原始图】图像Laplace变换", src);

	//【3】使用高斯滤波消除噪声
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	//【4】转换为灰度图
	cvtColor(src, src_gray, COLOR_RGB2GRAY);

	//【5】使用Laplace函数
	Laplacian(src_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);

	//【6】计算绝对值，并将结果转换为8位
	convertScaleAbs(dst, abs_dst);

	//【7】显示效果图
	imshow("【效果图】图像Laplace变换", abs_dst);
}

void scharr()
{
	//【0】创建grad_x和grad_y矩阵
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y, dst;

	//【1】载入原始图
	Mat src = imread("F:\\opencv\\images\\1.jpg");

	//【2】显示原始图
	imshow("【原始图】Scharr滤波器", src);

	//【3】求X方向梯度
	Scharr(src, grad_x, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	imshow("【效果图】X方向Scharr", abs_grad_x);

	//【4】求Y方向梯度
	Scharr(src, grad_y, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	imshow("【效果图】Y方向Scharr", abs_grad_y);

	//【5】合并梯度（近似）
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);

	//【6】显示效果图
	imshow("【效果图】合并梯度后Scharr", dst);

}
//-----------------------------【main()函数】---------------------------------
//描述：控制台应用程序的入口函数，我们的程序从这里开始
//----------------------------------------------------------------------------
int main()
{
	
	//canny();
	//sobel();
	//laplacian();
	scharr();
	waitKey(0);
	return 0;
}
