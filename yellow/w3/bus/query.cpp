#include "query.h"

istream& operator >> (istream& is, Query& q) {
    string type = "";
    cin >> type;
    if (type == "NEW_BUS") {
        q.type = QueryType::NewBus;
        cin >> q.bus;
        int stop_count;
        cin >> stop_count;
        vector<string>& stops = q.stops;
        stops.resize(stop_count);
        for (string& stop : stops) {
            cin >> stop;
        }
    } else if (type == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        cin >> q.stop;
    } else if (type == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        cin >> q.bus;
    } else if (type == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }
    return is;
}

