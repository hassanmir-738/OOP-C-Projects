#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <iostream>
#include <memory>

// Forward declaration for friend class demonstration
class AirlineReportHelper;

/**
 * ============================================================================
 * OOP CONCEPT: ABSTRACTION, ENCAPSULATION & POLYMORPHISM
 * Abstract Base Class: Flight
 * Demonstrates:
 *  - Encapsulation: private/protected members with getters and setters
 *  - Abstraction: pure virtual functions calculateBaseFare(), displayDetails()
 *  - Virtual destructor for safe polymorphic deletion
 *  - Operator Overloading: operator<<, operator==, operator<
 *  - Friend Function & Friend Class
 * ============================================================================
 */
class Flight {
protected:
    std::string flightNumber;
    std::string origin;
    std::string destination;
    std::string departureDateTime; // Format: "YYYY-MM-DD HH:MM"
    int totalSeats;
    int availableSeats;
    double baseRate;
    double totalRevenue;

public:
    // Constructors & Destructor
    Flight();
    Flight(const std::string& fNum, const std::string& orig, const std::string& dest,
           const std::string& depTime, int seats, double rate);
    virtual ~Flight(); // Virtual destructor for polymorphism

    // Getters (Encapsulation)
    std::string getFlightNumber() const;
    std::string getOrigin() const;
    std::string getDestination() const;
    std::string getDepartureDateTime() const;
    int getTotalSeats() const;
    int getAvailableSeats() const;
    int getBookedSeats() const;
    double getBaseRate() const;
    double getTotalRevenue() const;
    double getOccupancyRate() const;

    // Setters
    void setOrigin(const std::string& orig);
    void setDestination(const std::string& dest);
    void setDepartureDateTime(const std::string& depTime);
    void setBaseRate(double rate);

    // Business Logic Methods
    bool bookSeat();
    bool releaseSeat();
    void addRevenue(double amount);
    void refundRevenue(double amount);

    // Pure Virtual Functions (Abstraction & Runtime Polymorphism)
    virtual double calculateBaseFare() const = 0;
    virtual void displayDetails() const = 0;
    virtual std::string getFlightType() const = 0;
    virtual std::string serialize() const = 0;
    virtual std::shared_ptr<Flight> clone() const = 0;

    // Operator Overloading
    bool operator==(const Flight& other) const;
    bool operator<(const Flight& other) const; // Sort by revenue in descending order

    // Friend Function (Topic 4)
    friend std::ostream& operator<<(std::ostream& os, const Flight& flight);

    // Friend Class (Topic 4)
    friend class AirlineReportHelper;
};

#endif // FLIGHT_H
