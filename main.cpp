#include "src/renderer.h"
#include "src/test.h"
#include "src/utils.h"
#include <map>
#include <iomanip>

void testParallelization(const std::vector<std::size_t>& testPlanes){
    headerResults(RESULT_PATH, omp_get_num_procs());
    for (auto test: testPlanes) {
        // GENERATION OF CIRCLES
        auto circles = parallelGenerateCircles(test * N_CIRCLES, WIDTH, HEIGHT, MIN_RADIUS, MAX_RADIUS);
        auto planes = parallelGeneratePlanes(test, circles, N_CIRCLES);

        printf("\nTEST PLANES: %llu\n", test);

        // TEST SEQUENTIAL
        double tSeq = sequentialRenderer(planes, test);
        printf("SEQUENTIAL Time: %f\n", tSeq);

        // TEST OPENMP
        std::map<std::size_t, double> tPars;
        std::map<std::size_t, double> speedUps;
        for (int i=2; i<=omp_get_num_procs(); i+=2) {
            // SET NUMBER OF THREADS
            omp_set_num_threads(i);

            // TEST PARALLEL
            double tPar = parallelRenderer(planes, test);
            printf("PARALLEL-%d Time: %f\n", i, tPar);

            double speedUp = tSeq / tPar;
            printf("PARALLEL-%d Speedup: %f \n", i, speedUp);

            // SAVE RESULTS
            tPars.insert(std::pair<std::size_t, double>(i, tPar));
            speedUps.insert(std::pair<std::size_t, double>(i, speedUp));
        }

        // WRITE RESULTS TO TXT FILE
        exportResults(RESULT_PATH, test, tSeq, tPars, speedUps);

        // DELETE ARRAY DYNAMIC ALLOCATED
        delete[] circles;
        delete[] planes;
    }
}

void testCircles(const std::vector<std::size_t>& testPlanes, const std::vector<std::size_t>& testCircles){
    headerResultsCircle(RESULT_CIRCLES_PATH);
    printf("\nTEST CIRCLES\n");
    for (auto testP: testPlanes) {
        printf("TEST PLANES: %llu\n", testP);

        for (auto testC : testCircles) {
            // GENERATION OF CIRCLES
            double start = omp_get_wtime();
            auto circles = sequentialGenerateCircles(testP * testC, WIDTH, HEIGHT, MIN_RADIUS, MAX_RADIUS);
            auto planes = sequentialGeneratePlanes(testP, circles, testC);
            double seqTime = omp_get_wtime() - start;
            printf("Sequential Time: %f\n", seqTime);

            // DELETE ARRAY DYNAMIC ALLOCATED
            delete[] circles;
            delete[] planes;

            // GENERATION OF CIRCLES
            start = omp_get_wtime();
            circles = parallelGenerateCircles(testP * testC, WIDTH, HEIGHT, MIN_RADIUS, MAX_RADIUS);
            planes = parallelGeneratePlanes(testP, circles, testC);
            double parTime = omp_get_wtime() - start;
            printf("Parallel Time: %f\n", parTime);

            // DELETE ARRAY DYNAMIC ALLOCATED
            delete[] circles;
            delete[] planes;

            // WRITE RESULTS TO CSV FILE
            exportResultsCircle(RESULT_CIRCLES_PATH, testP, testC, seqTime, parTime, seqTime/parTime);
        }

    }
}

int main() {
#ifdef _OPENMP
    printf("**OPENMP :: Number of cores/threads: %d**\n", omp_get_num_procs());
    omp_set_dynamic(0);
#endif

    // TEST PARALLELIZATION
    /*
    std::vector<std::size_t> testPlanes;
    for (std::size_t i = MIN_TEST; i <= MAX_TESTS; i += SPACE)
        testPlanes.push_back(i);
    testParallelization(testPlanes);
    */

    // n = 50, 500
    // N = 100, 1000, 10000
    // D = 256x256, 512x512, 1024x1024
    std::vector<std::size_t> testPlanesCircles {100, 1000, 10000};
    std::vector<std::size_t> testCirclesCircles {50, 500};
    testCircles(testPlanesCircles, testCirclesCircles);

    return 0;
}

