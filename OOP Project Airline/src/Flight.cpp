#include "../include/Flight.h"
#include <iomanip>

Flight::Flight()
    : flightNumber(""), origin(""), destination(""), departureDateTime(""),
      totalSeats(0), availableSeats(0), baseRate(0.0), totalRevenue(0.0) {}

Flight::Flight(const std::string& fNum, const std::string& orig, const std::string& dest,
               const std::string& depTime, int seats, double rate)
    : flightNumber(fNum), origin(orig), destination(dest), departureDateTime(depTime),
      totalSeats(seats), availableSeats(seats), baseRate(rate), totalRevenue(0.0) {}

Flight::~Flight() {
    // Base class virtual destructor ensures proper cleanup of derived classes
}

std::string Flight::getFlightNumber() const { return flightNumber; }
std::string Flight::getOrigin() const { return origin; }
std::string Flight::getDestination() const { return destination; }
std::string Flight::getDepartureDateTime() const { return departureDateTime; }
int Flight::getTotalSeats() const { return totalSeats; }
int Flight::getAvailableSeats() const { return availableSeats; }
int Flight::getBookedSeats() const { return totalSeats - availableSeats; }
double Flight::getBaseRate() const { return baseRate; }
double Flight::getTotalRevenue() const { return totalRevenue; }

double Flight::getOccupancyRate() const {
    if (totalSeats == 0) return 0.0;
    return (static_cast<double>(getBookedSeats()) / totalSeats) * 100.0;
}

void Flight::setOrigin(const std::string& orig) { origin = orig; }
void Flight::setDestination(const std::string& dest) { destination = dest; }
void Flight::setDepartureDateTime(const std::string& depTime) { departureDateTime = depTime; }
void Flight::setBaseRate(double rate) { baseRate = rate; }

bool Flight::bookSeat() {
    if (availableSeats > 0) {
        availableSeats--;
        return true;
    }
    return false;
}

bool Flight::releaseSeat() {
    if (availableSeats < totalSeats) {
        availableSeats++;
        return true;
    }
    return false;
}

void Flight::addRevenue(double amount) {
    if (amount > 0) {
        totalRevenue += amount;
    }
}

void Flight::refundRevenue(double amount) {
    totalRevenue -= amount;
    if (totalRevenue < 0) totalRevenue = 0.0;
}

// Operator Overloading: == compares flights by flightNumber
bool Flight::operator==(const Flight& other) const {
    return this->flightNumber == other.flightNumber;
}

// Operator Overloading: < compares flights by revenue for descending sort
bool Flight::operator<(const Flight& other) const {
    return this->totalRevenue > other.totalRevenue;
}

// Friend Function: Stream insertion operator<<
std::ostream& operator<<(std::ostream& os, const Flight& flight) {
    os << "[" << flight.getFlightType() << "] "
       << flight.flightNumber << " | "
       << flight.origin << " -> " << flight.destination << " | "
       << flight.departureDateTime << " | Seats: "
       << flight.availableSeats << "/" << flight.totalSeats << " available | Fare: $"
       << std::fixed << std::setprecision(2) << flight.calculateBaseFare();
    return os;
}
