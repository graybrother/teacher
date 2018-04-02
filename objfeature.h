#ifndef OBJFEATURE_H
#define OBJFEATURE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "opencv.hpp"

#define TRUSTTHRES  5
#define MAXOBJNUM   10
#define MOVENUM     8
#define LEASTLKPOINTS  20
#define LEASTMOVE  0.4
#define LEASTMOVEPOINTS 5
#define NOMATCHINDEX 50
#define NOMATCHTIMES 100
#define MAXTRUSTTIMES  20
#define NOTMOVECOUNT  30
#define SCREENTIME  50

#define SHIELDMARGIN 2

typedef struct object_Feature
{
    int objID;
    int trustedValue;
    int trustCount;
    int notmoveCount;
    bool isCurrentObj;
    bool Prestate_isCurrentObj;
    int noMatch;

   //for correspondent Rect from vibe
    int rectIndex;
    int screenIndex;  //overlap with a screenRange, -1 not overlaped
    //points for mediantrack
    std::vector<cv::Point2f> points[2];
    std::vector<cv::Point2f> initial;

    //data of tracking result
    int trackedPointNum;
    int lkMatchedNum;

    cv::Rect rect;
    cv::Rect initialRect;
   // cv::Point2f center;
    cv::Rect lkRect;
    int mvIndex;
    double objMvX[MOVENUM];
    double objMvY[MOVENUM];
    double moveX;    //filted objMvX
    double moveY;    //filted objMvY

    int overlapScreen;	//该目标是否和屏蔽区有交叉，完全不相交-1；和哪个屏蔽区相交，标几（0123）；初始-1//
    bool inScreen;		//不在屏蔽区内记-1；在第几个屏蔽区标几（0123）；初始-1	//

    int confirmAgain;
    int confirmCount;
    int confirmValue;

}object_Feature_t;

void object_Feature_Init(object_Feature_t &);
bool isMatchedRect(cv::Rect &,cv::Rect &);
bool isMatchedRectInitial(cv::Rect &rect, cv::Rect &initial);
bool isMatchedRectLK(cv::Rect &,cv::Rect &);
bool isMatchedNotMove(cv::Rect &,cv::Rect &);
bool isMatchedNotMove_1(cv::Rect &,cv::Rect &);
bool isSameRect(cv::Rect &,cv::Rect &);
bool isInRect(cv::Rect &rect, cv::Rect &target);
cv::Rect getRect(std::vector<cv::Point2f> &);
void BubbleSort(std::vector<cv::Rect> &, int );

bool IsInsideScreen(cv::Rect2i &screen, cv::Rect2i &a);

#endif // OBJFEATURE_H
