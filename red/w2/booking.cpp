#include <iomanip>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <stdint.h>
#include <queue>

using namespace std;

struct Booking {
    string hotel_name;
    int64_t time;
    int64_t client_id;
    int64_t room_count;
};

ostream &operator<<(ostream &ostream, const Booking &booking) {
    ostream << booking.time << "-" << booking.hotel_name << "-" << booking.client_id << "-" << booking.room_count;
    return ostream;
}

class BookingManager {

private:
    static const int TIME_WINDOWS_SIZE = 86400;

    int64_t cur_time;

    deque<Booking> bookings_;

    map<string, int64_t> rooms_booked_;

    map<string, set<int64_t>> clients_;

    map<string, map<int, int>> hotel_clients_bookings_;

    int64_t getLastDayStartTime() const {
        return cur_time - TIME_WINDOWS_SIZE;
    }

public:

    BookingManager() : bookings_(), rooms_booked_(), clients_(), cur_time(0) {}

    void addBooking(const int64_t time, const string &hotel_name, const int64_t client_id, const int64_t room_count) {
        bookings_.push_back({hotel_name, time, client_id, room_count});

        int64_t prev_time = cur_time;
        cur_time = time;
        if (time != prev_time) {
            int64_t day_start_time = getLastDayStartTime();
            while (!bookings_.empty() && bookings_.front().time <= day_start_time) {
                auto booking = bookings_.front();
                bookings_.pop_front();

                rooms_booked_[booking.hotel_name] -= booking.room_count;

                --hotel_clients_bookings_[booking.hotel_name][booking.client_id];
                if (hotel_clients_bookings_[booking.hotel_name][booking.client_id] == 0) {
                    clients_[booking.hotel_name].erase(booking.client_id);
                }
            }
        }

        rooms_booked_[hotel_name] += room_count;
        hotel_clients_bookings_[hotel_name][client_id]++;
        clients_[hotel_name].insert(client_id);
    }

    u_int64_t lastDayClientsCount(const string &hotel_name) const {
        if (clients_.count(hotel_name) == 0) return 0;
        return clients_.at(hotel_name).size();
    }

    int64_t lastDayRoomsBooked(const string &hotel_name) const {
        if (rooms_booked_.count(hotel_name) == 0) return 0;
        return rooms_booked_.at(hotel_name);
    }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    BookingManager bookingManager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string op;
        cin >> op;

        string hotel_name;
        if (op == "BOOK") {
            int64_t time, client_id, room_count;
            cin >> time >> hotel_name >> client_id >> room_count;
            bookingManager.addBooking(time, hotel_name, client_id, room_count);
        } else if (op == "CLIENTS") {
            cin >> hotel_name;
            cout << bookingManager.lastDayClientsCount(hotel_name) << endl;
        } else if (op == "ROOMS") {
            cin >> hotel_name;
            cout << bookingManager.lastDayRoomsBooked(hotel_name) << endl;
        }
    }
    return 0;
}