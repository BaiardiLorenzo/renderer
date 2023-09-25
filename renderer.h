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
#define ALPHA 0.4

#define MAX_RADIUS 200
#define MIN_RADIUS 100


struct Circle {
    cv::Scalar color;
    cv::Point center;
    int r;
};

void generateCircles(Circle circles[], std::size_t n);

double rendererSequential(Circle circles[], std::size_t nPlanes, std::size_t nCircles);

cv::Mat combinePlanesSequential(cv::Mat planes[], std::size_t nPlanes);

double rendererParallel(Circle circles[], std::size_t nPlanes, std::size_t nCircles);

cv::Mat combinePlanesParallel(cv::Mat planes[], std::size_t nPlanes);

#endif //RENDERER_RENDERER_H
