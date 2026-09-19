#include "../include/DomesticFlight.h"
#include <iostream>
#include <iomanip>
#include <sstream>

DomesticFlight::DomesticFlight()
    : Flight(), stateTaxRate(0.05), isRegionalSubsidized(false), airportLuggageFee(15.0) {}

DomesticFlight::DomesticFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                               const std::string& depTime, int seats, double rate,
                               double taxRate, bool subsidized, double luggageFee)
    : Flight(fNum, orig, dest, depTime, seats, rate),
      stateTaxRate(taxRate), isRegionalSubsidized(subsidized), airportLuggageFee(luggageFee) {}

double DomesticFlight::getStateTaxRate() const { return stateTaxRate; }
bool DomesticFlight::getIsRegionalSubsidized() const { return isRegionalSubsidized; }
double DomesticFlight::getAirportLuggageFee() const { return airportLuggageFee; }

// Pricing calculation: Base rate + State Tax + Luggage fee - Regional subsidy (if any)
double DomesticFlight::calculateBaseFare() const {
    double fare = (baseRate * (1.0 + stateTaxRate)) + airportLuggageFee;
    if (isRegionalSubsidized) {
        fare -= 20.0; // $20 government subsidy
    }
    return (fare > 10.0) ? fare : 10.0; // minimum floor
}

void DomesticFlight::displayDetails() const {
    std::cout << "\n---------------- [ DOMESTIC FLIGHT ] ----------------\n";
    std::cout << "Flight Number    : " << flightNumber << "\n";
    std::cout << "Route            : " << origin << " -> " << destination << "\n";
    std::cout << "Departure Time   : " << departureDateTime << "\n";
    std::cout << "Seats (Avail/Tot): " << availableSeats << " / " << totalSeats << "\n";
    std::cout << "Base Rate        : $" << std::fixed << std::setprecision(2) << baseRate << "\n";
    std::cout << "State Tax Rate   : " << (stateTaxRate * 100.0) << "%\n";
    std::cout << "Airport Luggage  : $" << airportLuggageFee << "\n";
    std::cout << "Govt Subsidized  : " << (isRegionalSubsidized ? "YES (-$20.00)" : "NO") << "\n";
    std::cout << "Final Base Fare  : $" << calculateBaseFare() << "\n";
    std::cout << "Total Revenue    : $" << totalRevenue << "\n";
    std::cout << "-----------------------------------------------------\n";
}

std::string DomesticFlight::getFlightType() const {
    return "Domestic";
}

std::string DomesticFlight::serialize() const {
    // Format: DOMESTIC|flightNumber|origin|destination|departureDateTime|totalSeats|availableSeats|baseRate|totalRevenue|stateTaxRate|isRegionalSubsidized|airportLuggageFee
    std::ostringstream oss;
    oss << "DOMESTIC|" << flightNumber << "|" << origin << "|" << destination << "|"
        << departureDateTime << "|" << totalSeats << "|" << availableSeats << "|"
        << baseRate << "|" << totalRevenue << "|"
        << stateTaxRate << "|" << (isRegionalSubsidized ? 1 : 0) << "|" << airportLuggageFee;
    return oss.str();
}

std::shared_ptr<Flight> DomesticFlight::clone() const {
    return std::make_shared<DomesticFlight>(*this);
}
