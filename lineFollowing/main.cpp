#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <stdlib.h>


cv::Mat preprocessing(cv::Mat frame){
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

    cv::Mat blurFrame;
    cv::GaussianBlur(grayFrame, blurFrame, cv::Size(5,5), 0);

    cv::Mat edges;
    cv::Canny(blurFrame, edges, 50, 150);

    return edges;
}


cv::Mat segmentation(cv::Mat frame) {
    cv::Mat segmentedFrame;
    cv::Mat mask = cv::Mat::zeros(frame.size(), frame.type());

    cv::Point ROIPoints[1][3];
    ROIPoints[0][0] = cv::Point(0, frame.rows - 50);
    ROIPoints[0][1] = cv::Point(frame.cols / 2, frame.rows / 2);
    ROIPoints[0][2] = cv::Point(frame.cols, frame.rows - 50);
    const cv::Point* ppt[1] = {ROIPoints[0]};
    int npt[] = {3};

    cv::fillPoly(mask, ppt, npt, 1, cv::Scalar(255, 255, 255), 8);
    frame.copyTo(segmentedFrame, mask);

    return segmentedFrame;
}



int main( int argc, char** argv )
{
    cv::VideoCapture video;
    video.open("/Users/mac/Documents/img/highway.mp4");

    if(!video.isOpened()){
        std::cout << "error opening the video file" << std::endl;
        return -1;
    }

    cv::Mat frame;
    cv::Mat original;

    while(video.isOpened()){
        video >> frame;
        frame.copyTo(original);

        if(frame.empty()){
            std::cout << "end of the video" << std::endl;
            return -1;
        }

        frame = preprocessing(frame);
        frame = segmentation(frame);

        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(frame, lines, 2, CV_PI/180, 10, 100, 20);

        for(int i = 0; i < lines.size(); ++i) {
            cv::Vec4i l = lines[i];
            cv::line(original, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 3, cv::LINE_AA);
        }


        cv::imshow("highway", original);
        cv::waitKey(1);
    }

    return 0;
}
