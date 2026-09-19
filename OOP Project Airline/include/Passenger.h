#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <iostream>
#include <memory>

/**
 * ============================================================================
 * OOP CONCEPT: ABSTRACTION, INHERITANCE & POLYMORPHISM
 * Base Class: Passenger
 * Demonstrates:
 *  - Encapsulation with protected members and public getters/setters
 *  - Pure virtual functions for polymorphic passenger behavior
 *  - Operator Overloading: operator<<, operator==
 * ============================================================================
 */
class Passenger {
protected:
    std::string passengerId;
    std::string name;
    std::string passportNumber;
    std::string email;
    std::string phone;
    int loyaltyPoints;

public:
    Passenger();
    Passenger(const std::string& id, const std::string& pName, const std::string& passport,
              const std::string& pEmail, const std::string& pPhone, int points = 0);
    virtual ~Passenger();

    // Getters & Setters
    std::string getPassengerId() const;
    std::string getName() const;
    std::string getPassportNumber() const;
    std::string getEmail() const;
    std::string getPhone() const;
    int getLoyaltyPoints() const;

    void setName(const std::string& pName);
    void setEmail(const std::string& pEmail);
    void setPhone(const std::string& pPhone);

    // Loyalty Points Management
    void addLoyaltyPoints(int points);
    bool redeemLoyaltyPoints(int points);

    // Pure Virtual Functions (Runtime Polymorphism)
    virtual double calculateFare(double flightBaseFare) const = 0;
    virtual int getBaggageAllowance() const = 0; // in kilograms
    virtual double getLoyaltyMultiplier() const = 0;
    virtual double calculateRefund(double farePaid, int daysBeforeDeparture) const = 0;
    virtual void displayDetails() const = 0;
    virtual std::string getPassengerClass() const = 0;
    virtual std::string serialize() const = 0;
    virtual std::shared_ptr<Passenger> clone() const = 0;

    // Operator Overloading
    bool operator==(const Passenger& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Passenger& passenger);
};

#endif // PASSENGER_H
