/** @file pi_approximation.cc
 * Solution for sheet 1 exercise 1.
 */
#include <iostream>
#include <random>

typedef std::mt19937 RandomGenerator;

/** Define global variables
 */
enum Config {
    NUM_RUNS = 1000000,
    RANDOM_SEED = 0
};

/** Calculate pi with a given number of steps
 *
 * @param nSteps the number of random points, used to calculate pi
 * @return the approximated value of pi
 */
double CalculatePi(const long nSteps, RandomGenerator &rng)
{
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    long counter(0);
    double xVal(0.0),
           yVal(0.0);
    for (long i=0; i<nSteps; i++){
        xVal = dist(rng);
        yVal = dist(rng);
        if ((xVal * xVal + yVal * yVal) < 1.0){
            counter++;
        }
    }

    return 4.0*((double)counter/(double)nSteps);
}

int main(int argc, char* argv[])
{
    std::random_device rd;
    RandomGenerator rng(rd());
    std::cout << "Pi calculated with " << Config::NUM_RUNS << " steps:" << std::endl;
    std::cout << CalculatePi(Config::NUM_RUNS, rng) << std::endl;
    return 0;
}
