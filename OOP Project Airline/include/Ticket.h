#ifndef TICKET_H
#define TICKET_H

#include <string>
#include <iostream>
#include <memory>

/**
 * ============================================================================
 * OOP CONCEPT: COMPOSITION, AGGREGATION & OPERATOR OVERLOADING
 * Ticket Class:
 *  - Links Passenger and Flight
 *  - Overloads operator<< for stream output
 *  - Overloads operator== for comparing tickets by Ticket ID
 * ============================================================================
 */
class Ticket {
private:
    std::string ticketId;
    std::string passengerId;
    std::string flightNumber;
    std::string seatNumber;
    double farePaid;
    std::string bookingStatus; // "CONFIRMED" or "CANCELLED"
    std::string bookingDate;   // "YYYY-MM-DD HH:MM"
    double refundAmount;

public:
    Ticket();
    Ticket(const std::string& tId, const std::string& pId, const std::string& fNum,
           const std::string& seat, double fare, const std::string& date,
           const std::string& status = "CONFIRMED", double refund = 0.0);
    ~Ticket() = default;

    // Getters
    std::string getTicketId() const;
    std::string getPassengerId() const;
    std::string getFlightNumber() const;
    std::string getSeatNumber() const;
    double getFarePaid() const;
    std::string getBookingStatus() const;
    std::string getBookingDate() const;
    double getRefundAmount() const;
    bool isActive() const;

    // Status modifier
    void cancelTicket(double refund);

    // Serialization for File I/O
    std::string serialize() const;
    void displayTicket() const;

    // Operator Overloading
    bool operator==(const Ticket& other) const; // Compare tickets by Ticket ID
    friend std::ostream& operator<<(std::ostream& os, const Ticket& ticket);
};

#endif // TICKET_H
