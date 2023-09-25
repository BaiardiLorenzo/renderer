#include <iostream>
#include "renderer.h"
#include <fstream>
#include <vector>

#define MAX_TESTS 100
#define SPACE 10
#define MIN_TEST 10
#define N_CIRCLES 100


int main() {
#ifdef _OPENMP
    printf("Core/Threads: %d\n", omp_get_num_procs());
    omp_set_dynamic(0);
#endif
    std::ofstream outfile;
    std::string fileName = "../test.csv";
    outfile.open(fileName, std::ios::out | std::ios::app);
    if(outfile.is_open())
        outfile << "THREADS ; SPEEDUP ; TEST ; SEQ ; PAR \n";
    outfile.close();

    std::vector<std::size_t> testPlanes;
    for (std::size_t i = MIN_TEST; i <= MAX_TESTS; i += SPACE)
        testPlanes.push_back(i);

    for (int i=2; i<=omp_get_num_procs(); i*=2) {
        //SET NUMBER OF THREADS
        omp_set_num_threads(i);
        printf("TEST with Core/Threads: %d\n", i);
        for (auto test: testPlanes) {
            // GENERATION OF CIRCLES
            std::size_t n = test * N_CIRCLES;
            auto* circles = new Circle[n];
            generateCircles(circles, n);

            // TEST SEQUENTIAL AND PARALLEL
            double t1 = rendererSequential(circles, test, N_CIRCLES);
            double t2 = rendererParallel(circles, test, N_CIRCLES);

            delete[] circles;

            double speedUp = t1/t2;
            printf("SPEEDUP TEST %llu: %f \n\n", test, speedUp);

            // WRITE TO TEXT
            outfile.open(fileName, std::ios::out | std::ios::app);
            if(outfile.is_open())
                outfile<<i<<";"<<speedUp<<";"<<test<<";"<<t1<<";"<<t2<<"\n";
            outfile.close();
        }
    }
    return 0;
}

