#include "../include/Passenger.h"
#include <iomanip>

Passenger::Passenger()
    : passengerId(""), name(""), passportNumber(""), email(""), phone(""), loyaltyPoints(0) {}

Passenger::Passenger(const std::string& id, const std::string& pName, const std::string& passport,
                     const std::string& pEmail, const std::string& pPhone, int points)
    : passengerId(id), name(pName), passportNumber(passport),
      email(pEmail), phone(pPhone), loyaltyPoints(points) {}

Passenger::~Passenger() {
    // Virtual destructor
}

std::string Passenger::getPassengerId() const { return passengerId; }
std::string Passenger::getName() const { return name; }
std::string Passenger::getPassportNumber() const { return passportNumber; }
std::string Passenger::getEmail() const { return email; }
std::string Passenger::getPhone() const { return phone; }
int Passenger::getLoyaltyPoints() const { return loyaltyPoints; }

void Passenger::setName(const std::string& pName) { name = pName; }
void Passenger::setEmail(const std::string& pEmail) { email = pEmail; }
void Passenger::setPhone(const std::string& pPhone) { phone = pPhone; }

void Passenger::addLoyaltyPoints(int points) {
    if (points > 0) {
        loyaltyPoints += points;
    }
}

bool Passenger::redeemLoyaltyPoints(int points) {
    if (points > 0 && loyaltyPoints >= points) {
        loyaltyPoints -= points;
        return true;
    }
    return false;
}

bool Passenger::operator==(const Passenger& other) const {
    return this->passengerId == other.passengerId;
}

std::ostream& operator<<(std::ostream& os, const Passenger& passenger) {
    os << "[" << passenger.getPassengerClass() << "] "
       << passenger.passengerId << " | "
       << passenger.name << " | Passport: "
       << passenger.passportNumber << " | Points: "
       << passenger.loyaltyPoints << " | Baggage: "
       << passenger.getBaggageAllowance() << "kg";
    return os;
}
