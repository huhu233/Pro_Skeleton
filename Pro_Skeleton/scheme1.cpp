/*
	ƽ̨:console����
	����:ʵ�ֹؽڵ���ƣ��϶�, ���ҿ���ѡȡ������ģ���ڵ���ϵ
	���룺������ʾ����
	trackbar:0���ɻ���״̬���޷���ק����������ͼƬ��������Ӧ�ĵ�
	trackbar:1, ���϶�״̬���޷����ƣ����һ��ؽڵ��ƶ����ɽ�����ק������̶������ѡ�й����������㣬������ɫ�ᷢ���仯
	�����������ʾ���棬�ļ�������ϵͳʱ������(Ĭ�ϱ����·����D:\project\outPut\*.jpg)
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
bool lock_status;						//������־��false-�ɻ��ƣ�true-����ק
bool onMove;							//��ק����
int curJoint;							//��¼ѡ�еĹؽڵ㣬��ʼֵΪ-1�����ƶ��ڵ�ʱ����ʹ�ã�
int vertexNum;							//�ؽڵ�����
int trackValue;

std::vector<Point> vTotal;				//�ؽ�
std::vector<Bone> bTotal;				//����
std::vector<int> tmp;
std::vector<int> clkJoint;				//ѡ�����ʱ�洢�����������ڵ�

//��ʼ��
void init();
//�����Ӧ�¼�
void onMouse(int event, int x, int y, int flags, void *param);
//�ж�����Ƿ��ڹؽڵ㴦
int isJoint(int x, int y);
//�ƶ��ؽڵ�
void DoMovement(int xpos, int ypos, int curPos);
//trackbar��Ӧ�¼�
void onChange(int pos);
//�ػ�Ǽ�
void redraw(int curPos);
//�ػ������������
void redrawBone();
//�ػ�ڵ�
void redrawPoint();
//�жϵ���������ܷ񹹳ɹ���(�����ù������ʱʹ��)
int isBone(int p1, int p2);


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
			cvNamedWindow(WINNAME, 1);
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
	//�ػ�ǰ�ͷ�֮ǰ��ͼƬ
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

//���ص��¼�
void onMouse(int event, int x, int y, int flags, void *param)
{
	int curPos = isJoint(x, y);
	if (event == CV_EVENT_LBUTTONUP)				//�����ͼ
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
			else//��ֹͬһ�����������洢���                                   
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
			}//���������������ĵ㣬����Ի�������

		}
		else                                              
		{
			if (onMove == true)//���أ�����onMove״̬
			{
				onMove = false;
				cvCircle(imgNew, cvPoint(vTotal[curJoint].x, vTotal[curJoint].y), RADIUS, PCOLOR, CV_FILLED, cv::LINE_8, 0);
			}
			else//ѡ�����
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
		//��������������¼�����Թ�������������
		clkJoint.clear();

		if (lock_status == true && curPos != -1)
		{
			onMove = true;
			curJoint = curPos;
	//		cvCircle(imgNew, cvPoint(vTotal[curPos].x, vTotal[curPos].y), RADIUS, PCOLOR_UNLOCK, CV_FILLED, cv::LINE_8, 0);
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
	}//��ֹ�϶�����curPos = -1

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


