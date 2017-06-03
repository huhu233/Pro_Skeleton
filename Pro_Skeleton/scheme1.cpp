/*
	平台:console程序
	功能:实现关节点绘制，拖动, 并且可以选取骨骼来模拟遮挡关系
	输入：根据提示输入
	trackbar:0，可绘制状态（无法拖拽），左键点击图片会生成相应的点
	trackbar:1, 可拖动状态（无法绘制），右击关节点移动鼠标可进行拖拽，左击固定；左击选中骨骼的两个点，骨骼颜色会发生变化
	输出：根据提示保存，文件名按照系统时间命名(默认保存的路径是D:\project\outPut\*.jpg)
*/

#include<Windows.h>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv/cv.hpp>
#include<vector>
#include<cstring>
#include<cstdio>
#include<iostream>
#include<string>
#include<cmath>
#include"type.h"

std::string outputPath = "D:\\project\\outPut\\";

IplImage *img = NULL;
IplImage *imgNew = NULL;
bool lock_status;						//锁定标志：false-可绘制，true-可拖拽
bool onMove;							//拖拽开关
int curJoint;							//记录选中的关节点，初始值为-1（在移动节点时可以使用）
int vertexNum;							//关节点数量
int trackValue;

std::vector<Point> vTotal;				//关节
std::vector<Bone> bTotal;				//骨骼
std::vector<int> tmp;
std::vector<int> clkJoint;				//选择骨骼时存储骨骼的两个节点

//初始化
void init();
//鼠标响应事件
void onMouse(int event, int x, int y, int flags, void *param);
//判断鼠标是否处于关节点处
int isJoint(int x, int y);
//移动关节点
void DoMovement(int xpos, int ypos, int curPos);
//trackbar响应事件
void onChange(int pos);
//重绘骨架
void redraw(int curPos);
//重绘骨骼（线条）
void redrawBone();
//重绘节点
void redrawPoint();
//判断点击的两点能否构成骨骼(在设置骨骼深度时使用)
int isBone(int p1, int p2);


