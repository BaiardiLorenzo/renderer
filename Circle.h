#ifndef RENDERER_CIRCLE_H
#define RENDERER_CIRCLE_H


#include <opencv2/core/types.hpp>

class Circle {
    private:
        cv::Point center;
        cv::Scalar color;
        int r;
    public:
    Circle(cv::Point center, int r, cv::Scalar color) {
        this->center = center;
        this->r = r;
        this->color = color;
    }
};


#endif //RENDERER_CIRCLE_H
