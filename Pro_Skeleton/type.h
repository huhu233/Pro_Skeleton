#pragma once
#define RADIUS 10						//�ؽڵİ뾶
#define MAXNODE 1000					//�ؽڵ������Ŀ
#define PCOLOR cvScalar(255)			//�ؽڵ�ɫ��
#define PCOLOR_UNLOCK cvScalar(0, 0, 255)     //�������Ĺؽ�ɫ��
#define BCOLOR cvScalar(0)				//����ɫ��
#define BLINE 2							//��������
#define TRACKBARNAME "LockStatus"		//trackbar����
#define WINNAME "Skeleton"				//��������

//�ؽ�
struct Point
{
	int x;
	int y;
};
