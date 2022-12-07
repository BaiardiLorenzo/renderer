#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>

int main() {
    std::cout << "Hello, World!" << std::endl;
    cv::Mat whiteMatrix(200, 200, CV_8UC3, cv::Scalar(255, 255, 255));//Declaring a white matrix
    cv::Point center(100, 100);//Declaring the center point
    int radius = 50; //Declaring the radius
    cv::Scalar line_Color(0, 0, 0);//Color of the circle
    int thickness = 2;//thickens of the line
    cv::namedWindow("whiteMatrix");//Declaring a window to show the circle
    circle(whiteMatrix, center,radius, line_Color, thickness);//Using circle()function to draw the line//
    imshow("WhiteMatrix", whiteMatrix);//Showing the circle//
    cv::waitKey(0);//Waiting for Keystroke//
    return 0;
}
