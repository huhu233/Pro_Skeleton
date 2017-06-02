#pragma once
#define RADIUS 10						//关节的半径
#define MAXNODE 1000					//关节点最大数目
#define PCOLOR cvScalar(255)			//关节点色彩
#define PCOLOR_UNLOCK cvScalar(0, 0, 255)     //被解锁的关节色彩
#define BCOLOR cvScalar(0)				//骨骼色彩
#define BLINE 2							//骨骼线条
#define TRACKBARNAME "LockStatus"		//trackbar名字
#define WINNAME "Skeleton"				//窗口名字

//关节
struct Point
{
	int x;
	int y;
};
