#pragma once
#define RADIUS 5								//�ؽڵİ뾶
#define MAXNODE 1000							//�ؽڵ������Ŀ
#define PCOLOR cvScalar(255)					//�ؽڵ�ɫ��
#define PCOLOR_UNLOCK cvScalar(0, 0, 255)		//�������Ĺؽ�ɫ��
#define BCOLOR_ON cvScalar(255, 255, 255)		//�ϲ�����ɫ��
#define BCOLOR_BOTTOM cvScalar(0)				//����ס����ɫ��
#define BLINE 2								    //���沿�ֹ�������
#define TRACKBARNAME "LockStatus"				//trackbar����
#define WINNAME "Skeleton"						//��������
#define ON true
#define BOTTOM false
#include<vector>
//�ؽ�
struct Point
{
	int x;
	int y;	
};

struct Bone
{
	int p1;
	int p2;
	bool status;						//״̬,���ϲ�-ON, ���²�-BOTTOM
};
