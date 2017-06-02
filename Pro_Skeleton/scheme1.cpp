/*
	ƽ̨:console����
	����:ʵ�ֹؽڵ���ƣ��϶�
	������ʾ����
	trackbar:0���ɻ���״̬���޷���ק����������ͼƬ��������Ӧ�ĵ�
	trackbar:1, ���϶�״̬���޷����ƣ����һ��ؽڵ��ƶ����ɽ�����ק������̶�
	������ʾ���棬�ļ�������ϵͳʱ������(Ĭ�ϱ����·����D:\project\outPut\.jpg)
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
int curJoint;							//��¼ѡ�еĹؽڵ㣬��ʼֵΪ-1
int vertexNum;
int trackValue;
int pre;
std::vector<Point> vTotal;				//�ؽ�
std::vector<int> Graph[MAXNODE];		//�Ǽ��ڽ�ͼ
std::vector<int> tmp;
bool visit[MAXNODE];					//���ʱ��
bool lock_status		;				//������־��false-�ɻ��ƣ�true-����ק
bool onMove;							//��ק����


//�����Ӧ�¼�
void onMouse(int event, int x, int y, int flags, void *param);
//�ж�����Ƿ��ڹؽڵ㴦
int isJoint(int x, int y);
//�ƶ��ؽڵ�
void DoMovement(int xpos, int ypos, int curPos);
//trackbar��Ӧ�¼�
void onChange(int pos);
//�ػ�Ǽ�
void reDraw();
//DFS������ͼ
void DFSRedraw(int u);
void reset();
void init();

int main()
{
	int choice, save;
	char str[100];
	printf("������ѡ��0-������1-�༭: ");
	scanf("%d", &choice);
	while (choice)
	{
		system("cls");
		init();

		printf("��������Ƭ·��: ");
		scanf("%s", str);
		img = cvLoadImage(str, 1);
		if (img != NULL)						//ȷ��ͼ��·����ȷ
		{
			imgNew = (IplImage*)cvClone(img);
			cvNamedWindow(WINNAME, 0);
			cvCreateTrackbar(TRACKBARNAME, WINNAME, &trackValue, 1, onChange);
			cvShowImage(WINNAME, imgNew);
			cvSetMouseCallback(WINNAME, onMouse, 0);
			cvWaitKey(0);
			//����ͼƬѡ��
			printf("�Ƿ񱣴棿0-��1-��: ");
			scanf("%d", &save);
			if (save)
			{
				SYSTEMTIME st;
				char imgName[100];

				GetLocalTime(&st);
				sprintf(imgName, "%02d%02d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				cvSaveImage((outputPath + imgName + ".jpg").c_str(), imgNew);
				printf("�ѱ���\n");
			}
			else
			{
				printf("\aδ����!\n");
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
			printf("\a·������\n");
		}

		printf("���������ѡ��:0-�˳���1-�����༭: ");
		scanf("%d", &choice);
		if (!choice) printf("�����˳�...");
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

//���÷��ʱ��
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
	//�ػ�֮ǰ�ͷ�֮ǰ��ͼƬ
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

//���ص��¼�
void onMouse(int event, int x, int y, int flags, void *param)
{
	int curPos = isJoint(x, y);
	if (event == CV_EVENT_LBUTTONUP)				//�����ͼ
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
			}//ͬһ����ֻ�洢һ��

			if (tmp.size() == 2)
			{
				Graph[tmp[0]].push_back(tmp[1]);
				cvLine(imgNew, cvPoint(vTotal[tmp[0]].x, vTotal[tmp[0]].y), cvPoint(vTotal[tmp[1]].x, vTotal[tmp[1]].y), BCOLOR, BLINE, cv::LINE_8, 0);
				tmp.clear();
			}//���������������ĵ㣬����Ի�������
		}
		else                                              
		{
			if (onMove == true) onMove = false;
		}//���أ�����onMove״̬

		cvShowImage(WINNAME, imgNew);
	}
	else if (event == CV_EVENT_RBUTTONDOWN)	
	{
		if (lock_status == true && curPos != -1)
		{
			onMove = true;
			curJoint = curPos;
		}
	}//�һ���ѡ��Ҫ�϶��Ľڵ�

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
	}//��ֹ�϶���������쳣

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
