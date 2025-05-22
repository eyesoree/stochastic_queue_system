#include "QueueSimulation.h"

int main() {
    double iA_mean = 1.0;             // mean for inter-arrival generation
    double sD_mean = 1.0;             // mean for service duration generation
    size_t totalServed = 1000;        // number of customers to complete service
    size_t numberOfServers = 1;       // number of servers
    size_t waitingQueueCapacity = 5;  // maximum size of the waiting queue

    QueueSimulation simulation(iA_mean,
                               sD_mean,
                               totalServed,
                               numberOfServers,
                               waitingQueueCapacity);
    simulation.run();
    simulation.printStatistics();

    return 0;
}