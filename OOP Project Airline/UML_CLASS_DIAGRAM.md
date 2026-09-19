# UML Class Diagram — SkyLink Airways System

This document provides complete UML Class Diagrams for the Airline Reservation & Flight Management System, illustrating class hierarchies, attributes, operations, visibility specifiers, and object relationships.

---

## 1. Visual Mermaid Class Diagram

```mermaid
classDiagram
    %% Inheritance: Flight Hierarchy
    Flight <|-- DomesticFlight : Inheritance (is-a)
    Flight <|-- InternationalFlight : Inheritance (is-a)
    Flight <|-- CharterFlight : Inheritance (is-a)

    %% Inheritance: Passenger Hierarchy
    Passenger <|-- EconomyPassenger : Inheritance (is-a)
    Passenger <|-- BusinessPassenger : Inheritance (is-a)
    Passenger <|-- FirstClassPassenger : Inheritance (is-a)

    %% Exception Hierarchy
    std_exception <|-- AirlineException : Inheritance (is-a)
    AirlineException <|-- FlightFullException : Inheritance (is-a)
    AirlineException <|-- InvalidCancellationException : Inheritance (is-a)
    AirlineException <|-- DuplicateBookingException : Inheritance (is-a)
    AirlineException <|-- FlightNotFoundException : Inheritance (is-a)
    AirlineException <|-- PassengerNotFoundException : Inheritance (is-a)

    %% Aggregations & Associations
    Airline o-- Repository_Flight : Aggregates
    Airline o-- Repository_Passenger : Aggregates
    Airline o-- Repository_Ticket : Aggregates
    Repository_Flight o-- Flight : Contains
    Repository_Passenger o-- Passenger : Contains
    Repository_Ticket o-- Ticket : Contains

    Ticket ..> Flight : References flightNumber
    Ticket ..> Passenger : References passengerId

    %% Friend Class Relationship
    AirlineReportHelper ..> Flight : Friend Class Access

    %% Class Definitions
    class Flight {
        <<abstract>>
        #string flightNumber
        #string origin
        #string destination
        #string departureDateTime
        #int totalSeats
        #int availableSeats
        #double baseRate
        #double totalRevenue
        +Flight()
        +Flight(fNum, orig, dest, depTime, seats, rate)
        +virtual ~Flight()
        +getFlightNumber() string
        +getOrigin() string
        +getDestination() string
        +getDepartureDateTime() string
        +getTotalSeats() int
        +getAvailableSeats() int
        +getBookedSeats() int
        +getBaseRate() double
        +getTotalRevenue() double
        +getOccupancyRate() double
        +bookSeat() bool
        +releaseSeat() bool
        +addRevenue(amount) void
        +refundRevenue(amount) void
        +virtual calculateBaseFare()* double
        +virtual displayDetails()* void
        +virtual getFlightType()* string
        +virtual serialize()* string
        +virtual clone()* shared_ptr~Flight~
        +operator==(other) bool
        +operator<(other) bool
    }

    class DomesticFlight {
        -double stateTaxRate
        -bool isRegionalSubsidized
        -double airportLuggageFee
        +DomesticFlight()
        +DomesticFlight(...)
        +getStateTaxRate() double
        +getIsRegionalSubsidized() bool
        +getAirportLuggageFee() double
        +calculateBaseFare() double
        +displayDetails() void
        +getFlightType() string
        +serialize() string
        +clone() shared_ptr~Flight~
    }

    class InternationalFlight {
        -bool visaRequired
        -double customsDuty
        -int passportValidityMonths
        +InternationalFlight()
        +InternationalFlight(...)
        +getVisaRequired() bool
        +getCustomsDuty() double
        +getPassportValidityMonths() int
        +calculateBaseFare() double
        +displayDetails() void
        +getFlightType() string
        +serialize() string
        +clone() shared_ptr~Flight~
    }

    class CharterFlight {
        -string contractHolder
        -bool cateringIncluded
        -double hourlyRate
        -int flightHours
        +CharterFlight()
        +CharterFlight(...)
        +getContractHolder() string
        +getCateringIncluded() bool
        +getHourlyRate() double
        +getFlightHours() int
        +calculateBaseFare() double
        +displayDetails() void
        +getFlightType() string
        +serialize() string
        +clone() shared_ptr~Flight~
    }

    class Passenger {
        <<abstract>>
        #string passengerId
        #string name
        #string passportNumber
        #string email
        #string phone
        #int loyaltyPoints
        +Passenger()
        +Passenger(id, name, passport, email, phone, points)
        +virtual ~Passenger()
        +getPassengerId() string
        +getName() string
        +getPassportNumber() string
        +getEmail() string
        +getPhone() string
        +getLoyaltyPoints() int
        +addLoyaltyPoints(pts) void
        +redeemLoyaltyPoints(pts) bool
        +virtual calculateFare(flightBaseFare)* double
        +virtual getBaggageAllowance()* int
        +virtual getLoyaltyMultiplier()* double
        +virtual calculateRefund(farePaid, daysBefore)* double
        +virtual displayDetails()* void
        +virtual getPassengerClass()* string
        +virtual serialize()* string
        +virtual clone()* shared_ptr~Passenger~
        +operator==(other) bool
    }

    class EconomyPassenger {
        -bool mealPreferenceVegan
        +EconomyPassenger()
        +EconomyPassenger(...)
        +getMealPreferenceVegan() bool
        +setMealPreferenceVegan(vegan) void
        +calculateFare(flightBaseFare) double
        +getBaggageAllowance() int
        +getLoyaltyMultiplier() double
        +calculateRefund(farePaid, daysBefore) double
        +displayDetails() void
        +getPassengerClass() string
        +serialize() string
    }

    class BusinessPassenger {
        -bool loungeAccess
        -string companyName
        +BusinessPassenger()
        +BusinessPassenger(...)
        +getLoungeAccess() bool
        +getCompanyName() string
        +calculateFare(flightBaseFare) double
        +getBaggageAllowance() int
        +getLoyaltyMultiplier() double
        +calculateRefund(farePaid, daysBefore) double
        +displayDetails() void
        +getPassengerClass() string
        +serialize() string
    }

    class FirstClassPassenger {
        -bool chauffeurService
        -string suitePreference
        +FirstClassPassenger()
        +FirstClassPassenger(...)
        +getChauffeurService() bool
        +getSuitePreference() string
        +calculateFare(flightBaseFare) double
        +getBaggageAllowance() int
        +getLoyaltyMultiplier() double
        +calculateRefund(farePaid, daysBefore) double
        +displayDetails() void
        +getPassengerClass() string
        +serialize() string
    }

    class Ticket {
        -string ticketId
        -string passengerId
        -string flightNumber
        -string seatNumber
        -double farePaid
        -string bookingStatus
        -string bookingDate
        -double refundAmount
        +Ticket()
        +Ticket(...)
        +getTicketId() string
        +getPassengerId() string
        +getFlightNumber() string
        +getSeatNumber() string
        +getFarePaid() double
        +getBookingStatus() string
        +getBookingDate() string
        +getRefundAmount() double
        +isActive() bool
        +cancelTicket(refund) void
        +serialize() string
        +displayTicket() void
        +operator==(other) bool
    }

    class Airline {
        -string airlineName
        -Repository~Flight~ flightRepo
        -Repository~Passenger~ passengerRepo
        -Repository~Ticket~ ticketRepo
        -map~string, vector~string~~ passengerBookingIndex
        -int nextTicketCounter
        +Airline(name)
        +addFlight(flight) void
        +removeFlight(fNum) bool
        +getFlight(fNum) shared_ptr~Flight~
        +listAllFlights() void
        +searchFlights(fNum) vector~shared_ptr~Flight~~
        +searchFlights(orig, dest) vector~shared_ptr~Flight~~
        +searchFlightsByDate(date) vector~shared_ptr~Flight~~
        +registerPassenger(psg) void
        +removePassenger(pId) bool
        +getPassenger(pId) shared_ptr~Passenger~
        +listAllPassengers() void
        +viewPassengerHistory(pId) void
        +bookFlight(pId, fNum, seat) shared_ptr~Ticket~
        +cancelBooking(tId) double
        +getTicket(tId) shared_ptr~Ticket~
        +listAllTickets() void
        +reportTodaysDepartures(date) void
        +reportOccupancyRates() void
        +reportTopRevenueFlights(topN) void
        +reportFinancialSummary() void
        +saveToFile(fFile, pFile, tFile) bool
        +loadFromFile(fFile, pFile, tFile) bool
    }

    class AirlineReportHelper {
        +static generateFlightAudit(flight) void
    }
```

