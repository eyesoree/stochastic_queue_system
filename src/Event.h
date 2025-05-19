#ifndef EVENT_H
#define EVENT_H

enum class EventType {
    ARRIVAL,
    DEPARTURE
};

class Event {
   private:
    double time;
    EventType type;
    size_t serverID;

   public:
    Event(const EventType& type, const double& time, const size_t& id = -1)
        : time(time), type(type), serverID(id) {}

    // Getters
    double getTime() const { return time; }
    EventType getType() const { return type; }
    size_t getID() const { return serverID; }

    // Setters
    void setTime(const double& newTime) { time = newTime; }
    void setType(const EventType& newType) { type = newType; }
    void setID(const size_t& newID) { serverID = newID; }

    // Operator for priority queue
    bool operator>(const Event& other) const { return time > other.time; }
};

#endif