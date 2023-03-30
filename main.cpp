#include <iostream>
#include "renderer.h"
#include <vector>

#define MAX_TESTS 200
#define N_CIRCLES 50

int main() {
#ifdef _OPENMP
    int threads = omp_get_num_procs();
    printf("Core/Threads: %d\n", threads);
    omp_set_dynamic(0);
#endif
    std::vector<double> sequentialTimes;
    std::vector<double> parallelTimes;
    std::vector<int> testPlanes;
    for(int i=10; i <= MAX_TESTS; i+=10)
        testPlanes.push_back(i * threads);

    for (int test: testPlanes) {
        const unsigned long long n = test * N_CIRCLES;
        Circle circles[n];
        generateCircles(circles, n);
        double t1 = rendererSequential(circles, test, N_CIRCLES);
        double t2 = rendererParallel(circles, test, N_CIRCLES);
        printf("SPEEDUP TEST %d: %f \n", test, t1 / t2);
        sequentialTimes.push_back(t1);
        parallelTimes.push_back(t2);
        printf("\n");
    }
}