---

## 2. Text / ASCII Hierarchy Overview

```
                      +-----------------------------+
                      |       Flight (Abstract)     |
                      +-----------------------------+
                                     ^
                                     | (Inheritance)
           +-------------------------+-------------------------+
           |                         |                         |
+---------------------+   +-----------------------+   +--------------------+
|   DomesticFlight    |   |  InternationalFlight  |   |   CharterFlight    |
+---------------------+   +-----------------------+   +--------------------+

                      +-----------------------------+
                      |     Passenger (Abstract)    |
                      +-----------------------------+
                                     ^
                                     | (Inheritance)
           +-------------------------+-------------------------+
           |                         |                         |
+---------------------+   +-----------------------+   +--------------------+
|  EconomyPassenger   |   |   BusinessPassenger   |   | FirstClassPassenger|
+---------------------+   +-----------------------+   +--------------------+

                      +-----------------------------+
                      |           Ticket            |
                      +-----------------------------+
                         |                       |
                         v (Refers to)           v (Refers to)
                      [Flight]              [Passenger]

                      +-----------------------------+
                      |           Airline           |
                      | (Aggregates & Coordinates)  |
                      +-----------------------------+
                         |           |           |
                         v           v           v
                    [Flights]  [Passengers]  [Tickets]
```

---

## 3. Relationships & OOP Mechanisms Table

| Relationship Type | Source Class | Target Class | Description & OOP Rationale |
| :--- | :--- | :--- | :--- |
| **Inheritance (`is-a`)** | `DomesticFlight`, `InternationalFlight`, `CharterFlight` | `Flight` | Hierarchical inheritance with polymorphic pricing and route rules. |
| **Inheritance (`is-a`)** | `EconomyPassenger`, `BusinessPassenger`, `FirstClassPassenger` | `Passenger` | Hierarchical inheritance with polymorphic refund and multiplier logic. |
| **Inheritance (`is-a`)** | `FlightFullException`, `DuplicateBookingException`, etc. | `AirlineException` | Custom exception hierarchy derived from `std::exception`. |
| **Aggregation (`has-a`)**| `Airline` | `Flight`, `Passenger`, `Ticket` | The airline manages independent collections stored in `Repository<T>`. |
| **Association (`uses-a`)**| `Ticket` | `Flight`, `Passenger` | A ticket records a completed booking between a passenger and a flight. |
| **Friendship** | `AirlineReportHelper` | `Flight` | Friend class allowed direct inspection of private revenue members. |
| **Templates** | `Repository<T>` | Generic Type `T` | Type-safe generic container replacing repetitive boilerplate. |
