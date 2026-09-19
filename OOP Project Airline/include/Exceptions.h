#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <string>
#include <exception>

/**
 * ============================================================================
 * OOP CONCEPT: EXCEPTION HANDLING & INHERITANCE
 * Custom Exception Hierarchy for SkyLink Airline System.
 * All custom exceptions inherit from std::exception or AirlineException.
 * ============================================================================
 */

// Base Exception for all Airline specific errors
class AirlineException : public std::exception {
protected:
    std::string message;
public:
    explicit AirlineException(const std::string& msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
    virtual ~AirlineException() noexcept = default;
};

// Thrown when trying to book a seat on a full flight
class FlightFullException : public AirlineException {
public:
    explicit FlightFullException(const std::string& flightNum)
        : AirlineException("Error: Flight [" + flightNum + "] is fully booked! No seats available.") {}
};

// Thrown when an invalid cancellation is attempted (e.g. already cancelled or past departure)
class InvalidCancellationException : public AirlineException {
public:
    explicit InvalidCancellationException(const std::string& reason)
        : AirlineException("Cancellation Error: " + reason) {}
};

// Thrown when a passenger attempts to double-book the same flight
class DuplicateBookingException : public AirlineException {
public:
    DuplicateBookingException(const std::string& passengerId, const std::string& flightNum)
        : AirlineException("Booking Error: Passenger [" + passengerId + "] already holds an active ticket for Flight [" + flightNum + "].") {}
};

// Thrown when a flight search or lookup fails
class FlightNotFoundException : public AirlineException {
public:
    explicit FlightNotFoundException(const std::string& flightNum)
        : AirlineException("Search Error: Flight [" + flightNum + "] not found in the airline system.") {}
};

// Thrown when a passenger lookup fails
class PassengerNotFoundException : public AirlineException {
public:
    explicit PassengerNotFoundException(const std::string& passengerId)
        : AirlineException("Search Error: Passenger with ID [" + passengerId + "] does not exist.") {}
};

// Thrown when invalid user input is detected
class InvalidInputException : public AirlineException {
public:
    explicit InvalidInputException(const std::string& details)
        : AirlineException("Input Error: " + details) {}
};

// Thrown when file loading or saving fails
class FileIOException : public AirlineException {
public:
    explicit FileIOException(const std::string& filename, const std::string& details)
        : AirlineException("File Error on [" + filename + "]: " + details) {}
};

#endif // EXCEPTIONS_H
