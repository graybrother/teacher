#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sys/time.h"

#include "vibe-background-sequential.h"
#include "objfeature.h"

#define SKIN 0
#define FRAMEINTERV 3

enum{ NOT_SET = 0, IN_PROCESS = 1, SET = 2 ,EXIT =3 };
cv::VideoCapture capture;
cv::String inputWindow="inputWindow";
cv::Mat inputFrame;
bool roiState=false;
bool maskState1=false;
bool maskState2=false;

cv::Rect2i roiRect;
cv::Rect2i maskRect1;
cv::Rect2i maskRect2;

int rectState=NOT_SET;
cv::Point2i startP;





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void nothing_on_mouse( int event, int x, int y, int flags, void* para )
{
    //nothing to do
}

void roi_on_mouse( int event, int x, int y, int flags, void* para )
{
    // TODO add bad args check
    using namespace cv;

    switch( event )
    {
    case EVENT_LBUTTONDOWN: // set rect or GC_BGD(GC_FGD) labels
        {
                rectState = IN_PROCESS;
                roiRect = cv::Rect( x, y, 1, 1 );


        }
        break;
    case EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels
        {

        }
        break;
    case EVENT_LBUTTONUP:
        if( rectState == IN_PROCESS )
        {
            roiRect =cv::Rect( cv::Point(roiRect.x, roiRect.y), cv::Point(x,y) );
            rectState = SET;

        }

        break;
    case EVENT_RBUTTONUP:

        {
            rectState=EXIT;
        }
        break;
    case EVENT_MOUSEMOVE:
        if( rectState == IN_PROCESS )
        {
            roiRect = cv::Rect( cv::Point(roiRect.x, roiRect.y), cv::Point(x,y) );

        }

        break;
    }

}

void mask1_on_mouse( int event, int x, int y, int flags, void* para )
{
    // TODO add bad args check
    using namespace cv;
    switch( event )
    {
    case EVENT_LBUTTONDOWN: // set rect or GC_BGD(GC_FGD) labels
        {
                rectState = IN_PROCESS;
                maskRect1 = cv::Rect( x, y, 1, 1 );


        }
        break;
    case EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels
        {

        }
        break;
    case EVENT_LBUTTONUP:
        if( rectState == IN_PROCESS )
        {
            maskRect1 =cv::Rect( cv::Point(maskRect1.x, maskRect1.y), cv::Point(x,y) );
            rectState = SET;

        }

        break;
    case EVENT_RBUTTONUP:

        {
            rectState=EXIT;
        }
        break;
    case EVENT_MOUSEMOVE:
        if( rectState == IN_PROCESS )
        {
            maskRect1 = cv::Rect( cv::Point(maskRect1.x, maskRect1.y), cv::Point(x,y) );

        }

        break;
    }
}

void mask2_on_mouse( int event, int x, int y, int flags, void* para )
{
    // TODO add bad args check
    using namespace cv;
    switch( event )
    {
    case EVENT_LBUTTONDOWN: // set rect or GC_BGD(GC_FGD) labels
        {
                rectState = IN_PROCESS;
                maskRect2 = cv::Rect( x, y, 1, 1 );


        }
        break;
    case EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels
        {

        }
        break;
    case EVENT_LBUTTONUP:
        if( rectState == IN_PROCESS )
        {
            maskRect2 =cv::Rect( cv::Point(maskRect2.x, maskRect2.y), cv::Point(x,y) );
            rectState = SET;

        }

        break;
    case EVENT_RBUTTONUP:

        {
            rectState=EXIT;
        }
        break;
    case EVENT_MOUSEMOVE:
        if( rectState == IN_PROCESS )
        {
            maskRect2 = cv::Rect( cv::Point(maskRect2.x, maskRect2.y), cv::Point(x,y) );

        }

        break;
    }
}

