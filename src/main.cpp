/**
 * @file QueueSimulation.cpp
 * @brief Simulation of a queue with random arrival and service times.
 *
 * AUTHOR: Salih, Johaire C.
 * CREATED DATE: 03-23-2025 3:23 PM
 * TOTAL HOUR SPENT: 5
 *
 * This file implements a discrete event simulation for a queue system using
 * exponential distributions for inter-arrival and service times. The simulation
 * processes events until a specified number of customers have been served.
 */

#include "QueueSimulation.h"

void testScenarios(double iA, double sD, size_t nServer) {
    QueueSimulation simulation(iA, sD, 1000, nServer, 5);

    simulation.run();
    simulation.printStatistics();
}

int main() {
    testScenarios(1.0, 1.0, 1);  // iA == sD
    testScenarios(0.5, 1.0, 1);  // iA < sD
    testScenarios(2.0, 1.0, 1);  // iA > sD
    return 0;
}