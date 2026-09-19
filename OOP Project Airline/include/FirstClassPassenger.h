#ifndef FIRST_CLASS_PASSENGER_H
#define FIRST_CLASS_PASSENGER_H

#include "Passenger.h"

/**
 * ============================================================================
 * OOP CONCEPT: INHERITANCE & POLYMORPHISM
 * Derived Class: FirstClassPassenger
 * Rules:
 *  - Fare Multiplier: 2.4x
 *  - Baggage Allowance: 50 kg
 *  - Loyalty Multiplier: 2.0x
 *  - Chauffeur Service: Yes
 *  - Refund Rules:
 *      > 7 days before departure : 100% refund (Full refund)
 *      2 - 7 days                : 90% refund (10% penalty)
 *      < 2 days                  : 75% refund (25% penalty)
 * ============================================================================
 */
class FirstClassPassenger : public Passenger {
private:
    bool chauffeurService;
    std::string suitePreference;

public:
    FirstClassPassenger();
    FirstClassPassenger(const std::string& id, const std::string& pName, const std::string& passport,
                        const std::string& pEmail, const std::string& pPhone, int points = 0,
                        bool chauffeur = true, const std::string& suite = "Royal Suite");
    virtual ~FirstClassPassenger() override = default;

    bool getChauffeurService() const;
    std::string getSuitePreference() const;
    void setChauffeurService(bool chauffeur);
    void setSuitePreference(const std::string& suite);

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

#endif // FIRST_CLASS_PASSENGER_H