void MainWindow::on_lkvb2_pushButton_clicked()
{
    using namespace std;
    using namespace cv;

    if(!capture.isOpened())
    {
        ui->warning_lineEdit->setText("Please open a video file first");
        return ;
    }
    if (!capture.read(inputFrame)) {
        ui->warning_lineEdit->setText("Video read error");
        capture.release();

        cv::destroyAllWindows();
        return;
    }
    int left;  //60;
    int top;     //90;
    int width;   //350;
    int height;    //85;

    if(roiState)
    {
        left=roiRect.x;  //60;
        top=roiRect.y;     //90;
        width=roiRect.width;   //350;
        height=roiRect.height;    //85;
    }
    else
    {
        left=0; top=0;
        width=inputFrame.cols;
        height=inputFrame.rows;
    }
    int CENTERx=width/2;
    int CENTERy=height/2;
    int outputCenterx=left+width/2;
    int outputCentery=top+height/2;
    Rect2i processRange(left,top,width,height);

    bool haveScreenRange=false;
    int screenLeft;  //215-left
    int screenTop;  //130-top;
    int screenWidth;   //150;
    int screenHeight;   //75;
    vector<Rect2i> screenRange;

    int insideScreenRange=false;
    int isOverlap=false;
    int overlapScreenID=-1;
    int screenTime=0;

    if(maskState1)
    {
        screenLeft=maskRect1.x-left;
        if(screenLeft<0)
        {
            screenLeft=0;
            maskRect1.width=maskRect1.x+maskRect1.width-left;
        }
        screenTop=maskRect1.y-top;  //130-top;
        if(screenTop<0)
        {
            screenTop=0;
            maskRect1.height=maskRect1.y+maskRect1.height-top;
        }
        screenWidth=maskRect1.width;   //150;
        if(screenWidth>width)
            screenWidth=width;
        screenHeight=maskRect1.height;   //75;
        if(screenHeight>height)
            screenHeight=height;
        Rect2i ts_rect=cv::Rect(screenLeft,screenTop,screenWidth,screenHeight);
        screenRange.push_back(ts_rect);

    }
    if(maskState2)
    {
        screenLeft=maskRect2.x-left;
        if(screenLeft<0)
        {
            screenLeft=0;
            maskRect2.width=maskRect2.x+maskRect2.width-left;
        }
        screenTop=maskRect2.y-top;  //130-top;
        if(screenTop<0)
        {
            screenTop=0;
            maskRect2.height=maskRect2.y+maskRect2.height-top;
        }
        screenWidth=maskRect2.width;   //150;
        if(screenWidth>width)
            screenWidth=width;
        screenHeight=maskRect2.height;   //75;
        if(screenHeight>height)
            screenHeight=height;
        Rect2i ts_rect=cv::Rect(screenLeft,screenTop,screenWidth,screenHeight);
        screenRange.push_back(ts_rect);

    }
    int Num_shield=screenRange.size();
    if(Num_shield>0)
         haveScreenRange=true;
    static int frameNumber = 1; /* The current frame number */
  //  Mat inputFrame;
    Mat colorFrame;
    Mat faceFrame;
    Mat frame;                  /* Current gray frame. */
    Mat frame_prev; // previous gray-level image
    Mat segmentationMap;        /* Will contain the segmentation map. This is the binary output map. */
    Mat updateMap;
    Mat showrectMap;
    int keyboard = 0;           /* Input from keyboard. Used to stop the program. Enter 'q' to quit. */
    int k;
    int i,j,index,disToCenter;
    Mat element(5,5,CV_8U,cv::Scalar(1));

    vector<vector<Point> > contours;
    vector<Rect> recNoSort;
    vector<Rect> rects;
    int rectsNum=0;
    vector<Rect> notMoveRect;
    notMoveRect.clear();
    int notMoveRectNum=0;


    int xxx;
    Rect rectemp;
    Rect recpre;
    Rect vbtemp;


//       Rect maxrect;


    Point2f p;
    Point2f centerPoint;
    uint8_t *segTemp;

    int matched[MAXOBJNUM];
    // tracked features from 0->1
//    std::vector<cv::Point2f> points[2];
    // initial position of tracked points
//    std::vector<cv::Point2f> initial;
 //   std::vector<cv::Point2f> grid; // created features

    std::vector<uchar> status; // status of tracked features
    std::vector<float> err; // error in tracking

    object_Feature_t objFeature[MAXOBJNUM];
   // object_Feature_t* objTemp;
    int matchedNum= 0;
    int vbNum=0;
    int objNum=     0;
    int currentID=  -1;
    int objJointed=0;
    int objSplited=0;
    int jointedIndex=-1;
    int currentDirection=-1;
    int jointedDirection=-1;
    int noObjCount=0;

    bool haveMatchedRect=false;
    bool matchNoMove1=false;
    bool matchNoMove2=false;
    bool splitChooseOK=false;

    int pointNum=0;
    int moveNum=0;
    double xmove=0;
    double ymove=0;
    double sumXmove=0;
    double sumYmove=0;
    double meanMove=0;

    int lkLeft,lkRight,lkTop,lkBottom;
    int candoLK;

    // output parameters
    Rect outputRect(outputCenterx-20,outputCentery-30,40,60);
    int  outputTimeout=0;
    int outputObjNum=0;


    int imin=40;
    int imax=230;
    Mat lut(1,256,CV_8U);
    Mat afterLut;

    for(int i=0;i<256;i++)
    {
        if(i<imin)
            lut.at<uchar>(i)=0;
        else if(i>imax)
            lut.at<uchar>(i)=255;
        else
            lut.at<uchar>(i)=static_cast<uchar>(
                        255.0*(i-imin)/(imax-imin)+0.5);

    }


    //-----------------------测试时间
    struct timeval tsBegin,tsEnd;
    long runtimes;


    for(i=0; i< MAXOBJNUM; i++) {
        object_Feature_Init(objFeature[i]);
    }
    QString  ipaddress;


   //  filename = QFileDialog::getOpenFileName(this, tr("select Video file"), ".",
   //                                             tr("Video Files(*.mp4 *.avi *.mkv *.yuv)"));

        if (filename.isEmpty())
        {
            filename=ui->ip_lineEdit->text();
            if(filename.isEmpty())
            {
                ui->warning_lineEdit->setText("Filename is empty,please open a file or input URL");
                return;
            }
        }
  //  filename="rtsp://192.168.100.182/2";

      VideoCapture capture(filename.toStdString());
 //   VideoCapture capture("rtsp://192.168.100.182/2");
    if(!capture.isOpened()){
        ui->warning_lineEdit->setText("File open error,please open video file first");
        return ;
    }
    namedWindow("Frame");
   // namedWindow("Gray");
    namedWindow("ShowRect");
    namedWindow("Segmentation by ViBe");
    namedWindow("Afterdilate");
    namedWindow("Tracking");
#if SKIN
    namedWindow("faceFrame");
#endif
    /* Model for ViBe. */
    vibeModel_Sequential_t *model = NULL; /* Model used by ViBe. */

    frameNumber=1;
    int frameInterV=0;

    while ((char)keyboard != 'q' && (char)keyboard != 27) {
      /* Read the current frame. */
      if (!capture.read(inputFrame)) {
          ui->warning_lineEdit->setText("end of File ");
          capture.release();

          /* Frees the model. */
          libvibeModel_Sequential_Free(model);
          destroyAllWindows();
          return;
      }

       Mat roiframe= inputFrame(processRange);
       roiframe.copyTo(colorFrame);
       roiframe.copyTo(showrectMap);
   //    roiframe.copyTo(faceFrame);
      //show the current frame
   //   imshow("Frame", inputFrame);
      //convert to gray
      cvtColor(colorFrame, frame, CV_BGR2GRAY);

   //   imshow("Gray",frame);

 //      cv::LUT(frame,lut,afterLut);

  //     imshow("afterLut",afterLut);

    //   afterLut.copyTo(frame);
    // Applying ViBe.
      if (frameNumber == 1) {
        segmentationMap = Mat(frame.rows, frame.cols, CV_8UC1);
        model = (vibeModel_Sequential_t*)libvibeModel_Sequential_New();
        libvibeModel_Sequential_AllocInit_8u_C1R(model, frame.data, frame.cols, frame.rows);
      }

      /* ViBe: Segmentation and updating. */
      //give some time for background building
      if (frameNumber < 10){
        libvibeModel_Sequential_Segmentation_8u_C1R(model, frame.data, segmentationMap.data);
        libvibeModel_Sequential_Update_8u_C1R(model, frame.data, segmentationMap.data);
        ++frameNumber;
        continue;
      }
      if(frameNumber>10000000)
          frameNumber=100;

      if(frameInterV==FRAMEINTERV)
          frameInterV=0;
      if(frameInterV>0)
      {
          frameNumber++;
          frameInterV++;
          continue;
      }
      frameInterV++;
      ++frameNumber;

      // for first image of the sequence
      if(frame_prev.empty())
        frame.copyTo(frame_prev);

     //test the time
      gettimeofday(&tsBegin, NULL);

// **ViBe: Segmentation..............

      libvibeModel_Sequential_Segmentation_8u_C1R(model, frame.data, segmentationMap.data);

      gettimeofday(&tsEnd, NULL);//-----------------------测试时间

      runtimes=1000000L*(tsEnd.tv_sec-tsBegin.tv_sec)+tsEnd.tv_usec-tsBegin.tv_usec;
      cout<<" vibe segmentation time: "<<runtimes<<endl;

      imshow("Segmentation by ViBe",segmentationMap);

      segmentationMap.copyTo(updateMap);

// **Erode,Dilate and CCL..................
      cv::erode(segmentationMap,segmentationMap,cv::Mat());
      cv::dilate(segmentationMap,segmentationMap,element,Point(-1,-1),7);
      cv::erode(segmentationMap,segmentationMap,cv::Mat(),Point(-1,-1),10);

      //Extract the contours so that
      cv::findContours( segmentationMap, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


      rectsNum=0;
      recNoSort.clear();
      rects.clear();

//       recpre=Rect(1920,1080,10,10);
      for(k = 0; k < contours.size(); k++ ){
         rectemp=boundingRect(contours[k]);
         cv::rectangle(segmentationMap, rectemp, cv::Scalar(128), 4);
         //rect is the same of screenrange,let it go
//         if(haveScreenRange && isSameRect(rectemp,screenRange))
//             continue;
         if(rectemp.area()>300 && rectemp.area()<50000
                 && rectemp.width>5 && rectemp.height>5
                 && rectemp.width<300 && rectemp.height<155){

              recNoSort.push_back(rectemp);
              rectsNum++;
         }

      } //end of for(k<contours.size()

// **Rect process: sort, conbine................
      if(rectsNum>0)
      {
          //sort the rects
          BubbleSort(recNoSort,rectsNum);

          // conbine some rects
          rects.clear();
          rects.push_back(recNoSort[0]);
          recpre=rects[0];
          i=1;
          for(k=1; k<rectsNum; k++){
              rectemp=recNoSort[k];
              //all in
              if((rectemp.x+rectemp.width)<=(recpre.x+recpre.width)){
                  // some rect splited to up and down two parts
                  // just joint them together
                  rectemp=rectemp | recpre;
                  rects.pop_back();
                  rects.push_back(rectemp);
                  recpre=rectemp;
                  cout<<"rect joined "<<recpre.x<<"   "<<rectemp.x<<endl;

              }
              else if((rectemp.x+rectemp.width/2)< (recpre.x+recpre.width-rectemp.width/5))
              {
                  rectemp=rectemp | recpre;
                  rects.pop_back();
                  rects.push_back(rectemp);
                  recpre=rectemp;
                  cout<<"rect joined "<<recpre.x<<"   "<<rectemp.x<<endl;

              }
              else if((recpre.x+recpre.width/2)> (rectemp.x+recpre.width/5))
              {
                  rectemp=rectemp | recpre;
                  rects.pop_back();
                  rects.push_back(rectemp);
                  recpre=rectemp;
                  cout<<"rect joined "<<recpre.x<<"   "<<rectemp.x<<endl;

              }
              else
              {
                  rects.push_back(rectemp);
                  recpre=rectemp;
                  i++;
              }
          }
          rectsNum=i;
      }

      cout<<"rects   " <<rectsNum<<endl;
      for(k=0; k<rectsNum; k++){
      cv::rectangle(segmentationMap, rects[k], cv::Scalar(255), 2);
      }


     if(rectsNum>MAXOBJNUM)
          rectsNum=MAXOBJNUM;

// **NotMoveRect process......................
     /*aaa*/  //detect if not move rect still there
     cout<<" notMoveRect="<<notMoveRectNum<<endl;
     pointNum=0;
     for(i=0; i<notMoveRectNum; i++)
     {
         for(k=0; k<rectsNum; k++)
         {
            if(isMatchedNotMove(notMoveRect[i],rects[k]))
            {
               rectemp=notMoveRect[i] & rects[k];
               notMoveRect[pointNum]=rectemp;
               pointNum++;
               cv::rectangle(showrectMap, notMoveRect[i], cv::Scalar(0,255,255), 4);
               break;
            }
         }
     }
     notMoveRectNum=pointNum;
     notMoveRect.resize(notMoveRectNum);

     //merge insided not move rect
     if(notMoveRectNum>1)
     {
         BubbleSort(notMoveRect,notMoveRectNum);
         recpre=notMoveRect[0];
         k=1;
         for(i=1; i<notMoveRectNum; i++)
         {
             rectemp=notMoveRect[i];
             if(isInRect(rectemp,recpre)||isInRect(recpre,rectemp))
             {
                 rectemp=rectemp | recpre;
                 notMoveRect[k-1]=rectemp;
                 recpre=rectemp;
             }
             else
             {
                 notMoveRect[k]=rectemp;
                 recpre=rectemp;
                 k++;
             }
         }
         notMoveRectNum=k;
         notMoveRect.resize(notMoveRectNum);
     }


     for(i=0; i<MAXOBJNUM; i++)
     {
         if(objFeature[i].rectIndex>-1)
         {
            cv::rectangle(showrectMap, objFeature[i].rect,
                     cv::Scalar(255,0,0), 1);
         }
     }

     imshow("Afterdilate", segmentationMap);
     imshow("ShowRect", showrectMap);

// ** Match vb-rect with obj............................
      matchedNum=0;
      for(k=0;k<rectsNum; k++)
      {
          matched[k]=0;
      }

      //1.first to match the currentobj

      haveMatchedRect=false;
      if(currentID>-1){
          for(k=0; k<rectsNum; k++)
          {
              if(isMatchedRectLK(objFeature[currentID].lkRect,rects[k]))
              {
                  objFeature[currentID].rectIndex=k;
                  // set after process join & split condition
                  /*aaa*/              //  objFeature[currentID].rect=rects[k];
                  objFeature[currentID].noMatch=-1;
                  //                        if(vbNum<2)
                  //                            matched[k]=1;
                  matchedNum++;
                  haveMatchedRect=true;
                  cout<<"a currentobj matched  id= "<<currentID<<"  k= "<<k;
                  cout<<"rect x width  "<<rects[k].x<<" "<<rects[k].width<<endl;
                  break;
              }
          }
/*aaa*/   if(!haveMatchedRect)
          {
              for(k=0; k<rectsNum; k++)
              {
                  if(isMatchedRect(objFeature[currentID].rect,rects[k]))
                  {
                      objFeature[currentID].rectIndex=k;
                      // set after process join & split condition
                      /*aaa*/              //  objFeature[currentID].rect=rects[k];
                      objFeature[currentID].noMatch=-1;
                      //                        if(vbNum<2)
                      //                            matched[k]=1;
                      matchedNum++;
                      haveMatchedRect=true;
                      cout<<"a currentobj matched  id= "<<currentID<<"  k= "<<k;
                      cout<<"rect x width  "<<rects[k].x<<" "<<rects[k].width<<endl;
                      break;
                  }
              }
          }
          if(!haveMatchedRect){
              // 11-27                 objNum-=1;
              //                  if(objNum<0)
              //                      objNum=0;
              //                  object_Feature_Init(objFeature[currentID]);
              //                  cout<<"current obj removed rect= "<<objFeature[currentID].rectIndex<<endl;
              //                  currentID=-1;
              objFeature[currentID].rectIndex=NOMATCHINDEX;
              jointedIndex=-1;
              objJointed=0;
              objSplited=0;
/*aaa*/       if(objFeature[currentID].noMatch==-1)
              objFeature[currentID].noMatch=NOMATCHTIMES;

          }
      }    //end of match currentobj

      // 1.1  非joined状态，可以匹配两个矩形，进入confirm again state
      index=0;

      if(currentID>-1 && objJointed==0)
      {

          // currentObj to match as many rect as possible
          // here obj[currentID].rect remain as last frame
          for(k=0; k<rectsNum; k++){
              if(isMatchedRect(objFeature[currentID].rect,rects[k]))
                  index++;
          }
          // currentObj can match more than one vbrect
          if(index>1)
          {
             objFeature[currentID].confirmAgain=1;
          }
      }  //end of if(currentID>-1


      //2. if there are obj joined or splited
      index=0;

      if(currentID>-1 && objJointed>0)
      {

          // currentObj to match as many rect as possible
          // here obj[currentID].rect remain as last frame
          for(k=0; k<rectsNum; k++){
              if(isMatchedRect(objFeature[currentID].rect,rects[k]))
                  index++;
          }
          //if obj[jointedIndex] can match two rects
          // obj splited,do not match currentobj rect again
          if(index>1)
          {
              matched[objFeature[currentID].rectIndex]=1;
              //match this splited rect to a obj joined before
              for(k=0; k<rectsNum; k++)
              {
                  if(isMatchedRect(objFeature[jointedIndex].rect,rects[k])
                          && matched[k]==0)
                  {
                      objFeature[jointedIndex].rectIndex=k;
                      /*aaa*/
                      objFeature[jointedIndex].rect=rects[k];
                      //mark this rect,not match again
                      matched[k]=1;
                      matchedNum++;

                  }
              }
              objSplited=1;
              objJointed=0;
              // jointedIndex set to -1 after select currentObj
              cout<<"rect split-------";
          }
          else if(index==1)
          {
              objJointed+=1;
              /*aaa*/
              objFeature[jointedIndex].rectIndex=objFeature[currentID].rectIndex;
              objFeature[jointedIndex].rect=rects[objFeature[currentID].rectIndex];
              if(objJointed==200)
              {
                  if(objFeature[jointedIndex].trustedValue>=TRUSTTHRES)
                      objNum--;
                  object_Feature_Init(objFeature[jointedIndex]);
                  objJointed=0;
                  objSplited=0;
                  jointedIndex=-1;
                  ui->warning_lineEdit->setText("timeout,remove a joined obj");
              }

          }
          else
          {
              if(objFeature[jointedIndex].trustedValue>=TRUSTTHRES)
                  objNum--;
              object_Feature_Init(objFeature[jointedIndex]);
              objJointed=0;
              objSplited=0;
              jointedIndex=-1;
              ui->warning_lineEdit->setText("can't match,remove a joined obj");
          }
      }
/*aaa*/ // set current rect to rect
      if(currentID>-1 && objFeature[currentID].noMatch==-1)
      {
         objFeature[currentID].rect=rects[objFeature[currentID].rectIndex];
      }

      //3. if splited==1, choose a correct rect as currentobj rect
/*aaa*/
      if(currentID>-1 && objFeature[currentID].noMatch==-1 && objSplited==1)
       {
          //a. when splited, do not choose a not moved rect as currentobj rect
          splitChooseOK=false;
          rectemp=objFeature[currentID].rect;
          matchNoMove1=false;
          for(k=0; k<notMoveRectNum; k++)
          {
              if(isMatchedNotMove(notMoveRect[k],rectemp))
                  matchNoMove1=true;
          }
          rectemp=objFeature[jointedIndex].rect;
          matchNoMove2=false;
          for(k=0; k<notMoveRectNum; k++)
          {
              if(isMatchedNotMove(notMoveRect[k],rectemp))
                  matchNoMove2=true;
          }
          //another obj.rect match to a not move rect
          if(matchNoMove2)
              splitChooseOK=true;
          //current obj.rect match to a not move rect but another not, change
          if(matchNoMove1 && !matchNoMove2)
          {
              rectemp=objFeature[currentID].rect;
              index=objFeature[currentID].rectIndex;
              objFeature[currentID].rect=objFeature[jointedIndex].rect;
              objFeature[currentID].rectIndex=objFeature[jointedIndex].rectIndex;
              objFeature[jointedIndex].rect=rectemp;
              objFeature[jointedIndex].rectIndex=index;
              splitChooseOK=true;
          }
          //b. choose one if obj.rect no match obj.initialrect
          if(!splitChooseOK)
          {
              matchNoMove1=false;
              if(isMatchedNotMove(objFeature[currentID].rect,objFeature[currentID].initialRect))
                  matchNoMove1=true;

              matchNoMove2=false;
              if(isMatchedNotMove(objFeature[jointedIndex].rect,objFeature[jointedIndex].initialRect))
                  matchNoMove2=true;
              // another obj.rect match to it's initial
              if(matchNoMove2)
                  splitChooseOK=true;
              //current obj.rect match to initial but another not, change
              if(matchNoMove1 && !matchNoMove2)
              {
                  rectemp=objFeature[currentID].rect;
                  index=objFeature[currentID].rectIndex;
                  objFeature[currentID].rect=objFeature[jointedIndex].rect;
                  objFeature[currentID].rectIndex=objFeature[jointedIndex].rectIndex;
                  objFeature[jointedIndex].rect=rectemp;
                  objFeature[jointedIndex].rectIndex=index;
                  splitChooseOK=true;
              }

          }
          //
          if(!splitChooseOK)
          {
              //add lkrect info
          }

      }

      //4. first time join
        //if an obj matched same rect as current obj
      if(currentID>-1 && objFeature[currentID].noMatch==-1 && objJointed==0 && objSplited==0)
      {
         for(i=0; i< MAXOBJNUM; i++)
         {
            if(objFeature[i].rectIndex>-1 && i!=currentID)
            {
                //there is a joined obj
                if(isMatchedRect(objFeature[i].rect,objFeature[currentID].rect))
                {
                    objFeature[i].rectIndex=objFeature[currentID].rectIndex;
                    objFeature[i].rect=objFeature[currentID].rect;
                    matchedNum++;
                    objJointed=1;
                    jointedIndex=i;
                    currentDirection=objFeature[currentID].moveX;
                    jointedDirection=objFeature[jointedIndex].moveX;
                    if(fabs(currentDirection)<0.1)
                        currentDirection=-jointedDirection;
                    cout<<"two rect jointed"<<currentID<<"  to "<<i<<endl;
                    break;

                }
            }
         }
      }  //end of if(objJoined==0)

      //5. not match current rect again
      //to treat jointed, when match currentObj,we may not set match[]
      //here add it. means this rect has been mached by currentObj
      if(currentID>-1 && objFeature[currentID].noMatch==-1)
      {
          matched[objFeature[currentID].rectIndex]=1;
      }

      //6.matching other objects
       for(i=0; i<MAXOBJNUM; i++)
       {

           if(i==currentID || i==jointedIndex)
               continue;
           if(objFeature[i].rectIndex >-1)
           {

               haveMatchedRect=false;
               for(k=0; k<rectsNum; k++)
               {
                   if(isMatchedRect(objFeature[i].rect,rects[k])
                           && matched[k]==0)
                   {
                       objFeature[i].rectIndex=k;
                       objFeature[i].rect=rects[k];
                       matched[k]=1;
                       matchedNum++;
                       haveMatchedRect=true;
                   }
               }
               if(!haveMatchedRect )
               {     //if there is a objFeture have no matched rect
                   //a confirmed obj removed
                   if(objFeature[i].trustedValue>=TRUSTTHRES)
                   {
                       objNum-=1;
                       if(objNum<0)
                           objNum=0;
                   }

                   object_Feature_Init(objFeature[i]);
                   cout<<"no match, object(comfirmed or not) removed"<<i<<endl;
               } //end of no match
           } //end of if
       }  //end of for

        //7. if there are new rects that not matched to a obj , add to objFeture
       for(k=0; k<rectsNum; k++)
       {
           if(matched[k]==0)                   //not matched rect
           {
               for(i=0; i<MAXOBJNUM; i++)
               {
                   if(objFeature[i].rectIndex >=0)    //already mateched obj,continue
                       continue;
                   objFeature[i].noMatch=-1;
                   objFeature[i].rectIndex= k;
                   objFeature[i].rect=rects[k];
                   objFeature[i].initialRect=rects[k];
                   objFeature[i].lkRect=rects[k];
                   objFeature[i].trackedPointNum=0;
                   objFeature[i].trustedValue =1;  //a obj needed to confirm
                   objFeature[k].notmoveCount=0;
                   matchedNum++;
                   matched[k]=1;
                   cout<<"a rect to add"<<endl;
                   break;
               }
           }
       }

      cout<<" matched number="<<matchedNum<<endl;

     //8. recalc vb matched Num as vbNum;
      vbNum=0;
      for(k=0; k<MAXOBJNUM; k++)
      {
          if(objFeature[k].rectIndex>-1)
          {
              vbNum++;
              cout<<" objindex="<<k;
              cout<<" rectindex="<<objFeature[k].rectIndex;
              cout<<" trust="<<objFeature[k].trustedValue;
              cout<<" points="<<objFeature[k].trackedPointNum<<endl;
              cv::rectangle(colorFrame, rects[objFeature[k].rectIndex],
                      cv::Scalar(255, 0, 0), 2);
          }
      }
      //9.detect if the obj is overlaped with a screenrange
      for(k=0; k<MAXOBJNUM; k++)
      {
          if(objFeature[k].rectIndex>-1 && haveScreenRange)
          {
              Rect2i rect_temp=objFeature[k].rect;
              for (i = 0; i < Num_shield; i++)
              {
                  Rect2i screen_temp=screenRange[i];
                  if (screen_temp.x - SHIELDMARGIN> (rect_temp.x + rect_temp.width)
                          || (screen_temp.x+screen_temp.width + SHIELDMARGIN) < rect_temp.x
                          || screen_temp.y - SHIELDMARGIN> rect_temp.y+rect_temp.height
                          || screen_temp.y+screen_temp.height + SHIELDMARGIN < rect_temp.y)		//	应该外扩	//
                  {
                      objFeature[k].overlapScreen = -1;												//不相交		//
                  }
                  else
                  {
                      objFeature[k].overlapScreen = i;												//相交，具体序号//
                      if (IsInsideScreen(screenRange[i], objFeature[k].rect))
                      {
                          objFeature[k].inScreen = true;													//包含，具体序号//
                      }
                      else
                      {
                          objFeature[k].inScreen = false;												//不包含		//
                      }
                      break;
                  }
              }
          }
      }
      cout<<" vb number="<<vbNum<<" joinedIndex="<<jointedIndex<<endl;



// ** 七、LK process..................................
      //begin to do lk tracking
      for(k=0; k<MAXOBJNUM; k++)
      {
          if(objFeature[k].rectIndex== -1)
              continue;
          // do not track untrusted obj
          else if(objFeature[k].trustedValue== -1)
              continue;
         //do not track jointed obj
          else if(objJointed>0 && k==jointedIndex)
              continue;
          //need to do lk
          else
          {
              // 1. if lk points must be regotten
              // a. if splited==1 get new points
              candoLK=0;
              segTemp=segmentationMap.data;
              pointNum=0;
              int x,y;
              if(objSplited==1)
              {
                  objFeature[k].points[0].clear();
                  pointNum=0;
                  rectemp=objFeature[k].rect;

                  for (int i = 0; i < 10; i++)
                  {
                      x=rectemp.x+i*rectemp.width/10.;
                      p.x=x;

                      for (int j = 0; j < 20; j++)
                      {
                          y=rectemp.y+j*rectemp.height/20.;
                          if(*(segTemp+y*width+x)==COLOR_FOREGROUND)
                          {
                              p.y=y;
                              objFeature[k].points[0].push_back(p);
                              pointNum++;
                              cv::circle(colorFrame, p, 1,
                                         cv::Scalar(255, 0, 0), -1);
                          }

                      }
                  }

                  objFeature[k].trackedPointNum=pointNum;
                  if(pointNum>5)
                    candoLK=1;
              }  //end of split==1

              //b. currentObj and noMatch>-1 do not get new points
              else if(k==currentID && objFeature[k].noMatch > -1)
              {
                  if(objFeature[k].trackedPointNum>0)
                      candoLK=1;
              }
              //c. not enough points
              else
              {
                  if(objFeature[k].trackedPointNum < LEASTLKPOINTS)
                  {
                      objFeature[k].points[0].clear();
                      rectemp=objFeature[k].lkRect;
                      vbtemp=objFeature[k].rect;
                      centerPoint.x=rectemp.x+rectemp.width/2;
                      centerPoint.y=rectemp.y+rectemp.height/2;

                      cv::circle(colorFrame, centerPoint, 4,
                              cv::Scalar(0, 255, 255), -1);
                      cout<<" k<least get new,lkrect= "<<rectemp.x<<" "
                         <<rectemp.y<<" "<<rectemp.width<<" "<<rectemp.height<<endl;
                      if(rectemp.width<20)
                      {
                          rectemp.x=centerPoint.x-15;
                          if(rectemp.x<0)
                              rectemp.x=0;
                          rectemp.width=30;
                          if((rectemp.x+rectemp.width)>(width-1))
                              rectemp.width=width-1-rectemp.x;
                      }
                      if(rectemp.height<30)
                      {
                          rectemp.y=centerPoint.y-25;
                          if(rectemp.y<0)
                              rectemp.y=0;
                          rectemp.height=50;
                          if((rectemp.y+rectemp.height)>(height-1))
                              rectemp.height=height-1-rectemp.y;

                      }
                      if(rectemp.x<vbtemp.x)
                          rectemp.x=vbtemp.x;
                      if((rectemp.x+rectemp.width)>(vbtemp.x+vbtemp.width))
                          rectemp.width=vbtemp.x+vbtemp.width-rectemp.x;
                      if(rectemp.y<vbtemp.y)
                          rectemp.y=vbtemp.y;
                      if(rectemp.y+rectemp.height>vbtemp.y+vbtemp.height)
                          rectemp.height=vbtemp.y+vbtemp.height-rectemp.y;



                      pointNum=0;

                      for (int i = 0; i < 10; i++)
                      {
                          x=rectemp.x+i*rectemp.width/10.;
                          p.x=x;

                          for (int j = 0; j < 20; j++)
                          {
                              y=rectemp.y+j*rectemp.height/20.;
                              if(*(segTemp+y*width+x)==COLOR_FOREGROUND)
                              {
                                  p.y=y;
                                  objFeature[k].points[0].push_back(p);
                                  pointNum++;
                                  cv::circle(colorFrame, p, 1,
                                             cv::Scalar(255, 0, 0), -1);
                              }

                          }
                      }
                      objFeature[k].trackedPointNum=pointNum;
                      if(pointNum>5)
                        candoLK=1;
                  }
                  else
                      candoLK=1;
              }  //end c.else

              // 2. do lk track

              TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.01);
              Size winSize(7,7);



              if(candoLK==1 ) //need to tracking
              {
                  cv::calcOpticalFlowPyrLK(
                              frame_prev, frame, // 2 consecutive images
                              objFeature[k].points[0], // input point positions in first image
                          objFeature[k].points[1], // output point positions in the 2nd image
                          status, // tracking success
                          err, // tracking error
                          winSize,1,termcrit,0,0.01);
                  //  a.loop over the tracked points to reject some
                  pointNum=0;
                  moveNum=0;
                  xmove=0;
                  ymove=0;
                  sumXmove=0;
                  sumYmove=0;
                  meanMove=0;
                  for( int i= 0; i < objFeature[k].points[1].size(); i++ ) {
                      //  do we keep this point?
                      if(status[i]){
                          xmove=objFeature[k].points[0][i].x-objFeature[k].points[1][i].x;
                          ymove=objFeature[k].points[0][i].y-objFeature[k].points[1][i].y;
                          if((fabs(xmove)+fabs(ymove))<15)
                          {
                              objFeature[k].points[1][pointNum] = objFeature[k].points[1][i];
                              objFeature[k].points[0][pointNum++] = objFeature[k].points[0][i];
                              cv::circle(colorFrame, objFeature[k].points[1][i], 1,
                                      cv::Scalar(0, 255, 0), -1);
                              // if point has moved,calculate sum
                              if((fabs(xmove)+fabs(ymove))>0.01)
                              {
                                  sumXmove += xmove;
                                  sumYmove += ymove;
                                  moveNum++;
                              }
                          }
                      }
                  } //end of for


                  // b. record lk results:lkrect, pointNUm, objMvx/y
                  objFeature[k].points[1].resize(pointNum);
                  objFeature[k].lkMatchedNum=pointNum;
                  objFeature[k].trackedPointNum=pointNum;
//modify 3.28   move index of MovX MovY
                  objFeature[k].mvIndex+=1;
                  if(objFeature[k].mvIndex==MOVENUM)
                      objFeature[k].mvIndex=0;

                  index=objFeature[k].mvIndex;


                  if(pointNum>=LEASTMOVEPOINTS)
                  {
                      // record movement
                      if(moveNum>0)
                      {
                          objFeature[k].objMvX[index]=sumXmove/moveNum;
                          objFeature[k].objMvY[index]=sumYmove/moveNum;
                      }
                      else
                      {
                          objFeature[k].objMvX[index]=0;
                          objFeature[k].objMvY[index]=0;
                      }
                      //record lkrect
                      lkLeft=width;
                      lkRight=0;
                      lkTop=height;
                      lkBottom=0;

                      for(i= 0; i < objFeature[k].points[1].size(); i++ )
                      {
                          p= objFeature[k].points[1][i];
                          if(p.x<lkLeft)
                              lkLeft=p.x;
                          if(p.x>lkRight)
                              lkRight=p.x;
                          if(p.y<lkTop)
                              lkTop=p.y;
                          if(p.y>lkBottom)
                              lkBottom=p.y;

                      }

                      objFeature[k].lkRect.x=lkLeft;
                      objFeature[k].lkRect.y =lkTop;
                      objFeature[k].lkRect.width=lkRight-lkLeft;
                      objFeature[k].lkRect.height=lkBottom-lkTop;

                      //  end of record lkrect
                      //--c.if obj moved, calc again, eliminate unmoved points
                      if(moveNum>0)
                      {
                          meanMove=(fabs(sumXmove)+fabs(sumYmove))/moveNum;
                          meanMove=meanMove/4;
                          pointNum=0;
                          sumXmove=0;
                          sumYmove=0;
                          for( int i= 0; i < objFeature[k].points[1].size(); i++ )
                          {
                              xmove=objFeature[k].points[0][i].x-objFeature[k].points[1][i].x;
                              ymove=objFeature[k].points[0][i].y-objFeature[k].points[1][i].y;
                              if((fabs(xmove)+fabs(ymove))>meanMove)
                              {
                                  objFeature[k].points[1][pointNum++] = objFeature[k].points[1][i];
                                  cv::circle(colorFrame, objFeature[k].points[1][i], 1,
                                          cv::Scalar(0, 0, 255), -1);
                                  sumXmove += xmove;
                                  sumYmove += ymove;
                              }
                          }
                          // set trackedpointNUm and objMvx/y again
                          objFeature[k].points[1].resize(pointNum);
                          objFeature[k].trackedPointNum=pointNum;
                          if(pointNum>0)
                          {
                              objFeature[k].objMvX[index]=sumXmove/pointNum;
                              objFeature[k].objMvY[index]=sumYmove/pointNum;
                          }
                          else
                          {
                              objFeature[k].objMvX[index]=0;
                              objFeature[k].objMvY[index]=0;
                          }

                      } // end of moveNum>0 calc again

                  }  //end of pointNum>=LEASTLKPOINTS/2
                 //pointNum<=10 //lkrect same as last frame
                  else
                  {
                      objFeature[k].objMvX[index]=0;
                      objFeature[k].objMvY[index]=0;
                  }

                  sumXmove=0;
                  sumYmove=0;
                  for(i=0; i<MOVENUM; i++){
                      sumXmove+=objFeature[k].objMvX[i];
                      sumYmove+=objFeature[k].objMvY[i];
                  }
                  objFeature[k].moveX=sumXmove/MOVENUM;
                  objFeature[k].moveY=sumYmove/MOVENUM;

                  // draw obj under tracking  lkrect
                  cv::rectangle(colorFrame, objFeature[k].lkRect,
                                cv::Scalar(255, 255, 255), 1);

                  cout<<"--k= "<<k;
                  cout<<"   point number="<<pointNum<<"  moveX="<<objFeature[k].moveX;
                  cout<<"  moveY="<<objFeature[k].moveY;
                  cout<<"   MvX="<<objFeature[k].objMvX[objFeature[k].mvIndex];
                  cout<<"  MvY="<<objFeature[k].objMvY[objFeature[k].mvIndex]<<endl;

                  //lkrect not matched with VB-rect, =vb-rect
                  if(k!=currentID || (k==currentID && objFeature[k].noMatch==-1))
                  {
                      if(!isMatchedRectLK(objFeature[k].lkRect,objFeature[k].rect))
                      {
                          objFeature[k].lkRect=objFeature[k].rect;
                          objFeature[k].trackedPointNum=0;

                      }
                  }

                  std::swap(objFeature[k].points[1], objFeature[k].points[0]);
              } //end of do lk tracking for a obj

          } //end of else (need to do lk)


      }  //end of do lk truacking for all obj


      //3.treat tracking status of lk: move or not
     for(k=0; k<MAXOBJNUM; k++){
         if(objFeature[k].trustedValue> -1)
         {
            if(!(objJointed>0 && k==jointedIndex))
            {
             //record lk detect counts.
             objFeature[k].trustCount++;
             //lk tracking,obj moved
             index=objFeature[k].mvIndex;
//modify 3.28  x,y two dimentional movement, maybe lower value of LEASTMOVE
             if((fabs(objFeature[k].objMvX[index])
                     +fabs(objFeature[k].objMvY[index]))>=LEASTMOVE)
             {
                 if(objFeature[k].trustedValue<TRUSTTHRES){  //待定目标
                     objFeature[k].trustedValue+=1;
                     if(objFeature[k].trustedValue==TRUSTTHRES){
                         objFeature[k].notmoveCount=0;
                         objFeature[k].trustCount=0;
                         objNum+=1;
                         cout<<"a object comfirmed index="<<k<<endl;
                     }

                 }    //end of 待定目标
                 else{
                     objFeature[k].trustedValue+=1;
                     if(objFeature[k].trustedValue>1000)
                         objFeature[k].trustedValue=100;
                     objFeature[k].notmoveCount=0;
                 }

             }  //end of tracking obj moved
             else
             {    //lk tracking,obj not moved
                 if(objFeature[k].trustedValue<TRUSTTHRES){      //待定目标
                     objFeature[k].trustedValue-=1;
                     if(objFeature[k].trustedValue==-1)
                     {
                         if(objFeature[k].trustCount<MAXTRUSTTIMES)
                             objFeature[k].trustedValue=1;
                         else
                         {
                             objFeature[k].trustCount=0;
                             //if not joined obj, record this rect as notMoveREct
                             if(k!=jointedIndex)
                             {
                                 notMoveRect.push_back(objFeature[k].rect);
                                 notMoveRectNum++;
                             }

                         }
                     }
                 }
                 else{                                  //目标
                     objFeature[k].notmoveCount+=1;

                 }
             }    //end of tracking obj not move
         }  //end of second if
       } //end of firs if
     }  //end of for(k=0 treat lk status of obj

// ** 八、策略：process after vb and lk......................
     //1. treat condition that jointed rect split
     if(vbNum>1 && currentID>-1 && objSplited==1 && !splitChooseOK)
     {
        // if(objFeature[currentID].rectIndex
        //         !=objFeature[jointedIndex].rectIndex){
           //rect splited
             objJointed=0;
             objSplited=0;
             cout<<"rect split, choose  "<<currentDirection<<"  "<<jointedDirection;
             cout<<"  "<<objFeature[currentID].objMvX[objFeature[currentID].mvIndex];
             cout<<"   "<<objFeature[jointedIndex].objMvX[objFeature[jointedIndex].mvIndex];
             cout<<"    "<<endl;

             index=currentDirection
                     * objFeature[currentID].objMvX[objFeature[currentID].mvIndex];
             if(objFeature[jointedIndex].trustedValue>=TRUSTTHRES
                     && objFeature[jointedIndex].objMvX[objFeature[jointedIndex].mvIndex]!=0
                     && index<=0){
                 //not the same direction, exchange currentobj and joined obj
                 objFeature[currentID].isCurrentObj= false;
                 currentID= jointedIndex;
                 objFeature[currentID].isCurrentObj= true;

             }
             jointedIndex=-1;
             ui->warning_lineEdit->setText("jointed obj splited");
      //   }
     }



     //2.treat status that current obj has not match a vb-rect
      if(currentID > -1 && objFeature[currentID].noMatch > -1)
      {
          if(objFeature[currentID].trackedPointNum>0)
          {
              objFeature[currentID].noMatch--;
              if(objFeature[currentID].noMatch==0)
              {
                  objNum-=1;
                  if(objNum<0)
                      objNum=0;
                  object_Feature_Init(objFeature[currentID]);
                  currentID=-1;
                  objJointed=0;
                  objSplited=0;
                  jointedIndex=-1;
                  overlapScreenID=-1;
                  isOverlap=0;
                  insideScreenRange=0;
                  screenTime=0;
              }
          }
          else
          {
              objNum-=1;
              if(objNum<0)
                  objNum=0;
              object_Feature_Init(objFeature[currentID]);
              currentID=-1;
              objJointed=0;
              objSplited=0;
              jointedIndex=-1;
              overlapScreenID=-1;
              isOverlap=0;
              insideScreenRange=0;
              screenTime=0;
          }
      }

      // 3.deadlocked(trustedValue==-1) obj, if move then do lk detect again
       for(k=0; k<MAXOBJNUM; k++)
       {
           if(objFeature[k].rectIndex>-1 && objFeature[k].trustedValue==-1)
               if(!isMatchedRectInitial(objFeature[k].rect,objFeature[k].initialRect))
               {
                   objFeature[k].trustedValue=1;
                   objFeature[k].notmoveCount=0;
                   objFeature[k].trustCount=0;
                   objFeature[k].trackedPointNum=0;
                   objFeature[k].initialRect=objFeature[k].rect;
                   objFeature[k].lkRect=objFeature[k].rect;

               }

       }

       // 4.tracking obj(trustedValue>=TRUSTTHRES), if not move,do confirm again
       for(k=0; k<MAXOBJNUM; k++)
       {
           if(objFeature[k].rectIndex>-1 && objFeature[k].trustedValue>=TRUSTTHRES
                   && k!=jointedIndex)
           {
             //a.if all in a notMoveRect, set trustedValue 1, re lk
               matchNoMove1=false;
               matchNoMove2=false;
               for(i=0; i<notMoveRectNum; i++)
               {
                   if(isMatchedNotMove_1(notMoveRect[i],objFeature[k].rect))
                   {
                       matchNoMove1=true;
                       break;
                   }
               }
//modified 3.30
               if(objFeature[k].notmoveCount>NOTMOVECOUNT)
                       matchNoMove2=true;

               if(matchNoMove1 && matchNoMove2)
               {
//modify 3.30  if a currentObj, enter confirmagain state
                   if(k==currentID)
                       objFeature[currentID].confirmAgain=1;
                   else
                   {
                       objFeature[k].trustedValue=1;
                       objFeature[k].notmoveCount=0;
                       objFeature[k].trustCount=0;
                       objFeature[k].trackedPointNum=0;
                       objFeature[k].initialRect=objFeature[k].rect;
                       objFeature[k].lkRect=objFeature[k].rect;

                   }
               }
           }

       }

     //增加一个当前目标再确定过程 confirmedAgain
     //三种情况会进入再确定过程
     // 1.在屏蔽区内，倒计数到时； 2.noMoveCount>阈值； 3.无joined状态分离两个矩形
     if(currentID>-1)
     {
         if(objFeature[currentID].confirmAgain==1)
         {
             objFeature[currentID].confirmCount++;
             index=objFeature[currentID].mvIndex;
             if((fabs(objFeature[currentID].objMvX[index])
                 +fabs(objFeature[currentID].objMvY[index]))>=LEASTMOVE)
             {
                 objFeature[currentID].confirmValue+=1;
                 if(objFeature[currentID].confirmValue==TRUSTTHRES) //再确认成功
                 {
                     objFeature[currentID].confirmAgain=0;
                     objFeature[currentID].confirmCount=0;
                     objFeature[currentID].confirmValue=-1;
                     cout<<"a currentobj comfirm again success index="<<currentID<<endl;
                 }

             }  //end of tracking obj moved
             else
             {    //lk tracking,obj not moved

                     objFeature[currentID].confirmValue-=1;
                     if(objFeature[currentID].confirmValue==-1)
                     {
                         if(objFeature[currentID].confirmCount<MAXTRUSTTIMES)
                             objFeature[currentID].confirmValue=1;
                         else   //confirm not ok
                         {
                             object_Feature_Init(objFeature[currentID]);
                             objNum--;

                             currentID=-1;
                             objJointed=0;
                             objSplited=0;
                             jointedIndex=-1;
                             overlapScreenID=-1;
                             isOverlap=0;
                             insideScreenRange=0;
                             screenTime=0;


                         }
                     }

             }    //end of tracking obj not move
         }
     }

    //recalclate objNum
      objNum=0;
      for(k=0; k<MAXOBJNUM; k++)
      {
          if(objFeature[k].rectIndex>-1 && objFeature[k].trustedValue >=TRUSTTHRES)
              objNum++;
      }
      outputObjNum = objNum;

      //there is at least one obj
      if(objNum>0)
      {
          // 5. 选取currentobj 输出x,y and objNum

          disToCenter= CENTERx;
          index= -1;

          //1). there is no current obj, choose one not overlap with a screenRange

          if(currentID<0)
          {
              for(k=0; k<MAXOBJNUM; k++)
              {
                  if(objFeature[k].trustedValue>=TRUSTTHRES
                          && objFeature[k].overlapScreen==-1)   //is a confirmed obj,not overlap screen
                  {
                      centerPoint.x=objFeature[k].lkRect.x+objFeature[k].lkRect.width/2;
                      i=abs(centerPoint.x-CENTERx);
                      if(i < disToCenter)
                      {   //obj near to center
                          disToCenter=i;
                          index=k;
                      }
                  }
              }
              if(index>-1)
              {
                  currentID=index;
                  objFeature[currentID].isCurrentObj=true;
                  objFeature[currentID].Prestate_isCurrentObj=false;
              }
          }  //end if currentID<0
          //2) if not a obj witch is not overlap with scrrenRange
          if(currentID<0)
          {
              for(k=0; k<MAXOBJNUM; k++)
              {
                  if(objFeature[k].trustedValue>=TRUSTTHRES)   //is a confirmed obj
                  {
                      if(!objFeature[k].inScreen)
                      {
                          currentID=k;
                          objFeature[currentID].isCurrentObj=true;
                          objFeature[currentID].Prestate_isCurrentObj=false;
                          break;
                      }
                  }
              }
          }  //end if currentID<0, maybe choose a obj overlaped with screenRange

          //3) if there has not a obj witch is not overlap with scrrenRange
          //     obj must inside screenRange, choose this
          if(currentID<0)
          {
              for(k=0; k<MAXOBJNUM; k++)
              {
                  if(objFeature[k].trustedValue>=TRUSTTHRES)   //is a confirmed obj
                  {
                      //+条件，宽，高，落地等
                      currentID=k;
                      objFeature[currentID].isCurrentObj=true;
                      objFeature[currentID].Prestate_isCurrentObj=false;
                      break;

                  }
              }
          }  //end if currentID<0, maybe choose a obj overlaped with screenRange



          //6. 屏蔽区处理 if there are at least one ScreenRange
          if(currentID >-1 && haveScreenRange)
          {
              //01当前跟踪目标与屏蔽区关系	开始标记
              insideScreenRange=0;
              if(objFeature[currentID].overlapScreen==-1)
              {
                  isOverlap = 0;					//不相交//
                  insideScreenRange = 0;		    //不包含//
                  screenTime = 0;					//目标和屏蔽区不相交，直接清零//
              }
              else
              {
                  isOverlap = 1;								//相交
                  overlapScreenID = objFeature[currentID].overlapScreen;//记录跟踪目标相交的屏蔽区序号	//
                  if (!objFeature[currentID].inScreen)
                  {
                      insideScreenRange = 0;				//不包含	//
                  }
                  else
                  {
                      insideScreenRange = 1;			 //包含		//
                      if (screenTime == 0)
                          screenTime = SCREENTIME;

                  }
              }/* ========= 标记结束 */


              /* 02 相交而不在内 设screenTime	------------- */
              if (isOverlap==1 && insideScreenRange==0)
              {
                  if (vbNum == 1 && objFeature[currentID].noMatch == -1)
                      //目标仅一个，只是相交，标记600，不做倒计时
                      screenTime = 600;
                  else													// 多个目标	//
                  {
                      index = -1;
                      for (k = 0; k < MAXOBJNUM; k++)
                      {
                          if (k == currentID)
                              continue;

                          //10.25  只要屏蔽区有rect，就可以认为屏幕打开的
                          if (objFeature[k].rectIndex > -1 && objFeature[k].overlapScreen == overlapScreenID)	//有另个目标在相交屏蔽区内，此时可能连在一起，形成一个CCL//
                              index = 1;
                      }
                      if (index == -1)
                          screenTime = 600;	 //有其他目标，但不在这个屏蔽区内，标记600，不倒计时	//
                      else
                          screenTime = SCREENTIME;	 //	多目标，当前跟踪相交不在内，且其它目标也在【该】屏蔽区内	//
                      //
                  }
              }

              /* 03 如果当前目标完全在屏蔽区内（包含），且有其他目标，选取一个不在屏蔽区的目标跟	------------------------------------ */

              if (insideScreenRange == 1)
              {
                  if (objNum > 1)																					//	目标不止一个	//
                  {
                      for (k = 0; k < MAXOBJNUM; k++)
                      {
                          if (k == currentID)
                              continue;

                          if (objFeature[k].overlapScreen == -1 && objFeature[k].trustedValue >= TRUSTTHRES)		//另个目标与其他屏蔽区不相交，则更换为当前目标，直接跟//
                          {
                              objFeature[currentID].isCurrentObj = 0;
                              currentID = k;
                              objFeature[currentID].isCurrentObj = 1;
                              objFeature[currentID].Prestate_isCurrentObj = 0;									//20171128	//

                              isOverlap = 0;
                              insideScreenRange = 0;
                              screenTime = 0;																	//目标和屏蔽区不相交，直接清零//
                              break;
                          }
                      }
                  }
                  //否则，if (objNum <= 1)只有这一个包含在内的目标，跳过不处理//
              }

              /* 04 screenTime--  	------------------------------------ */
              if (insideScreenRange == 1)
              {
                 //2017.10.30  判断目标从屏蔽区下方消失，若存在部分跟踪区，
                 // 则可认为目标直接消失，否则，目标进入屏蔽区；
                  if (screenTime > 0 && screenTime != 600)
                  {

                      if ((screenRange[overlapScreenID].y+ screenRange[overlapScreenID].height+8) < height)
                          //如果屏蔽区未贯穿检测区，当人从屏蔽区底边消失时，防止跟到屏蔽区，直接回中。//
                      {
                          screenTime -= 5;
                      }
                      else
                      {
                          screenTime--;
                      }

                      //printf("__LINE__%d	screenTime=%d\n", __LINE__, screenTime);

                      if (screenTime < 0)
                      {
                          screenTime = 0;
                      }
                  }
//modified 3.30
                  if (screenTime == 0)		//enter confirm again state
                  {
                     objFeature[currentID].confirmAgain = 1;
                     objFeature[currentID].confirmValue=1;
                     objFeature[currentID].confirmCount=0;
                  }

              }  //end of insideScreen==1
          }  //end of currentID>-1 && haveScreenRange

      }   //end of have obj

     //output
     if(objSplited==1)
     {
         objSplited=0;
         jointedIndex=-1;
     }

     if(currentID>-1){
         outputRect=objFeature[currentID].lkRect;
         outputRect.x+=left;
         outputRect.y+=top;
      //   outputTimeout=objFeature[currentID].notmoveCount;
         cout<<" currentid="<<currentID;
         cout<<" trustedValue="<<objFeature[currentID].trustedValue;
         cout<<" confirmAgain="<<objFeature[currentID].confirmAgain;
         cout<<" confirmValue="<<objFeature[currentID].confirmValue;
         cout<<" confirmCount="<<objFeature[currentID].confirmCount;
         cout<<" notmoveCount="<<objFeature[currentID].notmoveCount;
         cout<<" noMatch="<<objFeature[currentID].noMatch;
         cout<<endl;
         cv::rectangle(inputFrame, outputRect,
                            cv::Scalar(0, 0, 255), 4);
     }
//     else
//     {
//         outputRect=Rect(outputCenterx-20,outputCentery-30,40,60);
//     }

     cout<<" ...Status...  ";
     cout<<"  object number="<<objNum;
     cout<<"  outputObjNum="<<outputObjNum;
     cout<<"  inScreenRange="<<insideScreenRange;
     cout<<"  isOverlap="<<isOverlap;
     cout<<"  screenTime="<<screenTime;
     cout<<"  Jointed status="<<objJointed;
     cout<<" objSplit "<<objSplited<<" notmoveRect="<<notMoveRectNum;
     cout<<endl;

     //init all where are some errors
     if(objNum>0)
         noObjCount=0;
     if(objNum==0){
         noObjCount+=1;
         if(noObjCount>10){
             objNum=     0;
             currentID=  -1;
             objJointed=0;
             objSplited=0;
             jointedIndex=-1;
             overlapScreenID=-1;
             isOverlap=0;
             insideScreenRange=0;
             screenTime=0;
             currentDirection=-1;
             jointedDirection=-1;
             noObjCount=0;

         }
     }
     //reach MAXOBJNUM,to many obj,remove them except currentobj
     if(objNum==MAXOBJNUM && currentID>-1){
         for(k=0; k<MAXOBJNUM; k++){
             if(!objFeature[k].isCurrentObj){
                 object_Feature_Init(objFeature[k]);
             }
         }
         objNum=1;
     }
    //set mask of current object to COLOR_CURRENTOBJ
//modified 3.30  in confirm again state, dot not set mask of current obj(不锁定）
     if(currentID>-1 && objFeature[currentID].confirmAgain==0){
         for(i=0; i<objFeature[currentID].rect.height; i++){
             index=(i+objFeature[currentID].rect.y)* updateMap.cols;
             for(j=objFeature[currentID].rect.x; j<objFeature[currentID].rect.width; j++)
             {
                 segTemp=updateMap.data+index+j;
                 if(*segTemp==COLOR_FOREGROUND)
                     *segTemp=COLOR_CURRENTOBJ;
             }
         }
     }
     //VIBE background update
     libvibeModel_Sequential_Update_8u_C1R(model, frame.data, updateMap.data);

     cv::swap(frame_prev, frame);

      /* Shows  the segmentation map. */
     imshow("Frame", colorFrame);
     imshow("Tracking",inputFrame);

     //human skin detect
#if SKIN
     uint8_t *Pointer;
     uint8_t Blue,Green,Red,Max,Min;

     for (j = 0; j < height; j++)
     {
         Pointer = faceFrame.data + j * width*3;
         //SkinP = SkinScan0 + Y * SkinStride;
         for (i = 0; i < width; i++)
         {
             Blue = *Pointer; Green = *(Pointer + 1); Red = *(Pointer + 2);
             //*Pointer= Green;
             // *(Pointer + 2)=0;
             if (Red > 75 && Green > 40 && Blue > 20 && Red > Blue && Red > Green && abs(Red - Green) > 15)
             {
                 if (Blue >= Green)
                 {
                     Max = Blue;
                     Min = Green;
                 }
                 else
                 {
                     Max = Green;
                     Min = Blue;
                 }
      //           if (Red > Max)
                     Max = Red;
      //           else if (Red < Min)
      //               Min = Red;
                 if (Max - Min > 15)
                 {
                     *Pointer = 255;
                     *(Pointer+1)=0;
                     *(Pointer+2)=0;
                 }
             }
             Pointer += 3;
       //      SkinP++;
         }
     }

     imshow("faceFrame",faceFrame);
#endif  //end of skin detect

     gettimeofday(&tsEnd, NULL);//-----------------------测试时间
    // long runtimes;
     runtimes=1000000L*(tsEnd.tv_sec-tsBegin.tv_sec)+tsEnd.tv_usec-tsBegin.tv_usec;
     cout<<"  time: "<<runtimes/1000<<"................end"<<endl;

      /* Gets the input from the keyboard. */
      keyboard = waitKey(0);

    } //end of while

    /* Delete capture object. */
    capture.release();

    /* Frees the model. */
    libvibeModel_Sequential_Free(model);

    destroyAllWindows();
    return;

}


