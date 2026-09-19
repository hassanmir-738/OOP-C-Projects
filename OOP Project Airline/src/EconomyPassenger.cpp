#include "../include/EconomyPassenger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

EconomyPassenger::EconomyPassenger()
    : Passenger(), mealPreferenceVegan(false) {}

EconomyPassenger::EconomyPassenger(const std::string& id, const std::string& pName, const std::string& passport,
                                   const std::string& pEmail, const std::string& pPhone, int points,
                                   bool veganMeal)
    : Passenger(id, pName, passport, pEmail, pPhone, points),
      mealPreferenceVegan(veganMeal) {}

bool EconomyPassenger::getMealPreferenceVegan() const { return mealPreferenceVegan; }
void EconomyPassenger::setMealPreferenceVegan(bool vegan) { mealPreferenceVegan = vegan; }

double EconomyPassenger::calculateFare(double flightBaseFare) const {
    return flightBaseFare * 1.0; // Standard base fare
}

int EconomyPassenger::getBaggageAllowance() const {
    return 20; // 20 kg
}

double EconomyPassenger::getLoyaltyMultiplier() const {
    return 1.0;
}

double EconomyPassenger::calculateRefund(double farePaid, int daysBeforeDeparture) const {
    if (daysBeforeDeparture > 7) {
        return farePaid * 0.80; // 80% refund
    } else if (daysBeforeDeparture >= 2) {
        return farePaid * 0.50; // 50% refund
    } else {
        return 0.0; // No refund within 48h
    }
}

void EconomyPassenger::displayDetails() const {
    std::cout << "\n---------------- [ ECONOMY PASSENGER ] ----------------\n";
    std::cout << "Passenger ID     : " << passengerId << "\n";
    std::cout << "Full Name        : " << name << "\n";
    std::cout << "Passport Number  : " << passportNumber << "\n";
    std::cout << "Contact Email    : " << email << "\n";
    std::cout << "Contact Phone    : " << phone << "\n";
    std::cout << "Loyalty Points   : " << loyaltyPoints << "\n";
    std::cout << "Baggage Allowance: " << getBaggageAllowance() << " kg\n";
    std::cout << "Meal Preference  : " << (mealPreferenceVegan ? "Vegan Meal" : "Standard Meal") << "\n";
    std::cout << "Refund Policy    : >7 days: 80% | 2-7 days: 50% | <2 days: 0%\n";
    std::cout << "--------------------------------------------------------\n";
}

std::string EconomyPassenger::getPassengerClass() const {
    return "Economy";
}

std::string EconomyPassenger::serialize() const {
    // Format: ECONOMY|passengerId|name|passportNumber|email|phone|loyaltyPoints|mealPreferenceVegan
    std::ostringstream oss;
    oss << "ECONOMY|" << passengerId << "|" << name << "|" << passportNumber << "|"
        << email << "|" << phone << "|" << loyaltyPoints << "|" << (mealPreferenceVegan ? 1 : 0);
    return oss.str();
}

std::shared_ptr<Passenger> EconomyPassenger::clone() const {
    return std::make_shared<EconomyPassenger>(*this);
}
