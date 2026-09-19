#ifndef BUSINESS_PASSENGER_H
#define BUSINESS_PASSENGER_H

#include "Passenger.h"

/**
 * ============================================================================
 * OOP CONCEPT: INHERITANCE & POLYMORPHISM
 * Derived Class: BusinessPassenger
 * Rules:
 *  - Fare Multiplier: 1.6x
 *  - Baggage Allowance: 35 kg
 *  - Loyalty Multiplier: 1.5x
 *  - Priority Lounge Access: Yes
 *  - Refund Rules:
 *      > 7 days before departure : 95% refund (5% penalty)
 *      2 - 7 days                : 80% refund (20% penalty)
 *      < 2 days                  : 40% refund (60% penalty)
 * ============================================================================
 */
class BusinessPassenger : public Passenger {
private:
    bool loungeAccess;
    std::string companyName;

public:
    BusinessPassenger();
    BusinessPassenger(const std::string& id, const std::string& pName, const std::string& passport,
                      const std::string& pEmail, const std::string& pPhone, int points = 0,
                      bool lounge = true, const std::string& company = "SkyLink Corporate");
    virtual ~BusinessPassenger() override = default;

    bool getLoungeAccess() const;
    std::string getCompanyName() const;
    void setLoungeAccess(bool lounge);
    void setCompanyName(const std::string& company);

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

#endif // BUSINESS_PASSENGER_H