int main()
{
	int choice, save;
	char str[100];
	printf("请输入选择0-结束，1-编辑: ");
	scanf("%d", &choice);
	while (choice)
	{
		system("cls");
		init();

		printf("请输入照片路径: ");
		scanf("%s", str);
		img = cvLoadImage(str, 1);
		if (img != NULL)						//确保图像路径正确
		{
			imgNew = (IplImage*)cvClone(img);
			cvNamedWindow(WINNAME, 1);
			cvCreateTrackbar(TRACKBARNAME, WINNAME, &trackValue, 1, onChange);
			cvShowImage(WINNAME, imgNew);
			cvSetMouseCallback(WINNAME, onMouse, 0);
			cvWaitKey(0);
			//保存图片选择
			printf("是否保存？0-否，1-是: ");
			scanf("%d", &save);
			if (save)
			{
				SYSTEMTIME st;
				char imgName[100];

				GetLocalTime(&st);
				sprintf(imgName, "%02d%02d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				cvSaveImage((outputPath + imgName + ".jpg").c_str(), imgNew);
				printf("已保存\n");
			}
			else
			{
				printf("\a未保存!\n");
			}

			if (imgNew != NULL)
			{
				cvReleaseImage(&imgNew);
				imgNew = NULL;
			}
			if (img != NULL)
			{
				cvReleaseImage(&img);
				img = NULL;
			}
			cvDestroyWindow(WINNAME);
		}
		else
		{
			printf("\a路径错误\n");
		}
		printf("请继续输入选择:0-退出，1-继续编辑: ");
		scanf("%d", &choice);
		if (!choice) printf("正在退出...");
	}
	return 0;
}

void init()
{
	vTotal.clear();
	bTotal.clear();		
	tmp.clear();
	clkJoint.clear();
	curJoint = -1;
	vertexNum = 0;
	trackValue = 0;
	lock_status = false;
	onMove = false;
}

void onChange(int pos)
{
	clkJoint.clear();
	if (pos == 0) lock_status = false;
	else lock_status = true;
}

void redrawPoint()
{
	for (unsigned int i = 0; i < vTotal.size(); i++)
	{
		cvCircle(imgNew, cvPoint(vTotal[i].x, vTotal[i].y), RADIUS, PCOLOR, CV_FILLED, cv::LINE_8, 0);
	}
}

void redrawBone()
{
	for (unsigned int i = 0; i < bTotal.size(); i++)
	{
		int x1, y1, x2, y2;
		x1 = vTotal[bTotal[i].p1].x;
		y1 = vTotal[bTotal[i].p1].y;
		x2 = vTotal[bTotal[i].p2].x;
		y2 = vTotal[bTotal[i].p2].y;
		if (bTotal[i].status == ON)
			cvLine(imgNew, cvPoint(x1, y1), cvPoint(x2, y2), BCOLOR_ON, BLINE, cv::LINE_8, 0);
		else
			cvLine(imgNew, cvPoint(x1, y1), cvPoint(x2, y2), BCOLOR_BOTTOM, BLINE, cv::LINE_8, 0);
	}
}

void redraw(int curPos)
{
	//重绘前释放之前的图片
	if (imgNew != NULL)
	{
		cvReleaseImage(&imgNew);
		imgNew = NULL;
	}
	imgNew = (IplImage*)cvClone(img);
	redrawPoint();
	redrawBone();
	cvCircle(imgNew, cvPoint(vTotal[curPos].x, vTotal[curPos].y), RADIUS, PCOLOR_UNLOCK, CV_FILLED, cv::LINE_8, 0);
	cvShowImage(WINNAME, imgNew);

}

//鼠标回调事件
void onMouse(int event, int x, int y, int flags, void *param)
{
	int curPos = isJoint(x, y);
	if (event == CV_EVENT_LBUTTONUP)				//左击绘图
	{
		if (!lock_status)
		{
			clkJoint.clear();
			if (curPos == -1)
			{
				Point pt;
				pt.x = x;
				pt.y = y;
				vTotal.push_back(pt);
				cvCircle(imgNew, cvPoint(x, y), RADIUS, PCOLOR, CV_FILLED, cv::LINE_8, 0);
				tmp.push_back(vertexNum);
				vertexNum++;
			}
			else//防止同一个点点击多遍而存储多遍                                   
			{
				unsigned int i;
				for (i = 0; i < tmp.size(); i++)
				{
					if (tmp[i] == curPos) break;
				}
				if (i == tmp.size())
				{
					tmp.push_back(curPos);
				}
			}
			if (tmp.size() == 2)
			{
				Bone b;
				b.p1 = tmp[0];
				b.p2 = tmp[1];
				b.status = ON;
				bTotal.push_back(b);
				cvLine(imgNew, cvPoint(vTotal[tmp[0]].x, vTotal[tmp[0]].y), cvPoint(vTotal[tmp[1]].x, vTotal[tmp[1]].y), BCOLOR_ON, BLINE, cv::LINE_8, 0);
				tmp.clear();
			}//如果存在两个相异的点，则可以绘制线条

		}
		else                                              
		{
			if (onMove == true)//开关，设置onMove状态
			{
				onMove = false;
				cvCircle(imgNew, cvPoint(vTotal[curJoint].x, vTotal[curJoint].y), RADIUS, PCOLOR, CV_FILLED, cv::LINE_8, 0);
			}
			else//选择骨骼
			{
				if (curPos != -1)
				{
					clkJoint.push_back(curPos);
					cvCircle(imgNew, cvPoint(vTotal[curPos].x, vTotal[curPos].y), RADIUS, PCOLOR_UNLOCK, CV_FILLED, cv::LINE_8, 0);
				}
				if (clkJoint.size() == 2)
				{
					int j0, j1, bNum, x1, y1, x2, y2;
					j0 = clkJoint[0];
					j1 = clkJoint[1];
					bNum = isBone(j0, j1);
					if (bNum != -1)
					{
						x1 = vTotal[j0].x;
						y1 = vTotal[j0].y;
						x2 = vTotal[j1].x;
						y2 = vTotal[j1].y;
						if (bTotal[bNum].status == ON)
						{
							cvLine(imgNew, cvPoint(x1, y1), cvPoint(x2, y2), BCOLOR_BOTTOM, BLINE, cv::LINE_8, 0);
							bTotal[bNum].status = BOTTOM;
						}
						else
						{
							cvLine(imgNew, cvPoint(x1, y1), cvPoint(x2, y2), BCOLOR_ON, BLINE, cv::LINE_8, 0);
							bTotal[bNum].status = ON;
						}

					}
					cvCircle(imgNew, cvPoint(vTotal[j0].x, vTotal[j0].y), RADIUS, PCOLOR, CV_FILLED, cv::LINE_8, 0);
					cvCircle(imgNew, cvPoint(vTotal[j1].x, vTotal[j1].y), RADIUS, PCOLOR, CV_FILLED, cv::LINE_8, 0);
					clkJoint.clear();
				}
			}
		}
		cvShowImage(WINNAME, imgNew);
	}
	else if (event == CV_EVENT_RBUTTONDOWN)	
	{
		//当触发其他鼠标事件，则对骨骼缓存进行清空
		clkJoint.clear();

		if (lock_status == true && curPos != -1)
		{
			onMove = true;
			curJoint = curPos;
	//		cvCircle(imgNew, cvPoint(vTotal[curPos].x, vTotal[curPos].y), RADIUS, PCOLOR_UNLOCK, CV_FILLED, cv::LINE_8, 0);
		}
	}//右击，选择要拖动的节点

	if (onMove == true && curPos == curJoint)
	{
		DoMovement(x, y, curPos);
	}
}

void DoMovement(int xpos, int ypos, int curPos)
{
	if (curPos != -1)				
	{
		vTotal[curPos].x = xpos;
		vTotal[curPos].y = ypos;
	}//防止拖动过快curPos = -1

	redraw(curPos);
}

int isJoint(int x, int y)
{
	for (unsigned int i = 0; i < vTotal.size(); i++)
	{
		if ((vTotal[i].x - x) * (vTotal[i].x - x) + (vTotal[i].y - y) * (vTotal[i].y - y) < RADIUS * RADIUS)
		{
			return i;
		}
	}
	return -1;
}

int isBone(int p1, int p2)
{
	for (unsigned int i = 0; i < bTotal.size(); i++)
	{
		if (bTotal[i].p1 == p1 && bTotal[i].p2 == p2 || bTotal[i].p1 == p2 && bTotal[i].p2 == p1) return i;
	}
	return -1;
}


