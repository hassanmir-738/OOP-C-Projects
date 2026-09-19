# SkyLink Airways Management System — Design & Technical Report
**Course:** Object-Oriented Programming (C++)  
**Assignment:** Problem-Based Learning (PBL) Individual Project  
**Author:** Student / Software Engineer  

---

## 1. Executive Summary & Problem Scenario
SkyLink Airways previously operated its flight schedules, seat allocations, and passenger ticketing through ad-hoc spreadsheets. As operations expanded to domestic, international, and chartered routes, manual coordination caused frequent overbookings, misplaced passenger records, and financial discrepancies in cancellation refunds.

This project delivers a high-reliability, memory-safe, console-based C++ application utilizing standard Object-Oriented Programming principles. The system enforces domain rules at compile-time and runtime, separates business logic from data persistence, and provides comprehensive reporting for airline executives.

---

## 2. Design Architecture & OOP Justifications

```
+-------------------------------------------------------------------------------+
|                             SYSTEM ARCHITECTURE                               |
+-------------------------------------------------------------------------------+
|  Presentation Layer: Menu-Driven Console (Input validation & Error Catching)  |
|  Service Layer     : Airline Controller (Aggregator & Business Logic)         |
|  Domain Layer      : Flight Hierarchy, Passenger Hierarchy, Ticket Model     |
|  Utility & Core    : Repository<T> Templates, Custom Exceptions, File Engine  |
+-------------------------------------------------------------------------------+
```

### 2.1 Topic 1: Classes, Encapsulation, Abstraction, Aggregation & Composition
* **Encapsulation**: All class state variables (such as available seat counts, accumulated revenue, loyalty points, contact information) are marked `protected` or `private`. Modification is only possible through validated setters and business operations (`bookSeat()`, `releaseSeat()`, `addLoyaltyPoints()`), preserving object invariants.
* **Abstraction**: The base classes `Flight` and `Passenger` define abstract interfaces with pure virtual functions (`calculateBaseFare()`, `calculateFare()`, `calculateRefund()`, `displayDetails()`). The outer system interacts with these abstract concepts without coupling itself to concrete flight or passenger details.
* **Composition & Aggregation**: The `Airline` class acts as an aggregate root containing generic `Repository<T>` containers for `Flight`, `Passenger`, and `Ticket` objects. The `Ticket` class models an association linking a `Passenger` entity and a `Flight` entity.

### 2.2 Topic 2: Inheritance & Class Hierarchies
* **Hierarchical Inheritance in Flights**:
  * `Flight` (Abstract Base Class)
    * `DomesticFlight`: Incorporates state tax rates, regional route subsidies, and luggage surcharges.
    * `InternationalFlight`: Incorporates long-haul multiplier (1.25x), customs clearance duty, visa surcharges, and passport validity requirements.
    * `CharterFlight`: Incorporates private charter hourly rates, flight hours, contract holders, and VIP catering.
* **Hierarchical Inheritance in Passengers**:
  * `Passenger` (Base Class)
    * `EconomyPassenger`: Standard fare (1.0x), 20kg baggage, vegan meal preference.
    * `BusinessPassenger`: 1.6x fare, 35kg baggage, 1.5x loyalty points, lounge access.
    * `FirstClassPassenger`: 2.4x fare, 50kg luxury baggage, 2.0x loyalty points, private chauffeur, luxury suites.

### 2.3 Topic 3: Overloading & Overriding
* **Function Overloading**: Overloaded search methods in `Airline`:
  * `searchFlights(const std::string& flightNumber)` — exact/partial ID lookup.
  * `searchFlights(const std::string& origin, const std::string& destination)` — route lookup.
* **Function Overriding**: Virtual methods across the hierarchies are tagged with `override`, ensuring correct compile-time signature validation:
  * `Flight::calculateBaseFare()` overridden by `DomesticFlight`, `InternationalFlight`, and `CharterFlight`.
  * `Passenger::calculateRefund(fare, days)` overridden by `EconomyPassenger`, `BusinessPassenger`, and `FirstClassPassenger`.
