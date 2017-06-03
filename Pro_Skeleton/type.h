#pragma once
#define RADIUS 5								//关节的半径
#define MAXNODE 1000							//关节点最大数目
#define PCOLOR cvScalar(255)					//关节点色彩
#define PCOLOR_UNLOCK cvScalar(0, 0, 255)		//被解锁的关节色彩
#define BCOLOR_ON cvScalar(255, 255, 255)		//上部骨骼色彩
#define BCOLOR_BOTTOM cvScalar(0)				//被挡住骨骼色彩
#define BLINE 2								    //上面部分骨骼线条
#define TRACKBARNAME "LockStatus"				//trackbar名字
#define WINNAME "Skeleton"						//窗口名字
#define ON true
#define BOTTOM false
#include<vector>
//关节
struct Point
{
	int x;
	int y;	
};

struct Bone
{
	int p1;
	int p2;
	bool status;						//状态,在上部-ON, 在下部-BOTTOM
};
