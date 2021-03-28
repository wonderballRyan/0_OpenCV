#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//-----------------------------------【宏定义部分】--------------------------------
//描述：定义一些辅助宏
//---------------------------------------------------------------------------------
# define WINDOW_NAME1 "【原始图】"    //为窗口标题定义的宏
# define WINDOW_NAME2 "【修补后的效果图】"//为窗口标题定义的宏

//-------------------------------------【全局变量声明部分】---------------------------
//描述：全局变量声明
//----------------------------------------------------------------------------------
Mat srcImage1, inpaintMask;
Point previousPoint(-1, -1);//原来的点坐标

//-----------------------------------------【On_mouse()函数】-----------------------------
//描述：响应鼠标消息的回调函数
//--------------------------------------------------------------------------------------
static void On_Mouse(int event, int x, int y, int flags, void*)
{
	//鼠标左键弹起消息
	if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))
		previousPoint = Point(-1, -1);
	//鼠标左键按下消息
	else if (event == EVENT_LBUTTONDOWN)
		previousPoint = Point(x, y);
	//鼠标按下并移动，进行绘制
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
	{
		Point pt(x, y);
		if (previousPoint.x < 0)
			previousPoint = pt;
		//绘制白色线条
		line(inpaintMask, previousPoint, pt, Scalar::all(255), 5, 8, 0);
		line(srcImage1,previousPoint,pt,Scalar::all(255),5,8,0);
		previousPoint = pt;
		imshow(WINDOW_NAME1,srcImage1);
	}
}

//------------------------------【main()函数】---------------------------------
//描述：控制台应用程序的入口函数，我们的程序从这里开始执行
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//载入原始图并进行掩膜的初始化
	Mat srcImage = imread("F:\\img_process\\imgs\\4.jpg");
	if (!srcImage.data)
	{
		printf("读取图片错误！\n");
		return false;
	}
	srcImage1 = srcImage.clone();
	inpaintMask = Mat::zeros(srcImage1.size(), CV_8U);

	//显示原始图
	imshow(WINDOW_NAME1, srcImage1);

	//设置鼠标回调消息
	setMouseCallback(WINDOW_NAME1, On_Mouse, 0);

	//轮询按键，根据不同的按键进行处理
	while (1)
	{
		//获取按键键值
		char c = (char)waitKey();

		//键值为ESC，程序推出
		if (c == 27)
			break;

		//键值为2，恢复成原始图像
		if (c == '2')
		{
			inpaintMask = Scalar::all(0);
			srcImage.copyTo(srcImage1);
			imshow(WINDOW_NAME1, srcImage1);
		}

		//键值为1或者空格，进行图像修补操作
		if (c == '1' || c == ' ')
		{
			Mat inpaintedImage;
			inpaint(srcImage1, inpaintMask, inpaintedImage, 3, INPAINT_TELEA);
			imshow(WINDOW_NAME2, inpaintedImage);
		}
	}
		return 0;
}