void MainWindow::on_openvideo_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("select video file"), ".",
                                              tr("Vedio Files(*.mp4 *.avi *.mkv *.yuv *.wmv)"));

    if (filename.isEmpty()) {
        ui->warning_lineEdit->setText("file open does not success !!!");
        return;
    }
    ui->file_lineEdit->setText(filename);
    ui->warning_lineEdit->clear();


    capture.open(filename.toStdString());
    //   VideoCapture capture("rtsp://192.168.100.182/2");
    if(!capture.isOpened())
    {
        ui->warning_lineEdit->setText("File open error,please open video file first");
        return ;
    }

    if (!capture.read(inputFrame)) {
        ui->warning_lineEdit->setText("end of File ");
        capture.release();

        cv::destroyAllWindows();
        return;
    }

    cv::namedWindow(inputWindow);
    cv::imshow(inputWindow,inputFrame);

    return;
}

void MainWindow::on_roi_pushButton_clicked()
{
    if(!capture.isOpened())
    {
        ui->warning_lineEdit->setText("Please open a video file first");
        return ;
    }
   ui->warning_lineEdit->setText("setting ROI.......");
   cv::Mat tempFrame;
   rectState=NOT_SET;
   cv::setMouseCallback( inputWindow, roi_on_mouse, 0 );
   while(1)
   {
       if(rectState==EXIT)
       {
           ui->warning_lineEdit->setText("ROI set ok");
           break;
       }

       if(rectState==SET)
       {
           rectState=NOT_SET;
           roiState=true;
       }
       inputFrame.copyTo(tempFrame);
       cv::rectangle(tempFrame,roiRect,cv::Scalar(255, 0, 0), 2);
       if(maskState1)
            cv::rectangle(tempFrame,maskRect1,cv::Scalar(0, 255, 0), 2);
       if(maskState2)
            cv::rectangle(tempFrame,maskRect2,cv::Scalar(0, 255, 0), 2);
       cv::imshow(inputWindow,tempFrame);
       cv::waitKey(20);
   }
   cv::setMouseCallback( inputWindow, nothing_on_mouse, 0 );
}

