#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "test.h"
#include <omp.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// AoS
struct Circle {
    cv::Scalar color;
    cv::Point center;
    int r;
};

// SoA
struct Circles{
    cv::Scalar* colors;
    cv::Point* centers;
    int* rs;
};


Circle* generateCircles(std::size_t n);

Circles* generateSoACircles(std::size_t n);

double rendererSequential(Circle circles[], std::size_t nPlanes, std::size_t nCircles);

double rendererSoASequential(Circles* circles, std::size_t nPlanes, std::size_t nCircles);

cv::Mat combinePlanesSequential(cv::Mat planes[], std::size_t nPlanes);

double rendererParallel(Circle circles[], std::size_t nPlanes, std::size_t nCircles);

double rendererSoAParallel(Circles* circles, std::size_t nPlanes, std::size_t nCircles);

cv::Mat combinePlanesParallel(cv::Mat planes[], std::size_t nPlanes);

#endif //RENDERER_RENDERER_H
