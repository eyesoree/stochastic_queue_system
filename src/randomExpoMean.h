/*
 * File:   RandomGenExpoMean.h
 * Author: jojo
 *
 * Created on July 22, 2012, 2:30 PM
 */

#ifndef RANDOMGENEXPOMEAN_H
#define RANDOMGENEXPOMEAN_H
#include <cmath>
#include <cstdlib>
#include <random>

using namespace std;

class RandomExpoMean {
   public:
    RandomExpoMean(double mean);
    void setMean(double mean);
    double getValue();
    double getAverageMean() const { return average / count; }

   private:
    double randomExpoMean;
    double Globalmean;
    double average;
    int count;
    std::random_device rd;
    std::mt19937 gen;
    std::exponential_distribution<> dist;
    void generate();
};

void RandomExpoMean::generate() {
    randomExpoMean = dist(gen);
}

RandomExpoMean::RandomExpoMean(double mean)
    : Globalmean(mean), count(0), average(0), gen(rd()), dist(1.0 / mean)  // lambda = 1/mean for exponential distribution
{
}

void RandomExpoMean::setMean(double mean) {
    Globalmean = mean;
    dist = std::exponential_distribution<>(1.0 / mean);
}

double RandomExpoMean::getValue() {
    count++;
    generate();
    average += randomExpoMean;
    return randomExpoMean;
}

#endif /* RANDOMGENEXPOMEAN_H */