* **Operator Overloading**:
  * `operator<<`: Overloaded for `Flight`, `Passenger`, and `Ticket` to streamline formatting and console/file output.
  * `operator==`: Overloaded on `Ticket` and `Flight` for entity comparison by identifier.
  * `operator<`: Overloaded for revenue-based flight sorting.

### 2.4 Topic 4: Polymorphism, Friend Functions & Friend Classes
* **Runtime Polymorphism**: Dynamically dispatched at runtime via `std::shared_ptr<Flight>` and `std::shared_ptr<Passenger>`. Booking calculates total ticket cost polymorphically without branching on type enums:
  $$\text{Final Fare} = \text{passenger}\rightarrow\text{calculateFare}(\text{flight}\rightarrow\text{calculateBaseFare}())$$
* **Friend Functions**: `operator<<` functions are declared as friends of `Flight`, `Passenger`, and `Ticket` to read private members during formatted streaming.
* **Friend Classes**: `AirlineReportHelper` is declared as a `friend class Flight`, allowing privileged inspection of private internal revenue data for executive audit reports.

### 2.5 Topic 5: File Handling & Persistence
* Flat-file text persistence stores flights (`data/flights.txt`), passengers (`data/passengers.txt`), and bookings (`data/tickets.txt`).
* Text-based delimiter parsing (`|`) ensures human readability, portability across platforms, and fault tolerance against missing or corrupted records.

### 2.6 Topic 6: Templates (Generic Programming)
* **Function Templates**:
  * `genericSearch<T, Predicate>(collection, pred)`: Reusable predicate filter.
  * `printCollection<T>(items, heading)`: Streamlined console renderer.
  * `findById<T, IdExtractor>(collection, id, extractor)`: Generic key locator.
* **Class Template `Repository<T>`**: Encapsulates STL `std::vector<std::shared_ptr<T>>` with type-safe operations (`add`, `remove`, `find`, `filter`, `sort`, `count`).

### 2.7 Topic 7: Exception Handling
* Hierarchical custom exceptions derived from `std::exception`:
  * `AirlineException` (base)
    * `FlightFullException` (thrown when booking a zero-seat flight)
    * `InvalidCancellationException` (thrown on duplicate cancellation or past departure)
    * `DuplicateBookingException` (thrown when a passenger double-books)
    * `FlightNotFoundException` / `PassengerNotFoundException`
    * `InvalidInputException` / `FileIOException`
* Comprehensive `try-catch` blocks throughout `main.cpp` guarantee the application recovers gracefully without sudden termination.

---

## 3. Comprehensive Test Cases & Execution Outcomes

