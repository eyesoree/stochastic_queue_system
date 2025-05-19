#ifndef QUEUE_SIMULATION_H
#define QUEUE_SIMULATION_H

#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

#include "Event.h"
#include "Server.h"
#include "randomExpoMean.h"

class QueueSimulation {
   private:
    // Random number generators
    RandomExpoMean iA;  // Inter-arrival time generator
    RandomExpoMean sD;  // Service duration generator

    // Simulation parameters
    size_t maxQueue = 5;       // Maximum queue size
    size_t numServers;         // Total number of servers
    size_t maxCustomers;       // Maximum number of customers to simulate
    double currentTime = 0.0;  // Current simulation time

    // Data structures
    std::queue<double> waitingCustomers;                                             // Queue to hold arrival time of waiting customers
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> eventQueue;  // Event queue
    std::vector<Server> servers;                                                     // List of all servers

    // Statistics variables
    double simulationStartTime = 0.0;  // When simulation started
    double lastEventTime = 0.0;        // Time of the last event processed (for statistics)
    double totalQueueLength = 0.0;     // Time-weighted sum of queue lengths
    double totalDelay = 0.0;           // Total waiting time of all customers
    double totalServerBusy = 0.0;      // Total time servers were busy
    size_t totalArrivals = 0;          // Total number of arrivals
    size_t totalServiced = 0;          // Total number of customers who completed service

    // Event handlers
    void processArrival();
    void processDeparture(const size_t& serverId);

    // Event schedulers
    void scheduleArrival();                    // Schedule next arrival
    void scheduleDeparture(const size_t& id);  // Schedule next departure

    // Statistics update
    void updateTimeWeightedStats();

   public:
    // Constructor
    QueueSimulation(const double& interArrivalMean,
                    const double& serviceDurationMean,
                    const size_t& maxCustomersToServe = 10,
                    const size_t& numberOfServers = 1,
                    const size_t& maxWaitingQueue = 1000)
        : iA(interArrivalMean),
          sD(serviceDurationMean),
          maxCustomers(maxCustomersToServe),
          numServers(numberOfServers),
          maxQueue(maxWaitingQueue) {
        servers.resize(numServers);
        for (size_t i = 0; i < numServers; i++)
            servers[i].id = i;
    }

    // Main simulation methods
    void run();
    void printStatistics();
};

#include "QueueSimulation.cpp"
#endif