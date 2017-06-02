/*
	平台:console程序
	功能:实现关节点绘制，拖动
	根据提示输入
	trackbar:0，可绘制状态（无法拖拽），左键点击图片会生成相应的点
	trackbar:1, 可拖动状态（无法绘制），右击关节点移动鼠标可进行拖拽，左击固定
	根据提示保存，文件名按照系统时间命名(默认保存的路径是D:\project\outPut\.jpg)
*/

#include<Windows.h>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv/cv.hpp>
#include<vector>
#include<cstring>
#include<cstdio>
#include<string>
#include<cmath>
#include"type.h"

std::string outputPath = "D:\\project\\outPut\\";

IplImage *img = NULL;
IplImage *imgNew = NULL;
int curJoint;							//记录选中的关节点，初始值为-1
int vertexNum;
int trackValue;
int pre;
std::vector<Point> vTotal;				//关节
std::vector<int> Graph[MAXNODE];		//骨架邻接图
std::vector<int> tmp;
bool visit[MAXNODE];					//访问标记
bool lock_status		;				//锁定标志：false-可绘制，true-可拖拽
bool onMove;							//拖拽开关


//鼠标相应事件
void onMouse(int event, int x, int y, int flags, void *param);
//判断鼠标是否处于关节点处
int isJoint(int x, int y);
//移动关节点
void DoMovement(int xpos, int ypos, int curPos);
//trackbar响应事件
void onChange(int pos);
//重绘骨架
void reDraw();
//DFS遍历绘图
void DFSRedraw(int u);
void reset();
void init();

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
			cvNamedWindow(WINNAME, 0);
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
	for (int i = 0; i < MAXNODE; i++)
	{
		if (Graph[i].size() != 0) Graph[i].clear();
	}
	std::fill(visit, visit + MAXNODE, false);
	vTotal.clear();
	tmp.clear();
	curJoint = -1;
	pre = -1;
	vertexNum = 0;
	lock_status = false;
	onMove = false;
}

//重置访问标记
void reset()
{
	std::fill(visit, visit + MAXNODE, false);
}

void onChange(int pos)
{
	if (pos == 0) lock_status = false;
	else lock_status = true;
}

void reDraw()
{
	//重绘之前释放之前的图片
	if (imgNew != NULL)
	{
		cvReleaseImage(&imgNew);
		imgNew = NULL;
	}

	imgNew = (IplImage*)cvClone(img);
	reset();
	for (unsigned int i = 0; i < vTotal.size(); i++)
	{
		pre = -1;
		if (visit[i] == false)
		{
			DFSRedraw(i);
		}
	}
	cvShowImage(WINNAME, imgNew);
}

void DFSRedraw(int u)
{
	cvCircle(imgNew, cvPoint(vTotal[u].x, vTotal[u].y), RADIUS, PCOLOR, CV_FILLED, CV_AA, 0);
	if (pre != -1)
	{
		cvLine(imgNew, cvPoint(vTotal[pre].x, vTotal[pre].y), cvPoint(vTotal[u].x, vTotal[u].y), BCOLOR, BLINE, cv::LINE_8, 0);
	}
	for (unsigned int i = 0; i < Graph[u].size(); i++)
	{
		int v = Graph[u][i];
		if (v != pre)
		{
			pre = u;			
			DFSRedraw(v);
		}
	}
}

//鼠标回调事件
void onMouse(int event, int x, int y, int flags, void *param)
{
	int curPos = isJoint(x, y);
	if (event == CV_EVENT_LBUTTONUP)				//左击绘图
	{
		if (!lock_status)
		{
			if (curPos == -1)
			{
				Point pt;
				pt.x = x;
				pt.y = y;
				vTotal.push_back(pt);
				cvCircle(imgNew, cvPoint(x, y), RADIUS, PCOLOR, CV_FILLED, CV_AA, 0);
				tmp.push_back(vertexNum);
				vertexNum++;
			}
			else                                    
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
			}//同一个点只存储一遍

			if (tmp.size() == 2)
			{
				Graph[tmp[0]].push_back(tmp[1]);
				cvLine(imgNew, cvPoint(vTotal[tmp[0]].x, vTotal[tmp[0]].y), cvPoint(vTotal[tmp[1]].x, vTotal[tmp[1]].y), BCOLOR, BLINE, cv::LINE_8, 0);
				tmp.clear();
			}//如果存在两个相异的点，则可以绘制线条
		}
		else                                              
		{
			if (onMove == true) onMove = false;
		}//开关，设置onMove状态

		cvShowImage(WINNAME, imgNew);
	}
	else if (event == CV_EVENT_RBUTTONDOWN)	
	{
		if (lock_status == true && curPos != -1)
		{
			onMove = true;
			curJoint = curPos;
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
	}//防止拖动过快出现异常

	reDraw();
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
