int main() {
    QueueSimulation simulation(0.1, 0.2, 10000, 10);
    simulation.run();
    simulation.printStatistics();

    return 0;
}