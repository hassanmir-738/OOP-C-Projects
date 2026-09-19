#include "../include/Ticket.h"
#include <iomanip>
#include <sstream>

Ticket::Ticket()
    : ticketId(""), passengerId(""), flightNumber(""), seatNumber(""),
      farePaid(0.0), bookingStatus("CONFIRMED"), bookingDate(""), refundAmount(0.0) {}

Ticket::Ticket(const std::string& tId, const std::string& pId, const std::string& fNum,
               const std::string& seat, double fare, const std::string& date,
               const std::string& status, double refund)
    : ticketId(tId), passengerId(pId), flightNumber(fNum), seatNumber(seat),
      farePaid(fare), bookingStatus(status), bookingDate(date), refundAmount(refund) {}

std::string Ticket::getTicketId() const { return ticketId; }
std::string Ticket::getPassengerId() const { return passengerId; }
std::string Ticket::getFlightNumber() const { return flightNumber; }
std::string Ticket::getSeatNumber() const { return seatNumber; }
double Ticket::getFarePaid() const { return farePaid; }
std::string Ticket::getBookingStatus() const { return bookingStatus; }
std::string Ticket::getBookingDate() const { return bookingDate; }
double Ticket::getRefundAmount() const { return refundAmount; }
bool Ticket::isActive() const { return bookingStatus == "CONFIRMED"; }

void Ticket::cancelTicket(double refund) {
    bookingStatus = "CANCELLED";
    refundAmount = refund;
}

std::string Ticket::serialize() const {
    // Format: TICKET|ticketId|passengerId|flightNumber|seatNumber|farePaid|bookingStatus|bookingDate|refundAmount
    std::ostringstream oss;
    oss << "TICKET|" << ticketId << "|" << passengerId << "|" << flightNumber << "|"
        << seatNumber << "|" << farePaid << "|" << bookingStatus << "|"
        << bookingDate << "|" << refundAmount;
    return oss.str();
}

void Ticket::displayTicket() const {
    std::cout << "\n=========================================================\n";
    std::cout << "                 SKYLINK AIRWAYS BOARDING PASS           \n";
    std::cout << "=========================================================\n";
    std::cout << " Ticket ID       : " << ticketId << "\n";
    std::cout << " Passenger ID    : " << passengerId << "\n";
    std::cout << " Flight Number   : " << flightNumber << "\n";
    std::cout << " Seat Assigned   : " << seatNumber << "\n";
    std::cout << " Fare Paid       : $" << std::fixed << std::setprecision(2) << farePaid << "\n";
    std::cout << " Booking Date    : " << bookingDate << "\n";
    std::cout << " Status          : [" << bookingStatus << "]\n";
    if (bookingStatus == "CANCELLED") {
        std::cout << " Refund Amount   : $" << refundAmount << "\n";
    }
    std::cout << "=========================================================\n";
}

// Operator Overloading: == compares tickets by Ticket ID
bool Ticket::operator==(const Ticket& other) const {
    return this->ticketId == other.ticketId;
}

// Friend Function: Stream insertion operator<<
std::ostream& operator<<(std::ostream& os, const Ticket& ticket) {
    os << "Ticket #" << ticket.ticketId << " | Flight: " << ticket.flightNumber
       << " | Passenger: " << ticket.passengerId
       << " | Seat: " << ticket.seatNumber
       << " | Status: " << ticket.bookingStatus
       << " | Fare: $" << std::fixed << std::setprecision(2) << ticket.farePaid;
    if (ticket.bookingStatus == "CANCELLED") {
        os << " (Refund: $" << ticket.refundAmount << ")";
    }
    return os;
}
