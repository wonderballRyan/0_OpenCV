#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;

//--------------------------------【宏定义部分】--------------------------------------
//描述：定义一些辅助宏
//------------------------------------------------------------------------------------
#define WINDOW_NAME "【程序窗口】"//为窗口标题定义的宏

//---------------------------------【全局变量声明部分】-------------------------------
//描述：全局变量的声明
//------------------------------------------------------------------------------------
int g_nThresholdValue = 100;
int g_nThresholdType = 3;
Mat g_srcImage, g_grayImage, g_dstImage;

//---------------------------------【全局函数声明部分】--------------------------------
//描述：全局函数的声明
//-------------------------------------------------------------------------------------
static void ShowHelpText();//输出帮助文字
void on_Threshold(int, void*);//回调函数

//----------------------------------【main()函数】-------------------------------------
//描述：控制台应用程序的入口函数，我们的程序从这里开始执行
//-------------------------------------------------------------------------------------
int main()
{
	//【1】读入源图片
	g_srcImage = imread("F:\\img_process\\imgs\\20210127\\1.color.png");
	if (!g_srcImage.data)
	{
		printf("读取图片错误，请确定目录下是否有imread函数指定的图片存在！\n");
		return false;
	}

	//存留一份原图的灰度图
	cvtColor(g_srcImage, g_grayImage, COLOR_RGB2GRAY);

	//创建窗口并显示原图
	namedWindow(WINDOW_NAME, WINDOW_AUTOSIZE);

	//【4】创建滑动条来控制阈值
	createTrackbar("模式", WINDOW_NAME, &g_nThresholdType, 4, on_Threshold);

	createTrackbar("参数值", WINDOW_NAME, &g_nThresholdValue, 255, on_Threshold);

	//【5】初始化自定义的阈值回调函数
	on_Threshold(0, 0);

	//【6】等待用户按键，如果ESC键按下则退出程序
	while (1)
	{
		int key;
		key = waitKey(20);
		if ((char)key == 27)
		{
			break;
		}
	}
}

//------------------------------【on_Threshold()函数】----------------------------
//描述：自定义的阈值回调函数
//--------------------------------------------------------------------------------
void on_Threshold(int, void*)
{
	//调用阈值函数
	threshold(g_grayImage, g_dstImage, g_nThresholdValue, 255, g_nThresholdType);

	//更新效果图
	imshow(WINDOW_NAME, g_dstImage);
}
