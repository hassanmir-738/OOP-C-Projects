#ifndef CHARTER_FLIGHT_H
#define CHARTER_FLIGHT_H

#include "Flight.h"

/**
 * ============================================================================
 * OOP CONCEPT: INHERITANCE (HIERARCHICAL) & POLYMORPHISM
 * Derived Class: CharterFlight (inherits from Flight)
 * Specific features:
 *  - contractHolder (Name/Company who chartered the plane)
 *  - cateringIncluded (bool)
 *  - hourlyRate (e.g. $1200/hr)
 *  - flightHours (e.g. 3 hours)
 * ============================================================================
 */
class CharterFlight : public Flight {
private:
    std::string contractHolder;
    bool cateringIncluded;
    double hourlyRate;
    int flightHours;

public:
    CharterFlight();
    CharterFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                  const std::string& depTime, int seats, double rate,
                  const std::string& holder = "SkyLink VIP Club", bool catering = true,
                  double hrRate = 1200.0, int hours = 3);
    virtual ~CharterFlight() override = default;

    // Getters / Setters
    std::string getContractHolder() const;
    bool getCateringIncluded() const;
    double getHourlyRate() const;
    int getFlightHours() const;

    // Overridden pure virtual functions
    virtual double calculateBaseFare() const override;
    virtual void displayDetails() const override;
    virtual std::string getFlightType() const override;
    virtual std::string serialize() const override;
    virtual std::shared_ptr<Flight> clone() const override;
};

#endif // CHARTER_FLIGHT_H
