/**
 * @file QueueSimulation.cpp
 * @brief Simulation of a queue with random arrival and service times.
 *
 * AUTHOR: Salih, Johaire C.
 * CREATED DATE: 03-23-2025 3:23 PM
 * TOTAL HOUR SPENT: 2
 *
 * This file implements a discrete event simulation for a queue system using
 * exponential distributions for inter-arrival and service times. The simulation
 * processes events until a specified number of customers have been served.
 */

#include "QueueSimulation.h"

int main() {
    double iA_mean = 0.25;            // mean for inter-arrival generation
    double sD_mean = 0.2;             // mean for service duration generation
    size_t totalCustomer = 10000;     // maximum customers to serve
    size_t numberOfServers = 1;       // number of server to utilize
    size_t waitingQueueCapacity = 5;  // maximum customer that the waiting queue can hold

    QueueSimulation simulation(iA_mean, sD_mean, totalCustomer, numberOfServers, waitingQueueCapacity);
    simulation.run();
    simulation.printStatistics();

    return 0;
}