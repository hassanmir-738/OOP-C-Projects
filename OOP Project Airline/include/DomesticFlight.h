#ifndef DOMESTIC_FLIGHT_H
#define DOMESTIC_FLIGHT_H

#include "Flight.h"

/**
 * ============================================================================
 * OOP CONCEPT: INHERITANCE (HIERARCHICAL) & POLYMORPHISM
 * Derived Class: DomesticFlight (inherits from Flight)
 * Specific features:
 *  - stateTaxRate (e.g. 5%)
 *  - isRegionalSubsidized (reduces base fare if true)
 *  - airportLuggageFee
 * ============================================================================
 */
class DomesticFlight : public Flight {
private:
    double stateTaxRate;
    bool isRegionalSubsidized;
    double airportLuggageFee;

public:
    DomesticFlight();
    DomesticFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                   const std::string& depTime, int seats, double rate,
                   double taxRate = 0.05, bool subsidized = false, double luggageFee = 15.0);
    virtual ~DomesticFlight() override = default;

    // Getters / Setters
    double getStateTaxRate() const;
    bool getIsRegionalSubsidized() const;
    double getAirportLuggageFee() const;

    // Overridden pure virtual functions (Runtime Polymorphism)
    virtual double calculateBaseFare() const override;
    virtual void displayDetails() const override;
    virtual std::string getFlightType() const override;
    virtual std::string serialize() const override;
    virtual std::shared_ptr<Flight> clone() const override;
};

#endif // DOMESTIC_FLIGHT_H
