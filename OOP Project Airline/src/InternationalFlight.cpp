#include "../include/InternationalFlight.h"
#include <iostream>
#include <iomanip>
#include <sstream>

InternationalFlight::InternationalFlight()
    : Flight(), visaRequired(true), customsDuty(35.0), passportValidityMonths(6) {}

InternationalFlight::InternationalFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                                         const std::string& depTime, int seats, double rate,
                                         bool visaReq, double customs, int validityMonths)
    : Flight(fNum, orig, dest, depTime, seats, rate),
      visaRequired(visaReq), customsDuty(customs), passportValidityMonths(validityMonths) {}

bool InternationalFlight::getVisaRequired() const { return visaRequired; }
double InternationalFlight::getCustomsDuty() const { return customsDuty; }
int InternationalFlight::getPassportValidityMonths() const { return passportValidityMonths; }

// Pricing calculation: Base rate * 1.25 (international operations multiplier) + customs duty + ($45 if visa assistance required)
double InternationalFlight::calculateBaseFare() const {
    double fare = (baseRate * 1.25) + customsDuty;
    if (visaRequired) {
        fare += 45.0; // Visa processing surcharge
    }
    return fare;
}

void InternationalFlight::displayDetails() const {
    std::cout << "\n------------- [ INTERNATIONAL FLIGHT ] -------------\n";
    std::cout << "Flight Number    : " << flightNumber << "\n";
    std::cout << "Route            : " << origin << " -> " << destination << "\n";
    std::cout << "Departure Time   : " << departureDateTime << "\n";
    std::cout << "Seats (Avail/Tot): " << availableSeats << " / " << totalSeats << "\n";
    std::cout << "Base Rate        : $" << std::fixed << std::setprecision(2) << baseRate << "\n";
    std::cout << "Intl Multiplier  : 1.25x\n";
    std::cout << "Customs Duty     : $" << customsDuty << "\n";
    std::cout << "Visa Required    : " << (visaRequired ? "YES (+$45 surcharge)" : "NO") << "\n";
    std::cout << "Min Passport Val : " << passportValidityMonths << " months\n";
    std::cout << "Final Base Fare  : $" << calculateBaseFare() << "\n";
    std::cout << "Total Revenue    : $" << totalRevenue << "\n";
    std::cout << "-----------------------------------------------------\n";
}

std::string InternationalFlight::getFlightType() const {
    return "International";
}

std::string InternationalFlight::serialize() const {
    // Format: INTERNATIONAL|flightNumber|origin|destination|departureDateTime|totalSeats|availableSeats|baseRate|totalRevenue|visaRequired|customsDuty|passportValidityMonths
    std::ostringstream oss;
    oss << "INTERNATIONAL|" << flightNumber << "|" << origin << "|" << destination << "|"
        << departureDateTime << "|" << totalSeats << "|" << availableSeats << "|"
        << baseRate << "|" << totalRevenue << "|"
        << (visaRequired ? 1 : 0) << "|" << customsDuty << "|" << passportValidityMonths;
    return oss.str();
}

std::shared_ptr<Flight> InternationalFlight::clone() const {
    return std::make_shared<InternationalFlight>(*this);
}
