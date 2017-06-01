/*
void cv::grabCut( InputArray _img, InputOutputArray _mask, Rect rect,

                  InputOutputArray _bgdModel, InputOutputArray _fgdModel,

                  int iterCount, int mode )
参数说明：
img: 分割的源图像，必须是8位3通道（CV_8UC3）图像，在处理的过程中不会被修改；
mask: 掩码图像，如果使用掩码进行初始化，那么mask保存初始化掩码信息；在执行分割的时候，也可以将用户交互所设定的前景与背景保存到mask中，然后再传入grabCut函数；在处理结束之后，mask中会保存结果。mask只能取以下四种值：
                   GCD_BGD（=0），背景；
                   GCD_FGD（=1），前景；
                   GCD_PR_BGD（=2），可能的背景；
                   GCD_PR_FGD（=3），可能的前景。
rect——用于限定需要进行分割的图像范围，只有该矩形窗口内的图像部分才被处理；
bgdModel——背景模型，如果为null，函数内部会自动创建一个bgdModel；bgdModel必须是单通道浮点型（CV_32FC1）图像，
且行数只能为1，列数只能为13x5；
fgdModel——前景模型，如果为null，函数内部会自动创建一个fgdModel；fgdModel必须是单通道浮点型（CV_32FC1）图像，
且行数只能为1，列数只能为13x5；
iterCount——迭代次数，必须大于0；
mode——用于指示grabCut函数进行什么操作，可选的值有：
                   GC_INIT_WITH_RECT（=0），用矩形窗初始化GrabCut；
                   GC_INIT_WITH_MASK（=1），用掩码图像初始化GrabCut；
                   GC_EVAL（=2），执行分割。
*/
/*
迭代最小化算法
1.为每个像素分配GMM中所属的高斯模型
2.从每个高斯模型的像素样本集中学习每个高斯模型的参数，并根据计算得到的能量项构建图
3.分割估计：最小割后者最大流算法
*/
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

string filename;
Mat image;
string winName = "show";
enum{NOT_SET = 0, IN_PROCESS = 1, SET = 2};
uchar rectState;
Rect rect;
Mat mask;
const Scalar GREEN = Scalar(0, 255, 0);
Mat bgdModel, fgdModel;
/*
void setRectMask()
{
	rect.x = max(0, rect.x);
	rect.y = max(0, rect.y);
	rect.width = min(rect.width, image.cols-rect.x);
	rect.height = min(rect.height, image.rows-rect.y);
}

static void getBinMask(const Mat& comMask, Mat& binMask)
{
	binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;
}

void on_mouse(int event, int x, int y, int flags, void *)
{
	Mat res;
	Mat binMask;

	switch(event)
	{
	case CV_EVENT_LBUTTONDOWN:
		if(rectState == NOT_SET)
		{
			rectState = IN_PROCESS;
			rect = Rect(x, y, 1, 1);
		}
		break;
	case CV_EVENT_LBUTTONUP:
		if(rectState == IN_PROCESS)
		{
			rect = Rect(Point(rect.x, rect.y), Point(x, y));
			rectState = SET;
			(mask(rect)).setTo(Scalar(GC_PR_FGD));
			image = imread(filename, 1);
			grabCut(image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);
			getBinMask(mask, binMask);
			image.copyTo(res, binMask);
			imshow("11", res);
		}
		break;
	case CV_EVENT_MOUSEMOVE:
		if(rectState == IN_PROCESS)
		{
			rect = Rect(Point(rect.x, rect.y), Point(x, y));
			image = imread(filename, 1);
			rectangle(image, Point(rect.x, rect.y), Point(rect.x+rect.width, rect.y+rect.height), GREEN, 2);
			imshow(winName, image);
		}
		break;
	}
}

int main(int argc, char* argv[])
{
	filename = "D:\\我的文档\\Documents\\ml\\grabCut\\grabCut\\1.jpg";
	image = imread(filename, 1);
	imshow(winName, image);

	mask.create(image.size(), CV_8UC1);  // 设置掩码
	rectState = NOT_SET;
	mask.setTo(GC_BGD);  // 将整个掩码空间设置为背景

	setMouseCallback(winName, on_mouse, 0);
	waitKey(0);

	return 0;
}
*/

// 创建rect
void setRectInMask()
{
	rect.x = 5;
	rect.y = 13;
	rect.width = 50;
	rect.height = 40;
}

// 筛选前景
static void getBinMask(const Mat& comMask, Mat& binMask)
{
	binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;
}

int main(int argc, char* argv[])
{
	// 加载源图像
	Mat binMask, res;
	filename = "D:\\我的文档\\Documents\\ml\\grabCut\\grabCut\\1.jpg";
	image = imread(filename, 1);
	imshow(winName, image);
    
	// 设置掩码
	mask.create(image.size(), CV_8UC1);  // 设置掩码
	mask.setTo(GC_BGD);  // 将整个掩码空间设置为背景
	
	// 设置前景
	setRectInMask();
	(mask(rect)).setTo(Scalar(GC_PR_FGD));
	rectangle(image, Point(rect.x, rect.y), Point(rect.x+rect.width, rect.y+rect.height), GREEN, 2);
	imshow(winName, image);

	// grabCut分离,结果保存在mask中（背景0，前景1）
	image = imread(filename, 1);
	grabCut(image, mask, rect,bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);

	// 筛选前景
	getBinMask(mask, binMask);
	image.copyTo(res, binMask);
	imshow("result", res);

	waitKey(0);

	return 0;
}