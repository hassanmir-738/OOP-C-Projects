#include "../include/CharterFlight.h"
#include <iostream>
#include <iomanip>
#include <sstream>

CharterFlight::CharterFlight()
    : Flight(), contractHolder("N/A"), cateringIncluded(true), hourlyRate(1000.0), flightHours(2) {}

CharterFlight::CharterFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                             const std::string& depTime, int seats, double rate,
                             const std::string& holder, bool catering, double hrRate, int hours)
    : Flight(fNum, orig, dest, depTime, seats, rate),
      contractHolder(holder), cateringIncluded(catering), hourlyRate(hrRate), flightHours(hours) {}

std::string CharterFlight::getContractHolder() const { return contractHolder; }
bool CharterFlight::getCateringIncluded() const { return cateringIncluded; }
double CharterFlight::getHourlyRate() const { return hourlyRate; }
int CharterFlight::getFlightHours() const { return flightHours; }

// Pricing calculation: Flat charter operational cost divided per seat + Catering surcharge (if any)
double CharterFlight::calculateBaseFare() const {
    int seats = (totalSeats > 0) ? totalSeats : 1;
    double perSeatCharterCost = (hourlyRate * flightHours) / seats;
    double fare = baseRate + perSeatCharterCost;
    if (cateringIncluded) {
        fare += 60.0; // VIP Gourmet Catering
    }
    return fare;
}

void CharterFlight::displayDetails() const {
    std::cout << "\n--------------- [ CHARTER FLIGHT ] ----------------\n";
    std::cout << "Flight Number    : " << flightNumber << "\n";
    std::cout << "Route            : " << origin << " -> " << destination << "\n";
    std::cout << "Departure Time   : " << departureDateTime << "\n";
    std::cout << "Seats (Avail/Tot): " << availableSeats << " / " << totalSeats << "\n";
    std::cout << "Base Rate        : $" << std::fixed << std::setprecision(2) << baseRate << "\n";
    std::cout << "Contract Holder  : " << contractHolder << "\n";
    std::cout << "Charter Duration : " << flightHours << " hrs @ $" << hourlyRate << "/hr\n";
    std::cout << "VIP Catering     : " << (cateringIncluded ? "YES (Included)" : "NO") << "\n";
    std::cout << "Final Base Fare  : $" << calculateBaseFare() << " / seat\n";
    std::cout << "Total Revenue    : $" << totalRevenue << "\n";
    std::cout << "---------------------------------------------------\n";
}

std::string CharterFlight::getFlightType() const {
    return "Charter";
}

std::string CharterFlight::serialize() const {
    // Format: CHARTER|flightNumber|origin|destination|departureDateTime|totalSeats|availableSeats|baseRate|totalRevenue|contractHolder|cateringIncluded|hourlyRate|flightHours
    std::ostringstream oss;
    oss << "CHARTER|" << flightNumber << "|" << origin << "|" << destination << "|"
        << departureDateTime << "|" << totalSeats << "|" << availableSeats << "|"
        << baseRate << "|" << totalRevenue << "|"
        << contractHolder << "|" << (cateringIncluded ? 1 : 0) << "|"
        << hourlyRate << "|" << flightHours;
    return oss.str();
}

std::shared_ptr<Flight> CharterFlight::clone() const {
    return std::make_shared<CharterFlight>(*this);
}
