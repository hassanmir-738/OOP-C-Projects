#include "../include/BusinessPassenger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

BusinessPassenger::BusinessPassenger()
    : Passenger(), loungeAccess(true), companyName("N/A") {}

BusinessPassenger::BusinessPassenger(const std::string& id, const std::string& pName, const std::string& passport,
                                     const std::string& pEmail, const std::string& pPhone, int points,
                                     bool lounge, const std::string& company)
    : Passenger(id, pName, passport, pEmail, pPhone, points),
      loungeAccess(lounge), companyName(company) {}

bool BusinessPassenger::getLoungeAccess() const { return loungeAccess; }
std::string BusinessPassenger::getCompanyName() const { return companyName; }
void BusinessPassenger::setLoungeAccess(bool lounge) { loungeAccess = lounge; }
void BusinessPassenger::setCompanyName(const std::string& company) { companyName = company; }

double BusinessPassenger::calculateFare(double flightBaseFare) const {
    return flightBaseFare * 1.60; // 1.6x business multiplier
}

int BusinessPassenger::getBaggageAllowance() const {
    return 35; // 35 kg
}

double BusinessPassenger::getLoyaltyMultiplier() const {
    return 1.5; // 1.5x loyalty points
}

double BusinessPassenger::calculateRefund(double farePaid, int daysBeforeDeparture) const {
    if (daysBeforeDeparture > 7) {
        return farePaid * 0.95; // 95% refund
    } else if (daysBeforeDeparture >= 2) {
        return farePaid * 0.80; // 80% refund
    } else {
        return farePaid * 0.40; // 40% refund within 48h
    }
}

void BusinessPassenger::displayDetails() const {
    std::cout << "\n---------------- [ BUSINESS PASSENGER ] ----------------\n";
    std::cout << "Passenger ID     : " << passengerId << "\n";
    std::cout << "Full Name        : " << name << "\n";
    std::cout << "Passport Number  : " << passportNumber << "\n";
    std::cout << "Contact Email    : " << email << "\n";
    std::cout << "Contact Phone    : " << phone << "\n";
    std::cout << "Loyalty Points   : " << loyaltyPoints << " (Earns 1.5x rate)\n";
    std::cout << "Baggage Allowance: " << getBaggageAllowance() << " kg\n";
    std::cout << "Lounge Access    : " << (loungeAccess ? "YES (VIP Executive Lounge)" : "NO") << "\n";
    std::cout << "Company Sponsor  : " << companyName << "\n";
    std::cout << "Refund Policy    : >7 days: 95% | 2-7 days: 80% | <2 days: 40%\n";
    std::cout << "--------------------------------------------------------\n";
}

std::string BusinessPassenger::getPassengerClass() const {
    return "Business";
}

std::string BusinessPassenger::serialize() const {
    // Format: BUSINESS|passengerId|name|passportNumber|email|phone|loyaltyPoints|loungeAccess|companyName
    std::ostringstream oss;
    oss << "BUSINESS|" << passengerId << "|" << name << "|" << passportNumber << "|"
        << email << "|" << phone << "|" << loyaltyPoints << "|"
        << (loungeAccess ? 1 : 0) << "|" << companyName;
    return oss.str();
}

std::shared_ptr<Passenger> BusinessPassenger::clone() const {
    return std::make_shared<BusinessPassenger>(*this);
}
