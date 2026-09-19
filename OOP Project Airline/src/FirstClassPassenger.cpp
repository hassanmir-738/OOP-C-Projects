#include "../include/FirstClassPassenger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

FirstClassPassenger::FirstClassPassenger()
    : Passenger(), chauffeurService(true), suitePreference("Royal Suite") {}

FirstClassPassenger::FirstClassPassenger(const std::string& id, const std::string& pName, const std::string& passport,
                                         const std::string& pEmail, const std::string& pPhone, int points,
                                         bool chauffeur, const std::string& suite)
    : Passenger(id, pName, passport, pEmail, pPhone, points),
      chauffeurService(chauffeur), suitePreference(suite) {}

bool FirstClassPassenger::getChauffeurService() const { return chauffeurService; }
std::string FirstClassPassenger::getSuitePreference() const { return suitePreference; }
void FirstClassPassenger::setChauffeurService(bool chauffeur) { chauffeurService = chauffeur; }
void FirstClassPassenger::setSuitePreference(const std::string& suite) { suitePreference = suite; }

double FirstClassPassenger::calculateFare(double flightBaseFare) const {
    return flightBaseFare * 2.40; // 2.4x luxury multiplier
}

int FirstClassPassenger::getBaggageAllowance() const {
    return 50; // 50 kg luxury baggage
}

double FirstClassPassenger::getLoyaltyMultiplier() const {
    return 2.0; // 2.0x loyalty points
}

double FirstClassPassenger::calculateRefund(double farePaid, int daysBeforeDeparture) const {
    if (daysBeforeDeparture > 7) {
        return farePaid * 1.00; // 100% full refund
    } else if (daysBeforeDeparture >= 2) {
        return farePaid * 0.90; // 90% refund
    } else {
        return farePaid * 0.75; // 75% refund even on last minute
    }
}

void FirstClassPassenger::displayDetails() const {
    std::cout << "\n-------------- [ FIRST CLASS PASSENGER ] --------------\n";
    std::cout << "Passenger ID     : " << passengerId << "\n";
    std::cout << "Full Name        : " << name << "\n";
    std::cout << "Passport Number  : " << passportNumber << "\n";
    std::cout << "Contact Email    : " << email << "\n";
    std::cout << "Contact Phone    : " << phone << "\n";
    std::cout << "Loyalty Points   : " << loyaltyPoints << " (Earns 2.0x rate)\n";
    std::cout << "Baggage Allowance: " << getBaggageAllowance() << " kg\n";
    std::cout << "Chauffeur Pickup : " << (chauffeurService ? "YES (Luxury Sedan Included)" : "NO") << "\n";
    std::cout << "Private Suite    : " << suitePreference << "\n";
    std::cout << "Refund Policy    : >7 days: 100% | 2-7 days: 90% | <2 days: 75%\n";
    std::cout << "-------------------------------------------------------\n";
}

std::string FirstClassPassenger::getPassengerClass() const {
    return "First Class";
}

std::string FirstClassPassenger::serialize() const {
    // Format: FIRSTCLASS|passengerId|name|passportNumber|email|phone|loyaltyPoints|chauffeurService|suitePreference
    std::ostringstream oss;
    oss << "FIRSTCLASS|" << passengerId << "|" << name << "|" << passportNumber << "|"
        << email << "|" << phone << "|" << loyaltyPoints << "|"
        << (chauffeurService ? 1 : 0) << "|" << suitePreference;
    return oss.str();
}

std::shared_ptr<Passenger> FirstClassPassenger::clone() const {
    return std::make_shared<FirstClassPassenger>(*this);
}
