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
#define LEASTLKPOINTS  10
#define LEASTMOVE  0.4
#define LEASTMOVEPOINTS 5
#define NOMATCHINDEX 50
#define NOMATCHTIMES 100
#define MAXTRUSTTIMES  20
#define NOTMOVECOUNT  30
#define SCREENTIME  50

#define SHIELDMARGIN 2

#define MINMATCHAREA 10

#define FBERROR 2

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

    cv::Rect2i rect;
    cv::Rect2i initialRect;
   // cv::Point2f center;
    cv::Rect2i lkRect;
    int mvIndex;
    float objMvX[MOVENUM];
    float objMvY[MOVENUM];
    float moveX;    //filted objMvX
    float moveY;    //filted objMvY

    int overlapScreen;	//该目标是否和屏蔽区有交叉，完全不相交-1；和哪个屏蔽区相交，标几（0123）；初始-1//
    bool inScreen;		//不在屏蔽区内记-1；在第几个屏蔽区标几（0123）；初始-1	//

    int confirmAgain;
    int confirmCount;
    int confirmValue;

}object_Feature_t;

void object_Feature_Init(object_Feature_t &);
bool isMatchedRect(cv::Rect2i &,cv::Rect2i &);
bool isMatchedRectInitial(cv::Rect2i &rect, cv::Rect2i &initial);
bool isMatchedRectLK(cv::Rect2i &,cv::Rect2i &);
bool isMatchedNotMove(cv::Rect2i &,cv::Rect2i &);
bool isMatchedNotMove_1(cv::Rect2i &,cv::Rect2i &);
bool isSameRect(cv::Rect2i &,cv::Rect2i &);
bool isInRect(cv::Rect2i &rect, cv::Rect2i &target);
cv::Rect2i getRect(std::vector<cv::Point2f> &);
void BubbleSort(std::vector<cv::Rect2i> &, int );

bool IsInsideScreen(cv::Rect2i &screen, cv::Rect2i &a);

int MatchedArea(cv::Rect2i &rect1,cv::Rect2i &rect2);

#endif // OBJFEATURE_H
