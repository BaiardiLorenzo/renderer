#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "test.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <random>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

struct Circle {
    cv::Scalar color;
    cv::Point center;
    int r;
};

// GENERATION
Circle* sequentialGenerateCircles(std::size_t n, int width, int height, int minRadius, int maxRadius);

cv::Mat* sequentialGeneratePlanes(std::size_t nPlanes, Circle circles[], std::size_t nCircles);

Circle* parallelGenerateCircles(std::size_t n, int width, int height, int minRadius, int maxRadius);

cv::Mat* parallelGeneratePlanes(std::size_t nPlanes, Circle circles[], std::size_t nCircles);


// RENDERING
double sequentialRenderer(cv::Mat planes[], std::size_t nPlanes);

double parallelRenderer(cv::Mat planes[], std::size_t nPlanes);

#endif //RENDERER_RENDERER_H
