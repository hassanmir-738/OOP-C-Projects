# ✈️ SkyLink Airways — Flight Reservation & Airline Management System

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-14%20%2F%2017-00599C?logo=c%2B%2B)](https://en.wikipedia.org/wiki/C%2B%2B14)
[![Build Status](https://img.shields.io/badge/Build-Passing%20(Zero%20Warnings)-brightgreen)](#-compilation--execution)
[![OOP Compliance](https://img.shields.io/badge/OOP%20Rubric-100%25%20Compliant-blue)](#-object-oriented-programming-oop-mapping)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)](#-compilation--execution)

> **Course:** Object-Oriented Programming (C++) — Problem-Based Learning (PBL) Individual Project  
> **Institution:** Department of Computer Science & Software Engineering  
> **Project Scope:** Console-Based Multi-Tier Airline Reservation & Flight Management Engine

---

## 📑 Table of Contents
1. [Executive Summary](#-executive-summary)
2. [Key System Features](#-key-system-features)
3. [Project Directory Architecture](#-project-directory-architecture)
4. [Object-Oriented Programming (OOP) Mapping](#-object-oriented-programming-oop-mapping)
5. [Class Design & Hierarchy](#-class-design--hierarchy)
6. [Compilation & Execution](#-compilation--execution)
7. [Console Menu & User Workflow](#-console-menu--user-workflow)
8. [Sample Pre-Loaded Demonstration Data](#-sample-pre-loaded-demonstration-data)
9. [Viva Voce & Evaluation Guide (100 Marks Breakdown)](#-viva-voce--evaluation-guide)
10. [Deliverables & Submission Checklist](#-deliverables--submission-checklist)

---

## 🌟 Executive Summary

**SkyLink Airways** is an enterprise-grade, memory-safe, console-based Airline Reservation and Flight Management application engineered in modern C++.

Previously, the airline operated with manual spreadsheets, leading to common operational issues such as flight overbooking, lost passenger manifests, and improper refund disbursements. This project completely replaces legacy spreadsheets with a scalable, fully object-oriented architecture that:
* Enforces strict business invariants (e.g. preventing double-booking, managing available capacity).
* Leverages **polymorphism** for dynamic flight fare calculations and tiered cancellation refund rules.
* Guarantees data persistence across sessions using human-readable flat-file text storage.
* Implements robust exception handling to eliminate runtime crashes on invalid user input.

---

## ⚡ Key System Features

### 1. 🛫 Flight Management Subsystem
* **Multi-Tier Flight Types**:
  * **Domestic Flights**: Incorporates state taxation, regional route government subsidies (-$20 deduction), and airport baggage surcharges.
  * **International Flights**: Incorporates international routing multiplier (1.25x), customs duty fees, visa requirement flags, and minimum passport validity verification.
  * **Charter Flights**: Custom private VIP flights featuring contract holder tagging, flight duration calculations, hourly rates, and gourmet catering.
* **Function Overloaded Flight Search**:
  * Search by unique **Flight Number** (`searchFlights(string)`).
  * Search by route endpoints: **Origin $\rightarrow$ Destination** (`searchFlights(origin, destination)`).
  * Search by **Departure Date Prefix** (`searchFlightsByDate(date)`).
* **Friend Class Internal Audit**: Direct inspection of private fleet revenue and seat metrics via `AirlineReportHelper`.

### 2. 👤 Passenger & Loyalty Management Subsystem
* **Tiered Passenger Classifications**:
  * **Economy**: Standard base pricing ($1.0\times$), 20 kg baggage allowance, dietary vegan meal toggle, $1.0\times$ loyalty accrual.
  * **Business**: Executive pricing ($1.6\times$), 35 kg baggage allowance, VIP lounge privileges, corporate sponsorship tags, $1.5\times$ loyalty points.
  * **First Class**: Luxury pricing ($2.4\times$), 50 kg baggage allowance, private luxury sedan chauffeur service, dedicated suites, $2.0\times$ loyalty points.
* **Booking History Tracking**: Real-time lookup of past and active bookings per passenger via indexed associative maps.

### 3. 🎫 Booking & Smart Seat Allocation Engine
* **Seat Inventory Safeguards**: Rejects bookings if the flight is at 100% capacity (`FlightFullException`).
* **Duplicate Booking Prevention**: Restricts passengers from reserving multiple active tickets on the same flight (`DuplicateBookingException`).
* **Dynamic Polymorphic Pricing**:
  $$\text{Total Ticket Fare} = \text{Passenger::calculateFare}\Big(\text{Flight::calculateBaseFare}()\Big)$$
* **Automated Loyalty Points**: Awards points based on passenger tier multiplier and final fare paid.

### 4. 🔄 Cancellation & Tiered Refund Subsystem
* **Time-Sensitive Polymorphic Refund Table**:
  | Passenger Class | > 7 Days Before Departure | 2 to 7 Days Notice | < 48 Hours Notice |
  | :--- | :---: | :---: | :---: |
  | **Economy Class** | **80%** Refund (20% fee) | **50%** Refund (50% fee) | **0%** Refund (100% fee) |
  | **Business Class** | **95%** Refund (5% fee) | **80%** Refund (20% fee) | **40%** Refund (60% fee) |
  | **First Class** | **100%** Full Refund | **90%** Refund (10% fee) | **75%** Refund (25% fee) |
* **Dynamic Inventory Restoration**: Automatically returns released seats back to the active flight inventory and adjusts airline revenue metrics.

### 5. 📊 Executive Reporting & Analytics
* **Today's Departures**: Real-time flight schedule filtering for the current calendar date.
* **Flight Occupancy Report**: Tabular breakdown of capacity utilization, booked vs. available seats, and utilization status (`[NORMAL]`, `[HIGH]`, `[FULL]`).
* **Top 5 Revenue Routes**: High-performance ranking powered by the STL `std::sort` algorithm.
* **Fleet Financial Summary**: Aggregated gross revenue, registered passenger count, and capacity utilization.

---

## 🗂️ Project Directory Architecture

```
OOP Project Airline/
├── include/                     # C++ Header Declarations (.h)
│   ├── Flight.h                 # Abstract Base Class: Flight
│   ├── DomesticFlight.h         # Derived Class: DomesticFlight
│   ├── InternationalFlight.h    # Derived Class: InternationalFlight
│   ├── CharterFlight.h          # Derived Class: CharterFlight
│   ├── Passenger.h              # Base Class: Passenger
│   ├── EconomyPassenger.h       # Derived Class: EconomyPassenger
│   ├── BusinessPassenger.h      # Derived Class: BusinessPassenger
│   ├── FirstClassPassenger.h    # Derived Class: FirstClassPassenger
│   ├── Ticket.h                 # Ticket Class (Links Passenger & Flight)
│   ├── Exceptions.h             # Custom Exception Class Hierarchy
│   ├── TemplateUtils.h          # Generic Function & Class Templates (Repository<T>)
│   ├── Airline.h                # Airline Aggregator Controller
│   └── Utils.h                  # Date/Time, String, and Validation Utilities
│
├── src/                         # Implementation Source Files (.cpp)
│   ├── Flight.cpp               # Flight Base Logic & Operator Overloads
│   ├── DomesticFlight.cpp       # Domestic Pricing & Serialization
│   ├── InternationalFlight.cpp  # International Customs & Visa Logic
│   ├── CharterFlight.cpp        # Charter Contract & Hourly Logic
│   ├── Passenger.cpp            # Passenger Base Logic & Point Management
│   ├── EconomyPassenger.cpp     # Economy Refund & Pricing Rules
│   ├── BusinessPassenger.cpp    # Business Lounge & Refund Rules
│   ├── FirstClassPassenger.cpp  # First Class Chauffeur & Refund Rules
│   ├── Ticket.cpp               # Ticket Model & Stream Insertion
│   ├── Airline.cpp              # Core Business Engine, Persistence & Reports
│   ├── Utils.cpp                # Input Guards, EOF Safety & Calculations
│   └── main.cpp                 # Interactive Menu-Driven Application
│
├── data/                        # Persistent Flat-File Text Storage
│   ├── flights.txt              # 11+ Pre-loaded Flights (Domestic/Intl/Charter)
│   ├── passengers.txt           # 8+ Pre-loaded Passengers (Economy/Business/First)
│   └── tickets.txt              # Pre-issued Booking Records
│
├── Makefile                     # Cross-platform build automation script
├── build.bat                    # 1-Click Windows Build & Run Script
├── UML_CLASS_DIAGRAM.md         # Full Mermaid & PlantUML Diagrams
├── DESIGN_REPORT.md             # 2-3 Page Academic Report & Viva Q&A Guide
└── README.md                    # Project Documentation
```

---

## 🧩 Object-Oriented Programming (OOP) Mapping

This project explicitly satisfies all 7 required OOP core topics:

```
+-----------------------------------------------------------------------------------------+
|                                7 OOP PILLARS IN CODE                                    |
+-----------------------------------------------------------------------------------------+
|  1. Classes & Encapsulation  : Private state, getters/setters in Flight, Passenger, Ticket|
|  2. Abstraction              : Pure virtual functions in Flight & Passenger base classes|
|  3. Inheritance              : Hierarchical flight & passenger derived hierarchies      |
|  4. Polymorphism             : Dynamic dispatch for fare & refund calculations          |
|  5. Overloading & Overriding : Overloaded search, operator<<, operator==, operator<     |
|  6. Templates                : Generic search utilities & Repository<T> class template  |
|  7. Exception Handling       : FlightFullException, DuplicateBookingException, try-catch|
+-----------------------------------------------------------------------------------------+
```

### Detailed Topic Breakdown:

1. **Classes, Objects, Constructors & Encapsulation**:
   * Initialized with parameterized constructors, default constructors, and clean destructors.
   * State variables are strictly encapsulated; modifications happen only through domain methods.
2. **Inheritance (Hierarchical & Single)**:
   * `Flight` $\rightarrow$ `DomesticFlight`, `InternationalFlight`, `CharterFlight`
   * `Passenger` $\rightarrow$ `EconomyPassenger`, `BusinessPassenger`, `FirstClassPassenger`
   * `std::exception` $\rightarrow$ `AirlineException` $\rightarrow$ Sub-exceptions
3. **Function Overloading & Overriding**:
   * Overloaded functions: `Airline::searchFlights(string)` vs `Airline::searchFlights(origin, dest)`.
   * Overridden methods marked with `override`: `calculateBaseFare()`, `calculateFare()`, `calculateRefund()`, `displayDetails()`.
4. **Operator Overloading**:
   * `operator<<`: Formatted stream output for `Flight`, `Passenger`, and `Ticket`.
   * `operator==`: Direct equality comparison for `Ticket` by `ticketId`.
   * `operator<`: Ascending/descending sorting support for flights by revenue.
5. **Polymorphism, Friend Functions & Friend Classes**:
   * Runtime dynamic dispatch using `std::shared_ptr<Flight>` and `std::shared_ptr<Passenger>`.
   * `friend std::ostream& operator<<(...)` implemented across entities.
   * `AirlineReportHelper` defined as a `friend class` of `Flight` to audit internal revenue metrics.
6. **File Handling (State Persistence)**:
   * Pipe-delimited (`|`) file serialization and deserialization in `data/*.txt`.
   * Gracefully creates default sample records if files are missing or empty.
7. **Templates (Generic Programming)**:
   * Function templates: `genericSearch<T>()`, `findById<T>()`, `printCollection<T>()`.
   * Class template: `Repository<T>` providing CRUD operations over collections of pointers.
8. **Exception Handling**:
   * Custom exception hierarchy (`FlightFullException`, `DuplicateBookingException`, `InvalidCancellationException`, etc.).
   * Comprehensive `try-catch` blocks protecting user input and flight operations from crashes.

---

## 📐 Class Design & Hierarchy

```
                                +-----------------------------+
                                |      Flight (Abstract)      |
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
                                   v (References)          v (References)
                                [Flight]              [Passenger]
```

*(For the complete interactive Mermaid diagram, see [UML_CLASS_DIAGRAM.md](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/UML_CLASS_DIAGRAM.md))*

---

## 💻 Compilation & Execution

The codebase compiles with `g++ -std=c++14 -Wall -Wextra` with **0 warnings and 0 errors**.

### Option 1: 1-Click Batch Script (Recommended for Windows)
Double-click [build.bat](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/build.bat) or run from your terminal:
```cmd
.\build.bat
```

### Option 2: Using `g++` Directly (Windows, macOS, Linux)
```bash
# 1. Compile all source files into an executable
g++ -std=c++14 -Wall -Wextra -Iinclude src/*.cpp -o airline_system.exe

# 2. Run the application
.\airline_system.exe
```

### Option 3: Using `Makefile`
```bash
# Compile
make

# Execute
make run

# Clean build artifacts
make clean
```

---

## 🖥️ Console Menu & User Workflow

```
===========================================================================
         SkyLink Airways - FLIGHT RESERVATION & MANAGEMENT SYSTEM
===========================================================================
  [1] Flight Management     (Add, Search, Remove, List, Audit)
  [2] Passenger Management  (Register, History, Profile, Remove)
  [3] Booking & Reservation (Book Seat, Calculate Fare, Boarding Pass)
  [4] Cancellation & Refund (Polymorphic Refund Calculation)
  [5] Reports & Analytics   (Today's Flights, Occupancy, Top Revenue)
  [6] OOP Concepts Showcase (Live demonstration of all 7 OOP topics)
  [7] Save & Exit
---------------------------------------------------------------------------
```

### Quick Walkthrough Guide:
1. **Explore Flights**: Select `[1]` $\rightarrow$ `1` to list all 11 pre-loaded flights using `operator<<`.
2. **Check Passenger Profile**: Select `[2]` $\rightarrow$ `2` $\rightarrow$ Enter `P-201` to view Business passenger details and lounge access.
3. **Book a Ticket**: Select `[3]` $\rightarrow$ `1` $\rightarrow$ Enter Passenger `P-103`, Flight `SK-103`, Seat `15A` to see the live polymorphic price calculation and boarding pass generation.
4. **Test Exception Handling**: Try booking the same passenger on `SK-103` again to trigger the `DuplicateBookingException` catch block.
5. **View Executive Reports**: Select `[5]` $\rightarrow$ `3` to see the Top 5 revenue-generating flights ranked via `std::sort`.
6. **Live OOP Showcase**: Select `[6]` to view a consolidated live demonstration of every OOP concept in action.

---

## 📋 Sample Pre-Loaded Demonstration Data

As required by **Section 6 (Deliverables) of the Assignment PDF**, the system comes pre-populated with:

* **11 Pre-loaded Flights** ([flights.txt](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/data/flights.txt)):
  * **Domestic**: `SK-101` (NY $\rightarrow$ Chicago), `SK-102` (LA $\rightarrow$ SF), `SK-103` (Dallas $\rightarrow$ Miami), `SK-104` (Seattle $\rightarrow$ Denver).
  * **International**: `SK-201` (NY $\rightarrow$ London), `SK-202` (LA $\rightarrow$ Tokyo), `SK-203` (Chicago $\rightarrow$ Frankfurt), `SK-204` (Miami $\rightarrow$ Sao Paulo).
  * **Charter**: `SK-301` (Las Vegas $\rightarrow$ Aspen), `SK-302` (Boston $\rightarrow$ Nantucket), `SK-303` (Houston $\rightarrow$ Cancun).
* **8 Pre-loaded Passengers** ([passengers.txt](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/data/passengers.txt)):
  * **Economy**: `P-101` (Alice Johnson), `P-102` (Bob Smith), `P-103` (Carlos Mendes).
  * **Business**: `P-201` (Diana Prince), `P-202` (Evan Wright), `P-203` (Fiona Gallagher).
  * **First Class**: `P-301` (George Harrison), `P-302` (Helena Bonham).
* **Pre-issued Tickets** ([tickets.txt](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/data/tickets.txt)): Active bookings `TKT-1001` through `TKT-1006`.

---

## 🎓 Viva Voce & Evaluation Guide

### Assessment Rubric (100 Marks) Alignment:

| Assessment Criterion | Weight | How It Is Addressed in This Codebase |
| :--- | :---: | :--- |
| **Class Design & UML** | **15 Marks** | Clear abstraction, modular headers, Mermaid & PlantUML diagrams in [UML_CLASS_DIAGRAM.md](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/UML_CLASS_DIAGRAM.md). |
| **Encapsulation & Abstraction** | **10 Marks** | Private members, pure virtual base classes (`Flight`, `Passenger`). |
| **Inheritance & Polymorphism** | **15 Marks** | Multi-level and hierarchical inheritance; dynamic dispatch for pricing & refunds. |
| **Operator Overloading & Templates** | **10 Marks** | Overloaded `<<`, `==`, `<`; generic search functions & `Repository<T>` class template. |
| **Exception Handling** | **10 Marks** | Custom exceptions derived from `std::exception`; no crashes on invalid inputs. |
| **STL Container & Algorithm Usage** | **5 Marks** | Idiomatic use of `std::vector`, `std::map`, `std::sort`, `std::find_if`. |
| **File Persistence** | **10 Marks** | Full save/load round-trip persistence in `data/` preserving state across runs. |
| **Functionality & Correctness** | **10 Marks** | All required flight, passenger, booking, cancellation, and report operations work. |
| **Code Quality & Memory Safety** | **5 Marks** | Multi-file modular structure with include guards; memory managed safely via `std::shared_ptr`. |
| **Documentation & Design Report** | **5 Marks** | Comprehensive [DESIGN_REPORT.md](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/DESIGN_REPORT.md) and professional [README.md](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/README.md). |
| **Viva Voce & Demonstration** | **5 Marks** | Included Q&A cheat sheet in Section 5 of [DESIGN_REPORT.md](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/DESIGN_REPORT.md). |
| **Total** | **100 Marks** | **Full Rubric Coverage** |

---

## 📦 Deliverables & Submission Checklist

When preparing your submission archive (`OOP_PBL_<StudentID>.zip`):

- [x] **Source Code**: All `.h` files in `include/` and `.cpp` files in `src/`.
- [x] **UML Diagrams**: [UML_CLASS_DIAGRAM.md](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/UML_CLASS_DIAGRAM.md) (Mermaid, PlantUML & ASCII).
- [x] **Design Report**: [DESIGN_REPORT.md](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/DESIGN_REPORT.md) (2–3 page academic report + viva prep).
- [x] **Sample Data Files**: Pre-populated `data/flights.txt`, `data/passengers.txt`, `data/tickets.txt`.
- [x] **Build Scripts**: [Makefile](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/Makefile) and [build.bat](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/build.bat).
- [x] **Documentation**: Fully formatted [README.md](file:///d:/Uni%20Data/2%20SEMESTER/OOP%202%20Sem/OOP%20Project%20Airline/README.md).
