#include <time.h>

#include "objfeature.h"


void object_Feature_Init(object_Feature_t &obj)
{
  /* Model structure alloc. */
 // object_Feature_t *obj = NULL;
 // obj = (object_Feature_t *)calloc(1, sizeof(*obj));

  /* Default parameters values. */
  obj.trustedValue       = -1;
  obj.trustCount      =0;
  obj.notmoveCount    = 0;
  obj.isCurrentObj    = false;
  obj.noMatch         =-1;

  //
  obj.rectIndex          = -1;
  obj.screenIndex      = -1;
 //
 // obj.center=cv::Point2f(0,0);
  obj.rect=cv::Rect(0,0,0,0);
  obj.mvIndex=0;
  memset(obj.objMvX,0,sizeof(obj.objMvX));
  memset(obj.objMvY,0,sizeof(obj.objMvY));
  obj.moveX=0;
  obj.moveY=0;

  obj.points[0].clear();
  obj.points[1].clear();
  return;
}


bool isMatchedRect(cv::Rect &rect1,cv::Rect &rect2)
{
    cv::Rect rect=rect1 & rect2;
    cv::Rect minRect;
    if(rect1.area()<rect2.area())
        minRect=rect1;
    else
        minRect=rect2;
    return (rect.area()> minRect.area()/4);
}

bool isMatchedRectInitial(cv::Rect &rect,cv::Rect &initial)
{
    cv::Rect temp= rect & initial;

    return (temp.area()> initial.area()/10);
}

bool isMatchedRectLK(cv::Rect &lkrect,cv::Rect &vbrect)
{
    cv::Rect rect=lkrect & vbrect;
    return (rect.area()> lkrect.area()/5);
}

bool isMatchedNotMove(cv::Rect &notMoveRect,cv::Rect &vbrect)
{
    cv::Rect rect=notMoveRect & vbrect;
    cv::Rect minRect;
    if(notMoveRect.area()<vbrect.area())
        minRect=notMoveRect;
    else
        minRect=vbrect;
    return (rect.area()> minRect.area()/2);
}

bool isMatchedNotMove_1(cv::Rect &notMoveRect,cv::Rect &vbrect)
{
    cv::Rect rect=notMoveRect & vbrect;
    cv::Rect minRect;
    if(notMoveRect.area()<vbrect.area())
        minRect=notMoveRect;
    else
        minRect=vbrect;
    return (rect.area()> minRect.area()*4/5);
}
bool isInRect(cv::Rect &rect, cv::Rect &target)
{
    return(rect.x>=target.x && rect.y>=target.y
           && rect.width<=target.width && rect.height<=target.height);
}


bool isSameRect(cv::Rect &rect,cv::Rect &screenRange)
{
    cv::Rect rect1=rect & screenRange;
    cv::Rect rect2=rect |  screenRange;
    return ((rect2.area()-rect1.area())<3000);
}

cv::Rect getRect(std::vector<cv::Point2f> &point )
{
    int sumX=0;
    int sumY=0;
    int recleft=1920;
    int rectop=1080;
    int recright=0;
    int recbottom=0;

   for(int i = 0;i < point.size();i++){
         sumX=point[i].x;
         sumY=point[i].y;
         if(point[i].x<recleft)
              recleft=point[i].x;
         if(point[i].x>recright)
             recright=point[i].x;
         if(point[i].y<rectop)
              rectop=point[i].y;
         if(point[i].y> recbottom)
              recbottom=point[i].y;
    }
     return cv::Rect(recleft,rectop,recright-recleft,recbottom-rectop);
}

void BubbleSort(std::vector<cv::Rect> &array, int n)
{
    cv::Rect temp;
    for (int i=n-1;i>0;i--)
    {
        for (int j=0;j<i;j++)
        {
            if (array[j].x > array[j+1].x)
            {
                temp=array[j];
                array[j]=array[j+1];
                array[j+1]=temp;
            }
        }
    }
}
