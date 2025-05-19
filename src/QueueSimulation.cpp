void QueueSimulation::run() {
    simulationStartTime = currentTime;

    // Make sure there's at least one arrival event to start
    if (eventQueue.empty())
        scheduleArrival();

    // Main simulation loop
    while (totalServiced < maxCustomers && !eventQueue.empty()) {
        Event ev = eventQueue.top();
        eventQueue.pop();

        updateTimeWeightedStats();
        currentTime = ev.getTime();

        // Process event based on type
        if (ev.getType() == EventType::ARRIVAL)
            processArrival();
        else  // DEPARTURE
            processDeparture(ev.getID());
    }

    updateTimeWeightedStats();
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

    // All servers busy → enqueue customer
    if (waitingCustomers.size() < maxQueue)
        waitingCustomers.push(currentTime);  // Add to queue only if there's room
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

void QueueSimulation::updateTimeWeightedStats() {
    double timeDiff = currentTime - lastEventTime;

    if (timeDiff <= 0) {
        // No time has passed since last event
        lastEventTime = currentTime;
        return;
    }

    totalQueueLength += waitingCustomers.size() * timeDiff;

    int busyServers = 0;
    for (const auto& server : servers) {
        if (server.status == ServerStatus::BUSY) {
            busyServers++;
        }
    }

    totalServerBusy += busyServers * timeDiff;
    lastEventTime = currentTime;
}

void QueueSimulation::printStatistics() {
    double totalSimulationTime = currentTime - simulationStartTime;

    // Avoid division by zero
    if (totalSimulationTime <= 0 || totalServiced == 0) {
        std::cout << "\nInsufficient data for statistics.\n";
        return;
    }

    std::cout << "\n--- SIMULATION STATISTICS ---\n";
    std::cout << std::fixed << std::setprecision(4);

    std::cout << "Total simulation time: " << totalSimulationTime << "\n";
    std::cout << "Total arrivals: " << totalArrivals << "\n";
    std::cout << "Total customers served: " << totalServiced << "\n";

    // Calculate average statistics
    double avgQueueLength = totalQueueLength / totalSimulationTime;
    double avgDelay = totalDelay / totalServiced;
    double avgServerUtilization = (totalServerBusy / numServers) / totalSimulationTime;

    std::cout << "Average queue length: " << avgQueueLength << "\n";
    std::cout << "Average customer waiting time: " << avgDelay << "\n";
    std::cout << "Average server utilization: " << (avgServerUtilization * 100) << "%\n";

    // Per-server statistics
    std::cout << "\n--- PER SERVER STATISTICS ---\n";
    for (size_t i = 0; i < servers.size(); i++)
        std::cout << "Server #" << i << ": Customers served: " << servers[i].totalCustomer << "\n";
}