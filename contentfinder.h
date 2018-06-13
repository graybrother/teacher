#ifndef CONTENTFINDER_H
#define CONTENTFINDER_H

#include "core.hpp"
#include "imgproc.hpp"

class ContentFinder {

  private:

    // histogram parameters
    float hranges[2];
    const float* ranges[3];
    int channels[3];

    float threshold;           // decision threshold
    cv::Mat histogram;         // histogram can be sparse 输入直方图
    cv::SparseMat shistogram;  // or not
    bool isSparse;

  public:

    ContentFinder() : threshold(0.1f), isSparse(false) {

        // in this class,
        // all channels have the same range
        ranges[0]= hranges;
        ranges[1]= hranges;
        ranges[2]= hranges;
    }

    // Sets the threshold on histogram values [0,1]
    void setThreshold(float t) {

        threshold= t;
    }

    // Gets the threshold
    float getThreshold() {

        return threshold;
    }

    // Sets the reference histogram
    void setHistogram(const cv::Mat& h) {

        isSparse= false;
        cv::normalize(h,histogram,1.0);
    }

    // Sets the reference histogram
    void setHistogram(const cv::SparseMat& h) {

        isSparse= true;
        cv::normalize(h,shistogram,1.0,cv::NORM_L2);
    }

    // All channels used, with range [0,256]
    cv::Mat find(const cv::Mat& image) {

        cv::Mat result;

        hranges[0]= 0.0;    // default range [0,256]
        hranges[1]= 256.0;
        channels[0]= 0;     // the three channels
        channels[1]= 1;
        channels[2]= 2;

        return find(image, hranges[0], hranges[1], channels);
    }

    // Finds the pixels belonging to the histogram
    cv::Mat find(const cv::Mat& image, float minValue, float maxValue, int *channels) {

        cv::Mat result;

        hranges[0]= minValue;
        hranges[1]= maxValue;
        ranges[0]= hranges;
        ranges[1]= hranges;
        ranges[2]= hranges;

        if (isSparse) { // call the right function based on histogram type

           for (int i=0; i<shistogram.dims(); i++)
              this->channels[i]= channels[i];

           cv::calcBackProject(&image,
                      1,            // we only use one image at a time
                      channels,     // vector specifying what histogram dimensions belong to what image channels
                      shistogram,   // the histogram we are using
                      result,       // the resulting back projection image
                      ranges,       // the range of values, for each dimension
                      255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255
           );

        } else {

           for (int i=0; i<histogram.dims; i++)
              this->channels[i]= channels[i];
//某对象的this指针，指向被调用函数所在的对象，此处对象为ContentFinder类
           //this->channels[i]即ContentFinder类的私有成员channels[3]
           //对ContentFinder类各成员的访问均通过this进行
           cv::calcBackProject(&image,
                      1,            // we only use one image at a time
                      channels,     // 向量表示哪个直方图维度属于哪个图像通道
                      histogram,    // 用到的直方图
                      result,       // 反向投影的图像
                      ranges,       // 每个维度值的范围
                      255.0         // 选用的换算系数
           );
        }
        // Threshold back projection to obtain a binary image阈值分割反向投影图像得到二值图
        if (threshold>0.0)// 设置的阈值>0时，才进行阈值分割
            cv::threshold(result, result, 255.0*threshold, 255.0, cv::THRESH_BINARY);
        return result;
    }
};
#endif // CONTENTFINDER_H
