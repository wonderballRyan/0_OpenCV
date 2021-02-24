#include <iostream>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//------------------------------------【全局变量声明部分】----------------------------------
//描述：全局变量声明
//------------------------------------------------------------------------------------------
Mat g_srcImage, g_dstImage;
int g_nElementShape = MORPH_RECT;

//变量接收的TrackBar位置参数
int g_nMaxIterationNum = 10;
int g_nOpenCloseNum = 0;
int g_nErodeDilateNum = 0;
int g_nTopBlackHatNum = 0;

//-------------------------------------【全局函数声明部分】--------------------------------
//描述：全局函数声明
//-----------------------------------------------------------------------------------------
static void on_OpenClose(int, void*);//回调函数
static void on_ErodeDilate(int, void*);//回调函数
static void on_TopBlackHat(int, void*);//回调函数
static void ShowHelpText();//帮助文字显示

//-------------------------------------【mian()函数】--------------------------------------
//描述：控制台应用程序的入口含少数，我们的程序从这里开始
//-----------------------------------------------------------------------------------------
int main()
{
	//载入原因
	g_srcImage = imread("F:\\opencv\\6_imgproc\\images\\1.jpg");
	if (!g_srcImage.data)
	{
		printf("读取srcImage错误！\n");
		return false;
	}

	//显示原始图
	namedWindow("【原始图】");
	imshow("【原始图】", g_srcImage);

	//创建三个窗口
	namedWindow("【开运算/闭运算】", 1);
	namedWindow("【腐蚀/膨胀】", 1);
	namedWindow("【顶帽/黑帽】", 1);

	//参数赋值
	g_nOpenCloseNum = 9;
	g_nErodeDilateNum = 9;
	g_nTopBlackHatNum = 2;

	//分别为三个窗口创建滚动条
	createTrackbar("迭代值", "【开运算/闭运算】", &g_nOpenCloseNum, g_nMaxIterationNum * 2 + 1, on_OpenClose);
	createTrackbar("迭代值", "【腐蚀/膨胀】", &g_nErodeDilateNum, g_nMaxIterationNum * 2 + 1, on_ErodeDilate);
	createTrackbar("迭代值", "【顶帽/黑帽】", &g_nTopBlackHatNum, g_nMaxIterationNum * 2 + 1, on_TopBlackHat);

	//轮询获取按键信息
	while (1)
	{
		int c;

		//执行回调函数
		on_OpenClose(g_nOpenCloseNum, 0);
		on_ErodeDilate(g_nErodeDilateNum, 0);
		on_TopBlackHat(g_nTopBlackHatNum, 0);

		//获取按键
		c = waitKey(0);

		//按下键盘按键Q或者ESC，程序退出
		if ((char)c == 'q' || (char)c == 27)
			break;
		//按下键盘按键1，使用椭圆（Elliptic）结构元素MORPH_ELLIPSE
		if ((char)c == 49)//键盘按键1的ASCII码为49
			g_nElementShape = MORPH_ELLIPSE;
		//按下键盘按键2，使用矩形（Rectangle）结构元素MORPH_RECT
		else if ((char)c == 50)//键盘按键2的ASICC，码为50
			g_nElementShape = MORPH_RECT;
		//按下键盘按键3，使用十字形（Cross_shaped）结构元素MORPH_CROSS
		else if ((char)c == 51)//键盘按键3的ASCII码为51
			g_nElementShape = MORPH_CROSS;
		//按下键盘按键space，在矩形、椭圆、十字形结构元素中循环
		else if ((char)c == ' ')
			g_nElementShape = (g_nElementShape+1)%3;
	}

	return 0;
}

//---------------------------------------【on_OpenClose()函数】-------------------------------
//描述：【开运算/闭运算】窗口的回调函数
//------------------------------------------------------------------------------------------
static void on_OpenClose(int, void*)
{
	//偏移量的定义
	int offset = g_nOpenCloseNum - g_nMaxIterationNum;//偏移量
	int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
	//自定义核
	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset * 2 + 1, Absolute_offset * 2 + 1), Point(Absolute_offset, Absolute_offset));
	//进行操作
	if (offset < 0)
		morphologyEx(g_srcImage, g_dstImage, MORPH_OPEN, element);
	else
		morphologyEx(g_srcImage, g_dstImage, MORPH_CLOSE, element);
	//显示图像
	imshow("【开运算/闭运算】", g_dstImage);
}

//-------------------------------------【on_ErodeDilate()函数】-----------------------
//描述：【腐蚀/膨胀】窗口的回调函数
//------------------------------------------------------------------------------------
static void on_ErodeDilate(int, void*)
{
	//偏移量的定义
	int offset = g_nErodeDilateNum - g_nMaxIterationNum;//偏移量
	int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
	//自定义核
	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset * 2 + 1, Absolute_offset * 2 + 1), Point(Absolute_offset, Absolute_offset));
	//进行操作
	if (offset < 0)
		erode(g_srcImage, g_dstImage, element);
	else
		dilate(g_srcImage, g_dstImage, element);
	//显示图像
	imshow("【腐蚀/膨胀】", g_dstImage);
}

//-----------------------------------【on_TopBlackHat()函数】-------------------------------
//描述：【顶帽运算/黑帽运算】窗口的回调函数
//------------------------------------------------------------------------------------------
static void on_TopBlackHat(int, void*)
{
	//偏移量的定义
	int offset = g_nTopBlackHatNum - g_nMaxIterationNum;//偏移量
	int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset * 2 + 1, Absolute_offset * 2 + 1), Point(Absolute_offset, Absolute_offset));
	//进行操作
	if (offset < 0)
		morphologyEx(g_srcImage, g_dstImage, MORPH_TOPHAT, element);
	else
		morphologyEx(g_srcImage, g_dstImage, MORPH_BLACKHAT, element);
	//显示图像
	imshow("【顶帽/黑帽】", g_dstImage);
}
