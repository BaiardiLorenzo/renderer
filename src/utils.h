#include <map>
#include <iomanip>

#ifndef RENDERER_CUDA_UTILS_H
#define RENDERER_CUDA_UTILS_H

void headerResults(const std::string& filename, int nThreads);

void exportResults(const std::string& filename, std::size_t test, double tSeq, const std::map<std::size_t, double>& tPars,
                   std::map<std::size_t,double> speedUps);

void headerResultsCircle(const std::string& filename);

void exportResultsCircle(const std::string& filename, std::size_t testP, std::size_t testC, double tSeq, double tPar, double speedUp);

#endif //RENDERER_CUDA_UTILS_H
