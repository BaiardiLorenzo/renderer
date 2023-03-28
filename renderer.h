#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include <opencv2/core/types.hpp>
#include <omp.h>
#include <cstdlib>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define WIDTH 1024
#define HEIGHT 1024
#define TRANSPARENT cv::Scalar(255, 255, 255, 0)
#define ALPHA 0.5

#define MAX_RADIUS 200
#define MIN_RADIUS 100

struct Circle {
    cv::Scalar color;
    cv::Point center;
    int r;
};

void generateCircles(Circle circles[], unsigned long long n);

double rendererSequential(Circle circles[], unsigned long long nPlanes, unsigned long long nCircles);
void combinePlanesSequential(cv::Mat* src1, cv::Mat* src2);

double rendererParallel(Circle circles[], unsigned long long nPlanes, unsigned long long nCircles);
void combinePlanesParallel(cv::Mat* result, cv::Mat planes[], unsigned long long nPlanes);

#endif //RENDERER_RENDERER_H