| Test ID | Test Scenario | Input / Action | Expected Behavior | Actual Outcome | Status |
| :---: | :--- | :--- | :--- | :--- | :---: |
| **TC-01** | System Initialization & File Load | Launch application | Load existing flights, passengers, and tickets from `data/` directory | All 11 flights, 8 passengers, and 6 tickets loaded cleanly | **PASS** |
| **TC-02** | Polymorphic Fare: Domestic Economy | Book `P-101` on `SK-101` | Base: $(180 \times 1.05) + 15 = \$204.00$, Econ Multiplier: $1.0\times$ | Total fare calculated = $\$204.00$, seat allocated | **PASS** |
| **TC-03** | Polymorphic Fare: Intl Business | Book `P-201` on `SK-201` | Base: $(650 \times 1.25) + 45 + 45 = \$902.50$, Bus Multiplier: $1.6\times$ | Total fare calculated = $\$1444.00$, 72 loyalty pts added | **PASS** |
| **TC-04** | Polymorphic Fare: Intl First Class | Book `P-301` on `SK-201` | Base: $\$902.50$, First Class Multiplier: $2.4\times$ | Total fare calculated = $\$2165.00$, 216 loyalty pts added | **PASS** |
| **TC-05** | Duplicate Booking Rejection | Re-book `P-101` on `SK-101` | Throw `DuplicateBookingException` | Exception caught: *"Passenger already holds active ticket"* | **PASS** |
| **TC-06** | Flight Capacity / Full Flight Error | Attempt booking on 0-seat flight | Throw `FlightFullException` | Exception caught: *"Flight is fully booked"* | **PASS** |
| **TC-07** | Polymorphic Cancellation Refund (>7 Days) | Cancel Business ticket `TKT-1002` | Apply 95% refund rule for Business tier | Refund issued: $\$1371.80$, seat released, revenue adjusted | **PASS** |
| **TC-08** | Invalid Ticket Cancellation | Cancel already cancelled ticket | Throw `InvalidCancellationException` | Exception caught: *"Ticket has already been cancelled"* | **PASS** |
| **TC-09** | Function Overloaded Search | Search by "New York" & "London" | Return flights matching both endpoints | Returned `SK-201` with real-time seat availability | **PASS** |
| **TC-10** | Executive Top Revenue Report | Run Report 3 | Sort flights by accumulated revenue using `std::sort` | Correctly ranked `SK-201` ($3609.00) and `SK-202` ($2778.00) top | **PASS** |
| **TC-11** | Friend Class Audit | Audit `SK-101` | Access private revenue and seat fields via `AirlineReportHelper` | Displayed exact internal revenue and capacity metrics | **PASS** |
| **TC-12** | State Persistence on Exit | Select Option 7 (Save & Exit) | Write serialized strings to `data/*.txt` | All records saved; subsequent startup restored exact state | **PASS** |

---

## 4. Known Limitations & Future Enhancements
1. **Interactive Graphical Seat Map**: Currently seats are assigned via seat numbers (e.g. `14A`); adding a 2D matrix seat grid UI would enhance visualization.
2. **Database Integration**: While the text-based flat file engine is fast and portable, migrating to SQLite or PostgreSQL would provide transactional ACID guarantees for concurrent multi-terminal booking.
3. **Multi-Leg / Connecting Flights**: The current model represents direct legs; a graph-based routing algorithm could be introduced to support connecting itineraries.

---

## 5. Viva Voce / Presentation Preparation Guide

### Q1: Why did you use an abstract base class for `Flight`?
> **Answer:** `Flight` contains common domain attributes (flight number, origin, destination, seats) but calculating the base fare varies fundamentally depending on whether the flight is Domestic, International, or Charter. By making `calculateBaseFare()` pure virtual (`= 0`), we enforce that derived classes supply their own pricing logic while allowing the `Airline` controller to handle all flights through polymorphic `Flight` pointers.

### Q2: How is runtime polymorphism demonstrated in your ticket booking and cancellation?
> **Answer:** During booking, the system calls `flight->calculateBaseFare()` and `passenger->calculateFare(baseFare)`. Neither method needs `if-else` or `switch` statements checking flight or passenger types; C++ virtual method tables (`vtable`) dynamically dispatch the call to the appropriate subclass. Similarly, during cancellation, `passenger->calculateRefund()` applies tier-specific penalty percentages based on the notice period.

### Q3: Where did you use Operator Overloading and why?
> **Answer:** We overloaded `operator<<` as a friend function to output formatted details for `Flight`, `Passenger`, and `Ticket` directly to `std::cout` or file streams. We overloaded `operator==` on `Ticket` and `Flight` to compare objects by their unique IDs, and `operator<` to enable sorting flights by revenue using standard STL algorithms.

### Q4: Explain your use of Templates in this project.
> **Answer:** We implemented both function templates (`genericSearch`, `findById`, `printCollection`) and a class template (`Repository<T>`). `Repository<T>` provides type-safe container management (adding, filtering, sorting, removing items) for any class (`Flight`, `Passenger`, `Ticket`) without duplicating container boilerplate code.

### Q5: How do custom exceptions improve your program's stability?
> **Answer:** Rather than returning error codes (like `-1` or `false`) that calling code might ignore, exceptional conditions (like `FlightFullException` or `DuplicateBookingException`) immediately transfer control to structured `try-catch` blocks. This ensures the system remains robust and prevents invalid data from corrupting system state.
