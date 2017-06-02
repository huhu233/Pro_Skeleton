//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui.hpp>
//#include<opencv/cv.hpp>
//#include<vector>
//#include<iostream>
//#include<cmath>
//#include"type.h"
//#define TRACKBARNAME "radius"
//#define WINNAME "Skeleton"
//using namespace std;
//
//IplImage *img = NULL;
//IplImage *imgNew = NULL;
//int unlockJoint = -1;					//��¼��ǰ�����ؽڵ㣬��ʼֵΪ-1
//int vertexNum = 0;
//int trackValue;
//int trackPrePos = 0;
//int pre;
////stack< IplImage* > s_back;
//vector<Point> vTotal;
//vector<Point> vTotalDump;
////vector<Line> lTotal;
//vector<int> Graph[MAXNODE];
//vector<int> tmp;
//bool visit[MAXNODE] = { false };
//bool lock_status = true;				//������־,��ʼֵΪfalse
//
//
//void onMouse(int event, int x, int y, int flags, void *param);
//int lockJoint(int x, int y);
//void DoMovement();
//void onChange(int pos);
//void reDraw();
//void DFSUpdate(int u, int pos, int deltaX, int deltaY);
//void DFSRedraw(int u);
//
//int main()
//{
//	img = cvLoadImage("person2.jpg", 1);
//	imgNew = (IplImage*)cvClone(img);
////	img = cvCreateImage(cvSize(imgCover->width, imgCover->height), imgCover->depth, imgCover->nChannels);
//	cvNamedWindow("Skeleton", 0);
//	cvCreateTrackbar(TRACKBARNAME, "Skeleton", &trackValue, 360, onChange);
//	cvShowImage("Skeleton", imgNew);
//	cvSetMouseCallback("Skeleton", onMouse, 0);
//
//	cvWaitKey(0);
//
//	if (imgNew != NULL)
//	{
//		cvReleaseImage(&imgNew);
//		imgNew = NULL;
//	}
//	if (img != NULL)
//	{
//		cvReleaseImage(&img);
//	}
//	cvDestroyWindow("Skeleton");
//
//}
//
//void init()
//{
//	fill(visit, visit + MAXNODE, false);
//}
//
//
//void onChange(int pos)
//{
//	cout << pos << endl;
//	if (lock_status == false)			//���δ�������޷���ת
//	{
//		init();
//		DFSUpdate(unlockJoint, pos, vTotal[unlockJoint].x, vTotal[unlockJoint].y);
//		if (imgNew != NULL)
//		{
//			cvReleaseImage(&imgNew);
//			imgNew = NULL;
//		}
//		reDraw();
////		trackPrePos = pos;				//��¼֮ǰ��λ��
//		vTotal = vTotalDump;
//	}
//
//}
//////////////////////���ھ������⣬������ת�����лᷢ�����Σ���ʱ��֪����ν��
//void DFSUpdate(int u, int pos, int deltaX, int deltaY)
//{
//	visit[u] = true;
//	vTotal[u].x = (vTotal[u].x - deltaX) * cos(pos * UNIT) - (vTotal[u].y - deltaY) * sin(pos * UNIT) + deltaX;			//������ת�������
//	vTotal[u].y = (vTotal[u].x - deltaX) * sin(pos * UNIT) + (vTotal[u].y - deltaY) * cos(pos * UNIT) + deltaY;			//������ת�������
//
//	cout << u << "-" << vTotal[u].x << "," << vTotal[u].y << ":" << sqrt((double)pow(vTotal[u].x - deltaX, 2.0) + pow((double)vTotal[u].y - deltaY, 2.0)) <<endl;
//	for (int i = 0; i < Graph[u].size(); i++)
//	{
//		int v = Graph[u][i];
//		if (visit[v] == false)
//		{
//			DFSUpdate(v, pos, deltaX, deltaY);
//		}
//	}
//}
//
//void reDraw()
//{
//	imgNew = (IplImage*)cvClone(img);
//	init();
//	for (int i = 0; i < vTotal.size(); i++)
//	{
//		pre = -1;
//		if (visit[i] == false)
//		{
//			DFSRedraw(i);
//		}
//	}
//
//	cvCircle(imgNew, cvPoint(vTotal[unlockJoint].x, vTotal[unlockJoint].y), RADIUS, cvScalar(UNLOCK), CV_FILLED, CV_AA, 0);
//	cvShowImage(WINNAME, imgNew);
//}
//
//
//void DFSRedraw(int u)
//{
//	visit[u] = true;
////	cout << vTotal[u].x << "," << vTotal[u].y << endl;
//	cvCircle(imgNew, cvPoint(vTotal[u].x, vTotal[u].y), RADIUS, cvScalar(255), CV_FILLED, CV_AA, 0);
//	if (pre != -1)
//	{
//		cvLine(imgNew, cvPoint(vTotal[pre].x, vTotal[pre].y), cvPoint(vTotal[u].x, vTotal[u].y), cvScalar(0), 2, cv::LINE_4, 0);
//	}
//	pre = u;
//	for (int i = 0; i < Graph[u].size(); i++)
//	{
//		int v = Graph[u][i];
//		if (visit[v] == false) DFSRedraw(v);
//	}
//}
//
////���ص��¼�
//void onMouse(int event, int x, int y, int flags, void *param)
//{
//	int curPos = lockJoint(x, y);	
//	if (event == CV_EVENT_LBUTTONUP)				//���������ƺ��϶�
//	{
//		if (lock_status)							//δ�н������������������ƣ���������϶�
//		{
//			if (curPos == -1)			
//			{
//				Point pt;
//				pt.x = x;
//				pt.y = y;
//				pt.number = vertexNum;
//				vTotal.push_back(pt);
//				vTotalDump.push_back(pt);			//vTotal�ı���
//				cvCircle(imgNew, cvPoint(x, y), RADIUS, cvScalar(LOCK, 0, 0, 0), CV_FILLED, CV_AA, 0);
//				tmp.push_back(vertexNum);
//				vertexNum++;
//			}
//			else                                   //ͬһ����ֻ�洢һ��
//			{
//				int i;
//				for (i = 0; i < tmp.size(); i++)
//				{
//					if (tmp[i] == curPos) break;
//				}
//				if (i == tmp.size())
//				{
//					tmp.push_back(curPos);
//				}
//			}
//			//���������������ĵ㣬����Ի�������
//			if (tmp.size() == 2)
//			{
//				Line l;
//				l.pt1 = tmp[0];
//				l.pt2 = tmp[1];
//				Graph[tmp[0]].push_back(tmp[1]);
//				cvLine(imgNew, cvPoint(vTotal[tmp[0]].x, vTotal[tmp[0]].y), cvPoint(vTotal[tmp[1]].x, vTotal[tmp[1]].y), cvScalar(0), 2, cv::LINE_AA, 0);
//				tmp.clear();
//			}
//		}							
//	}
//	else if (event == CV_EVENT_RBUTTONUP)			//�һ���������ؽں������ؽ�
//	{
//		if (curPos != -1)
//		{	
//			if (lock_status == true)				//���δ�н������������һ���ڵ�
//			{
//				lock_status = false;
//				unlockJoint = curPos;
//				cvCircle(imgNew, cvPoint(vTotal[curPos].x, vTotal[curPos].y), RADIUS, cvScalar(UNLOCK, 0, 0, 0), CV_FILLED, CV_AA, 0);
//			}
//			else if (curPos == unlockJoint)			//�����ٲ鿴��ǰ�ڵ��Ƿ�Ϊ�Ѿ������Ľ�㣬��-������������-����Ӧ
//			{
//				lock_status = true;
//				unlockJoint = -1;
//				cvCircle(imgNew, cvPoint(vTotal[curPos].x, vTotal[curPos].y), RADIUS, cvScalar(LOCK, 0, 0, 0), CV_FILLED, CV_AA, 0);
//			}
//		}
//	}
//
//	cvShowImage("Skeleton", imgNew);				//ˢ�½��
//}
//
//
//int lockJoint(int x, int y)
//{
//	for (unsigned int i = 0; i < vTotal.size(); i++)
//	{
//		if ((vTotal[i].x - x) * (vTotal[i].x - x) + (vTotal[i].y - y) * (vTotal[i].y - y) < RADIUS * RADIUS)
//		{
//			return i;
//		}
//	}
//	return -1;
//}
