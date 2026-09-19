#ifndef INTERNATIONAL_FLIGHT_H
#define INTERNATIONAL_FLIGHT_H

#include "Flight.h"

/**
 * ============================================================================
 * OOP CONCEPT: INHERITANCE (HIERARCHICAL) & POLYMORPHISM
 * Derived Class: InternationalFlight (inherits from Flight)
 * Specific features:
 *  - visaRequired (bool)
 *  - customsDuty (e.g. $35.00)
 *  - passportValidityMonths (e.g. minimum 6 months)
 * ============================================================================
 */
class InternationalFlight : public Flight {
private:
    bool visaRequired;
    double customsDuty;
    int passportValidityMonths;

public:
    InternationalFlight();
    InternationalFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                         const std::string& depTime, int seats, double rate,
                         bool visaReq = true, double customs = 35.0, int validityMonths = 6);
    virtual ~InternationalFlight() override = default;

    // Getters / Setters
    bool getVisaRequired() const;
    double getCustomsDuty() const;
    int getPassportValidityMonths() const;

    // Overridden pure virtual functions
    virtual double calculateBaseFare() const override;
    virtual void displayDetails() const override;
    virtual std::string getFlightType() const override;
    virtual std::string serialize() const override;
    virtual std::shared_ptr<Flight> clone() const override;
};

#endif // INTERNATIONAL_FLIGHT_H
