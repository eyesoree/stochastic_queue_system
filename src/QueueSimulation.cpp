void QueueSimulation::run() {
    simulationStartTime = currentTime;

    // Schedule the first arrival
    if (eventQueue.empty())
        scheduleArrival();

    while (!eventQueue.empty()) {
        Event ev = eventQueue.top();
        eventQueue.pop();

        double nextTime = ev.getTime();
        double timeDiff = nextTime - currentTime;

        if (timeDiff > 0) {
            totalQueueLength += waitingCustomers.size() * timeDiff;

            for (const auto& server : servers) {
                if (server.status == ServerStatus::BUSY)
                    totalServerBusy += timeDiff;
            }

            currentTime = nextTime;
        }

        // Process the event.
        if (ev.getType() == EventType::ARRIVAL)
            processArrival();
        else  // DEPARTURE
            processDeparture(ev.getID());
    }

    // Process any remaining waiting customers for final delay calculation.
    while (!waitingCustomers.empty()) {
        double arrivalTime = waitingCustomers.front();
        waitingCustomers.pop();
        totalDelay += (currentTime - arrivalTime);
    }
}

void QueueSimulation::processArrival() {
    ++totalArrivals;

    if (totalArrivals < maxCustomers)
        scheduleArrival();

    // Try to find an idle server
    for (auto& server : servers) {
        if (server.status == ServerStatus::IDLE) {
            // Assign customer to server
            server.status = ServerStatus::BUSY;
            scheduleDeparture(server.id);
            return;
        }
    }

    // All servers busy → try to enqueue customer
    if (waitingCustomers.size() < maxQueue)
        waitingCustomers.push(currentTime);  // Add to queue
    else
        ++totalRejected;  // Track rejected customers
}

void QueueSimulation::processDeparture(const size_t& serverId) {
    ++totalServiced;

    Server& server = servers[serverId];
    ++server.totalCustomer;
    server.status = ServerStatus::IDLE;

    if (waitingCustomers.empty())
        return;

    // Serve next waiting customer
    double arrivalTime = waitingCustomers.front();
    waitingCustomers.pop();

    // Record this customer's waiting time
    totalDelay += (currentTime - arrivalTime);

    // Re-assign server immediately
    server.status = ServerStatus::BUSY;
    scheduleDeparture(serverId);
}

void QueueSimulation::scheduleArrival() {
    double nextArrivalTime = currentTime + iA.getValue();
    eventQueue.push(Event(EventType::ARRIVAL, nextArrivalTime));
}

void QueueSimulation::scheduleDeparture(const size_t& id) {
    double departureTime = currentTime + sD.getValue();
    eventQueue.push(Event(EventType::DEPARTURE, departureTime, id));
}

void QueueSimulation::printStatistics() {
    double totalSimulationTime = currentTime - simulationStartTime;

    // Avoid division by zero
    if (totalSimulationTime <= 0 || totalServiced == 0) {
        cout << "\nInsufficient data for statistics.\n";
        return;
    }

    cout << "\n--- SIMULATION STATISTICS ---\n";
    cout << std::fixed << std::setprecision(4);

    std::cout << "Total simulation time: " << totalSimulationTime << "\n";
    std::cout << "Total arrivals: " << totalArrivals << "\n";
    std::cout << "Total customers served: " << totalServiced << "\n";
    std::cout << "Total customers rejected: " << totalRejected << "\n";

    // Calculate average statistics
    double avgQueueLength = totalQueueLength / totalSimulationTime;

    size_t customersInSystem = totalArrivals - totalRejected;
    double avgDelay = (customersInSystem > 0) ? (totalDelay / customersInSystem) : 0.0;

    // Calculate server utilization correctly - divide by total simulation time
    double avgServerUtilization = totalServerBusy / totalSimulationTime;

    std::cout << "Average queue length: " << avgQueueLength << "\n";
    std::cout << "Average customer waiting time: " << avgDelay << "\n";
    std::cout << "Average server utilization: " << (avgServerUtilization * 100) << "%\n";

    // Per-server statistics
    std::cout << "\n--- PER SERVER STATISTICS ---\n";
    for (size_t i = 0; i < servers.size(); i++)
        std::cout << "Server #" << i << ": Customers served: " << servers[i].totalCustomer << "\n";
}