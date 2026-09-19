#ifndef AIRLINE_H
#define AIRLINE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Flight.h"
#include "DomesticFlight.h"
#include "InternationalFlight.h"
#include "CharterFlight.h"
#include "Passenger.h"
#include "EconomyPassenger.h"
#include "BusinessPassenger.h"
#include "FirstClassPassenger.h"
#include "Ticket.h"
#include "TemplateUtils.h"

/**
 * ============================================================================
 * OOP CONCEPT: FRIEND CLASS (TOPIC 4)
 * AirlineReportHelper has friend access to Flight to perform internal audits.
 * ============================================================================
 */
class AirlineReportHelper {
public:
    static void generateFlightAudit(const Flight& flight);
};

/**
 * ============================================================================
 * OOP CONCEPT: AGGREGATION, ENCAPSULATION & STL CONTAINERS
 * Airline Class:
 *  - Aggregates Flights, Passengers, and Tickets using STL containers:
 *      std::vector<std::shared_ptr<Flight>>
 *      std::vector<std::shared_ptr<Passenger>>
 *      std::vector<std::shared_ptr<Ticket>>
 *      std::map<std::string, std::vector<std::string>> (Passenger ID -> Ticket IDs map)
 *  - Exposes all core business operations with Exception Handling
 * ============================================================================
 */
class Airline {
private:
    std::string airlineName;
    Repository<Flight> flightRepo;
    Repository<Passenger> passengerRepo;
    Repository<Ticket> ticketRepo;

    // STL Map for indexing passenger booking history: PassengerID -> Vector of Ticket IDs
    std::map<std::string, std::vector<std::string>> passengerBookingIndex;

    int nextTicketCounter;

    std::string generateNextTicketId();
    void updateBookingIndex();

public:
    explicit Airline(const std::string& name = "SkyLink Airways");
    ~Airline() = default;

    std::string getAirlineName() const;

    // ------------------------------------------------------------------------
    // 1. FLIGHT MANAGEMENT
    // ------------------------------------------------------------------------
    void addFlight(std::shared_ptr<Flight> flight);
    bool removeFlight(const std::string& flightNumber);
    std::shared_ptr<Flight> getFlight(const std::string& flightNumber) const;
    void listAllFlights() const;

    // Function Overloading for Flight Search (Topic 3)
    std::vector<std::shared_ptr<Flight>> searchFlights(const std::string& flightNumber) const;
    std::vector<std::shared_ptr<Flight>> searchFlights(const std::string& origin, const std::string& destination) const;
    std::vector<std::shared_ptr<Flight>> searchFlightsByDate(const std::string& datePrefix) const;

    // ------------------------------------------------------------------------
    // 2. PASSENGER MANAGEMENT
    // ------------------------------------------------------------------------
    void registerPassenger(std::shared_ptr<Passenger> passenger);
    bool removePassenger(const std::string& passengerId);
    std::shared_ptr<Passenger> getPassenger(const std::string& passengerId) const;
    void listAllPassengers() const;
    void viewPassengerHistory(const std::string& passengerId) const;

    // ------------------------------------------------------------------------
    // 3. BOOKING OPERATIONS
    // ------------------------------------------------------------------------
    std::shared_ptr<Ticket> bookFlight(const std::string& passengerId,
                                       const std::string& flightNumber,
                                       const std::string& seatNumber);

    // ------------------------------------------------------------------------
    // 4. CANCELLATION & REFUND OPERATIONS (Polymorphic)
    // ------------------------------------------------------------------------
    double cancelBooking(const std::string& ticketId);
    std::shared_ptr<Ticket> getTicket(const std::string& ticketId) const;
    void listAllTickets() const;

    // ------------------------------------------------------------------------
    // 5. REPORTS
    // ------------------------------------------------------------------------
    void reportTodaysDepartures(const std::string& targetDate = "") const;
    void reportOccupancyRates() const;
    void reportTopRevenueFlights(size_t topN = 5) const;
    void reportFinancialSummary() const;

    // ------------------------------------------------------------------------
    // 6. PERSISTENCE (FILE I/O)
    // ------------------------------------------------------------------------
    bool saveToFile(const std::string& flightFile,
                    const std::string& passengerFile,
                    const std::string& ticketFile) const;

    bool loadFromFile(const std::string& flightFile,
                      const std::string& passengerFile,
                      const std::string& ticketFile);

    void populateSampleData();
};

#endif // AIRLINE_H
