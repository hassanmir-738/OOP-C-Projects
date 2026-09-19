#ifndef ECONOMY_PASSENGER_H
#define ECONOMY_PASSENGER_H

#include "Passenger.h"

/**
 * ============================================================================
 * OOP CONCEPT: INHERITANCE & POLYMORPHISM
 * Derived Class: EconomyPassenger
 * Rules:
 *  - Fare Multiplier: 1.0x
 *  - Baggage Allowance: 20 kg
 *  - Loyalty Multiplier: 1.0x
 *  - Refund Rules:
 *      > 7 days before departure : 80% refund (20% penalty)
 *      2 - 7 days                : 50% refund (50% penalty)
 *      < 2 days                  : 0% refund (100% penalty)
 * ============================================================================
 */
class EconomyPassenger : public Passenger {
private:
    bool mealPreferenceVegan;

public:
    EconomyPassenger();
    EconomyPassenger(const std::string& id, const std::string& pName, const std::string& passport,
                     const std::string& pEmail, const std::string& pPhone, int points = 0,
                     bool veganMeal = false);
    virtual ~EconomyPassenger() override = default;

    bool getMealPreferenceVegan() const;
    void setMealPreferenceVegan(bool vegan);

    // Overridden methods
    virtual double calculateFare(double flightBaseFare) const override;
    virtual int getBaggageAllowance() const override;
    virtual double getLoyaltyMultiplier() const override;
    virtual double calculateRefund(double farePaid, int daysBeforeDeparture) const override;
    virtual void displayDetails() const override;
    virtual std::string getPassengerClass() const override;
    virtual std::string serialize() const override;
    virtual std::shared_ptr<Passenger> clone() const override;
};

#endif // ECONOMY_PASSENGER_H
