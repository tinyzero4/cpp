#include "bus_manager.h"

void BusManager::AddBus(const string &bus, const vector<string> &stops) {
    buses_to_stops[bus] = stops;
    for (auto const &stop : stops) {
        stops_to_buses[stop].push_back(bus);
    }
};

BusesForStopResponse BusManager::GetBusesForStop(const string &stop) const {
    BusesForStopResponse response;
    if (stops_to_buses.count(stop) > 0) response.buses = stops_to_buses.at(stop);
    return response;
}

StopsForBusResponse BusManager::GetStopsForBus(const string &bus) const {
    StopsForBusResponse response;
    response.bus = bus;
    if (buses_to_stops.count(bus) > 0) response.stops = buses_to_stops.at(bus);
    response.stops_to_buses = stops_to_buses;
    return response;
}

AllBusesResponse BusManager::GetAllBuses() const {
    AllBusesResponse response;
    response.buses_to_stops = buses_to_stops;
    return response;
}