void MainWindow::on_mask1_pushButton_clicked()
{
    if(!capture.isOpened())
    {
        ui->warning_lineEdit->setText("Please open a video file first");
        return ;
    }
   ui->warning_lineEdit->setText("setting MASK.......");
   cv::Mat tempFrame;
   rectState=NOT_SET;
   cv::setMouseCallback( inputWindow, mask1_on_mouse, 0 );
   while(1)
   {
       if(rectState==EXIT)
       {
           ui->warning_lineEdit->setText("MASK set ok");
           break;
       }

       if(rectState==SET)
       {
           rectState=NOT_SET;
           maskState1=true;
       }
       inputFrame.copyTo(tempFrame);
       if(roiState)
            cv::rectangle(tempFrame,roiRect,cv::Scalar(255, 0, 0), 2);
       if(maskState2)
            cv::rectangle(tempFrame,maskRect2,cv::Scalar(0, 255, 0), 2);
       cv::rectangle(tempFrame,maskRect1,cv::Scalar(0, 255, 0), 2);
       cv::imshow(inputWindow,tempFrame);
       cv::waitKey(10);
   }
   cv::setMouseCallback( inputWindow, nothing_on_mouse, 0 );
}

void MainWindow::on_mask2_pushButton_clicked()
{
    if(!capture.isOpened())
    {
        ui->warning_lineEdit->setText("Please open a video file first");
        return ;
    }
   ui->warning_lineEdit->setText("setting MASK.......");
   cv::Mat tempFrame;
   rectState=NOT_SET;
   cv::setMouseCallback( inputWindow, mask2_on_mouse, 0 );
   while(1)
   {
       if(rectState==EXIT)
       {
           ui->warning_lineEdit->setText("MASK set ok");
           break;
       }

       if(rectState==SET)
       {
           rectState=NOT_SET;
           maskState2=true;
       }
       inputFrame.copyTo(tempFrame);
       if(roiState)
            cv::rectangle(tempFrame,roiRect,cv::Scalar(255, 0, 0), 2);
       if(maskState1)
            cv::rectangle(tempFrame,maskRect1,cv::Scalar(0, 255, 0), 2);
       cv::rectangle(tempFrame,maskRect2,cv::Scalar(0, 255, 0), 2);
       cv::imshow(inputWindow,tempFrame);
       cv::waitKey(10);
   }
   cv::setMouseCallback( inputWindow, nothing_on_mouse, 0 );
}
