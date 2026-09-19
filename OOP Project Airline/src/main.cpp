#include <iostream>
#include <memory>
#include <iomanip>
#include "../include/Airline.h"
#include "../include/Exceptions.h"
#include "../include/Utils.h"

// Constants for file storage
const std::string FLIGHTS_FILE = "data/flights.txt";
const std::string PASSENGERS_FILE = "data/passengers.txt";
const std::string TICKETS_FILE = "data/tickets.txt";

// Sub-menu handlers
void handleFlightMenu(Airline& airline);
void handlePassengerMenu(Airline& airline);
void handleBookingMenu(Airline& airline);
void handleCancellationMenu(Airline& airline);
void handleReportsMenu(Airline& airline);
void handleOOPDemoMenu(Airline& airline);

void printMainMenu(const std::string& airlineName) {
    Utils::printHeader("       " + airlineName + " - FLIGHT RESERVATION & MANAGEMENT SYSTEM");
    std::cout << "  [1] Flight Management     (Add, Search, Remove, List, Audit)\n";
    std::cout << "  [2] Passenger Management  (Register, History, Profile, Remove)\n";
    std::cout << "  [3] Booking & Reservation (Book Seat, Calculate Fare, Boarding Pass)\n";
    std::cout << "  [4] Cancellation & Refund (Polymorphic Refund Calculation)\n";
    std::cout << "  [5] Reports & Analytics   (Today's Flights, Occupancy, Top Revenue)\n";
    std::cout << "  [6] OOP Concepts Showcase (Live demonstration of all 7 OOP topics)\n";
    std::cout << "  [7] Save & Exit\n";
    Utils::printDivider('-', 75);
}

int main() {
    std::cout << "\n===========================================================================\n";
    std::cout << "         WELCOME TO SKYLINK AIRWAYS FLIGHT MANAGEMENT SYSTEM               \n";
    std::cout << "                  Object-Oriented Programming (C++)                        \n";
    std::cout << "===========================================================================\n";

    Airline skyLink("SkyLink Airways");

    // Load data from persistent files
    try {
        std::cout << "[i] Loading airline records from data files...\n";
        skyLink.loadFromFile(FLIGHTS_FILE, PASSENGERS_FILE, TICKETS_FILE);
        std::cout << "[+] Records loaded successfully!\n";
    } catch (const AirlineException& e) {
        std::cout << "[!] Notice: " << e.what() << "\n";
        std::cout << "[+] Initialized with default sample records.\n";
    } catch (const std::exception& e) {
        std::cout << "[!] System error during initialization: " << e.what() << "\n";
    }

    bool running = true;
    while (running && !std::cin.eof()) {
        printMainMenu(skyLink.getAirlineName());
        int choice = Utils::readInt("Select an option (1-7): ", 1, 7);
        if (std::cin.eof()) {
            break;
        }

        switch (choice) {
            case 1:
                handleFlightMenu(skyLink);
                break;
            case 2:
                handlePassengerMenu(skyLink);
                break;
            case 3:
                handleBookingMenu(skyLink);
                break;
            case 4:
                handleCancellationMenu(skyLink);
                break;
            case 5:
                handleReportsMenu(skyLink);
                break;
            case 6:
                handleOOPDemoMenu(skyLink);
                break;
            case 7: {
                std::cout << "\n[i] Saving changes to persistent storage...\n";
                try {
                    skyLink.saveToFile(FLIGHTS_FILE, PASSENGERS_FILE, TICKETS_FILE);
                    std::cout << "[+] Data saved successfully to data/ directory.\n";
                } catch (const AirlineException& e) {
                    std::cout << "[!] Warning: " << e.what() << "\n";
                }
                std::cout << "\nThank you for using SkyLink Airways System. Goodbye!\n\n";
                running = false;
                break;
            }
            default:
                std::cout << "[!] Invalid option selected.\n";
                break;
        }
    }

    return 0;
}

// ============================================================================
// 1. FLIGHT MANAGEMENT MENU
// ============================================================================
void handleFlightMenu(Airline& airline) {
    bool inMenu = true;
    while (inMenu) {
        Utils::printHeader("FLIGHT MANAGEMENT");
        std::cout << "  1. List All Flights (operator<<)\n";
        std::cout << "  2. View Full Flight Details (Polymorphic display)\n";
        std::cout << "  3. Add New Flight (Domestic / International / Charter)\n";
        std::cout << "  4. Search Flights by Flight Number\n";
        std::cout << "  5. Search Flights by Route (Origin -> Destination)\n";
        std::cout << "  6. Search Flights by Departure Date\n";
        std::cout << "  7. Remove a Flight\n";
        std::cout << "  8. Run Flight Internal Audit (Friend Class Demo)\n";
        std::cout << "  9. Return to Main Menu\n";
        Utils::printDivider('-', 75);

        int choice = Utils::readInt("Enter choice (1-9): ", 1, 9);
        switch (choice) {
            case 1: {
                airline.listAllFlights();
                Utils::pauseScreen();
                break;
            }
            case 2: {
                std::string fNum = Utils::readLine("Enter Flight Number (e.g., SK-101): ");
                auto flt = airline.getFlight(fNum);
                if (flt) {
                    flt->displayDetails();
                } else {
                    std::cout << "[!] Flight [" << fNum << "] not found.\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 3: {
                Utils::printHeader("ADD NEW FLIGHT");
                std::cout << "Select Flight Type:\n";
                std::cout << "  1. Domestic Flight\n";
                std::cout << "  2. International Flight\n";
                std::cout << "  3. Charter Flight\n";
                int typeChoice = Utils::readInt("Enter type (1-3): ", 1, 3);

                std::string fNum = Utils::readLine("Flight Number (e.g., SK-901): ");
                std::string orig = Utils::readLine("Origin City (e.g., Boston): ");
                std::string dest = Utils::readLine("Destination City (e.g., Chicago): ");
                std::string depTime = Utils::readLine("Departure Date/Time (YYYY-MM-DD HH:MM): ");
                int seats = Utils::readInt("Total Passenger Seats: ", 1, 600);
                double rate = Utils::readDouble("Base Flight Rate ($): ", 10.0, 50000.0);

                try {
                    std::shared_ptr<Flight> newFlight = nullptr;
                    if (typeChoice == 1) {
                        double tax = Utils::readDouble("State Tax Rate (e.g. 0.05 for 5%): ", 0.0, 0.5);
                        bool subsidized = Utils::readBool("Is this route government subsidized?");
                        double luggageFee = Utils::readDouble("Airport Luggage Fee ($): ", 0.0, 100.0);
                        newFlight = std::make_shared<DomesticFlight>(fNum, orig, dest, depTime, seats, rate, tax, subsidized, luggageFee);
                    } else if (typeChoice == 2) {
                        bool visaReq = Utils::readBool("Is a destination visa required?");
                        double customs = Utils::readDouble("Customs Duty Fee ($): ", 0.0, 200.0);
                        int validMonths = Utils::readInt("Minimum Passport Validity (Months): ", 0, 24);
                        newFlight = std::make_shared<InternationalFlight>(fNum, orig, dest, depTime, seats, rate, visaReq, customs, validMonths);
                    } else if (typeChoice == 3) {
                        std::string holder = Utils::readLine("Contract Holder Name/Company: ");
                        bool catering = Utils::readBool("Include VIP Gourmet Catering?");
                        double hrRate = Utils::readDouble("Charter Hourly Rate ($): ", 100.0, 10000.0);
                        int hrs = Utils::readInt("Flight Duration in Hours: ", 1, 24);
                        newFlight = std::make_shared<CharterFlight>(fNum, orig, dest, depTime, seats, rate, holder, catering, hrRate, hrs);
                    }

                    if (newFlight) {
                        airline.addFlight(newFlight);
                        std::cout << "\n[+] Flight [" << fNum << "] registered successfully!\n";
                        newFlight->displayDetails();
                    }
                } catch (const AirlineException& e) {
                    std::cout << "[!] Error adding flight: " << e.what() << "\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 4: {
                std::string fNum = Utils::readLine("Enter Flight Number to search: ");
                auto results = airline.searchFlights(fNum);
                printCollection(results, "Flight Search Results for '" + fNum + "'");
                Utils::pauseScreen();
                break;
            }
            case 5: {
                std::string orig = Utils::readLine("Enter Origin City: ");
                std::string dest = Utils::readLine("Enter Destination City: ");
                auto results = airline.searchFlights(orig, dest);
                printCollection(results, "Flights Matching Route: " + orig + " -> " + dest);
                Utils::pauseScreen();
                break;
            }
            case 6: {
                std::string date = Utils::readLine("Enter Departure Date (YYYY-MM-DD): ");
                auto results = airline.searchFlightsByDate(date);
                printCollection(results, "Flights Departing on Date: " + date);
                Utils::pauseScreen();
                break;
            }
            case 7: {
                std::string fNum = Utils::readLine("Enter Flight Number to remove: ");
                try {
                    if (airline.removeFlight(fNum)) {
                        std::cout << "[+] Flight [" << fNum << "] removed successfully.\n";
                    } else {
                        std::cout << "[!] Flight [" << fNum << "] not found.\n";
                    }
                } catch (const AirlineException& e) {
                    std::cout << "[!] " << e.what() << "\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 8: {
                std::string fNum = Utils::readLine("Enter Flight Number to audit: ");
                auto flt = airline.getFlight(fNum);
                if (flt) {
                    AirlineReportHelper::generateFlightAudit(*flt);
                } else {
                    std::cout << "[!] Flight [" << fNum << "] not found.\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 9:
                inMenu = false;
                break;
        }
    }
}

// ============================================================================
// 2. PASSENGER MANAGEMENT MENU
// ============================================================================
void handlePassengerMenu(Airline& airline) {
    bool inMenu = true;
    while (inMenu) {
        Utils::printHeader("PASSENGER MANAGEMENT");
        std::cout << "  1. List All Registered Passengers\n";
        std::cout << "  2. View Passenger Profile & Details\n";
        std::cout << "  3. View Passenger Travel & Booking History\n";
        std::cout << "  4. Register New Passenger (Economy / Business / First Class)\n";
        std::cout << "  5. Remove Passenger Record\n";
        std::cout << "  6. Return to Main Menu\n";
        Utils::printDivider('-', 75);

        int choice = Utils::readInt("Enter choice (1-6): ", 1, 6);
        switch (choice) {
            case 1: {
                airline.listAllPassengers();
                Utils::pauseScreen();
                break;
            }
            case 2: {
                std::string pId = Utils::readLine("Enter Passenger ID (e.g., P-101): ");
                auto psg = airline.getPassenger(pId);
                if (psg) {
                    psg->displayDetails();
                } else {
                    std::cout << "[!] Passenger [" << pId << "] not found.\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 3: {
                std::string pId = Utils::readLine("Enter Passenger ID (e.g., P-101): ");
                try {
                    airline.viewPassengerHistory(pId);
                } catch (const AirlineException& e) {
                    std::cout << "[!] " << e.what() << "\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 4: {
                Utils::printHeader("REGISTER NEW PASSENGER");
                std::cout << "Select Passenger Tier:\n";
                std::cout << "  1. Economy Passenger (Standard Fare, 20kg Baggage)\n";
                std::cout << "  2. Business Passenger (1.6x Fare, 35kg Baggage, Lounge Access)\n";
                std::cout << "  3. First Class Passenger (2.4x Fare, 50kg Baggage, Chauffeur, Suite)\n";
                int tierChoice = Utils::readInt("Enter tier (1-3): ", 1, 3);

                std::string pId = Utils::readLine("Passenger ID (e.g., P-501): ");
                std::string name = Utils::readLine("Full Name: ");
                std::string passport = Utils::readLine("Passport Number: ");
                std::string email = Utils::readLine("Email Address: ");
                std::string phone = Utils::readLine("Phone Number: ");

                try {
                    std::shared_ptr<Passenger> newPassenger = nullptr;
                    if (tierChoice == 1) {
                        bool vegan = Utils::readBool("Dietary preference - Vegan Meal?");
                        newPassenger = std::make_shared<EconomyPassenger>(pId, name, passport, email, phone, 0, vegan);
                    } else if (tierChoice == 2) {
                        bool lounge = Utils::readBool("VIP Lounge Access required?");
                        std::string company = Utils::readLine("Corporate Sponsor / Company Name: ");
                        newPassenger = std::make_shared<BusinessPassenger>(pId, name, passport, email, phone, 0, lounge, company);
                    } else if (tierChoice == 3) {
                        bool chauffeur = Utils::readBool("Luxury Chauffeur Pickup required?");
                        std::string suite = Utils::readLine("Suite Preference (e.g. Royal Suite / Presidential Suite): ");
                        newPassenger = std::make_shared<FirstClassPassenger>(pId, name, passport, email, phone, 0, chauffeur, suite);
                    }

                    if (newPassenger) {
                        airline.registerPassenger(newPassenger);
                        std::cout << "\n[+] Passenger [" << name << " (" << pId << ")] registered successfully!\n";
                        newPassenger->displayDetails();
                    }
                } catch (const AirlineException& e) {
                    std::cout << "[!] Registration error: " << e.what() << "\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 5: {
                std::string pId = Utils::readLine("Enter Passenger ID to remove: ");
                try {
                    if (airline.removePassenger(pId)) {
                        std::cout << "[+] Passenger [" << pId << "] removed successfully.\n";
                    } else {
                        std::cout << "[!] Passenger [" << pId << "] not found.\n";
                    }
                } catch (const AirlineException& e) {
                    std::cout << "[!] " << e.what() << "\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 6:
                inMenu = false;
                break;
        }
    }
}

// ============================================================================
// 3. BOOKING & RESERVATION MENU
// ============================================================================
void handleBookingMenu(Airline& airline) {
    bool inMenu = true;
    while (inMenu) {
        Utils::printHeader("FLIGHT BOOKING & RESERVATION");
        std::cout << "  1. Book Flight Ticket (Seat Allocation & Polymorphic Pricing)\n";
        std::cout << "  2. View / Print Boarding Pass (Ticket Lookup)\n";
        std::cout << "  3. List All Issued Tickets (operator<<)\n";
        std::cout << "  4. Return to Main Menu\n";
        Utils::printDivider('-', 75);

        int choice = Utils::readInt("Enter choice (1-4): ", 1, 4);
        switch (choice) {
            case 1: {
                Utils::printHeader("BOOK FLIGHT TICKET");
                std::string pId = Utils::readLine("Enter Passenger ID (e.g., P-101): ");
                std::string fNum = Utils::readLine("Enter Flight Number (e.g., SK-101): ");
                std::string seat = Utils::readLine("Enter Desired Seat Number (e.g., 14B): ");

                try {
                    auto ticket = airline.bookFlight(pId, fNum, seat);
                    if (ticket) {
                        std::cout << "\n[+] TICKET BOOKED SUCCESSFULLY!\n";
                        ticket->displayTicket();
                    }
                } catch (const FlightFullException& e) {
                    std::cout << "\n[!] BOOKING REJECTED: " << e.what() << "\n";
                } catch (const DuplicateBookingException& e) {
                    std::cout << "\n[!] BOOKING REJECTED: " << e.what() << "\n";
                } catch (const AirlineException& e) {
                    std::cout << "\n[!] ERROR: " << e.what() << "\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 2: {
                std::string tId = Utils::readLine("Enter Ticket ID (e.g., TKT-1001): ");
                auto ticket = airline.getTicket(tId);
                if (ticket) {
                    ticket->displayTicket();
                } else {
                    std::cout << "[!] Ticket [" << tId << "] not found.\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 3: {
                airline.listAllTickets();
                Utils::pauseScreen();
                break;
            }
            case 4:
                inMenu = false;
                break;
        }
    }
}

// ============================================================================
// 4. CANCELLATION & REFUND MENU
// ============================================================================
void handleCancellationMenu(Airline& airline) {
    bool inMenu = true;
    while (inMenu) {
        Utils::printHeader("TICKET CANCELLATION & REFUND");
        std::cout << "  1. Cancel Ticket & Calculate Refund\n";
        std::cout << "  2. Return to Main Menu\n";
        Utils::printDivider('-', 75);

        int choice = Utils::readInt("Enter choice (1-2): ", 1, 2);
        switch (choice) {
            case 1: {
                std::string tId = Utils::readLine("Enter Ticket ID to cancel (e.g., TKT-1001): ");
                try {
                    auto tkt = airline.getTicket(tId);
                    if (!tkt) {
                        std::cout << "[!] Ticket [" << tId << "] not found.\n";
                        Utils::pauseScreen();
                        break;
                    }

                    std::cout << "\nFound Ticket: " << *tkt << "\n";
                    bool confirm = Utils::readBool("Are you sure you want to cancel this booking?");
                    if (!confirm) {
                        std::cout << "Cancellation aborted by user.\n";
                        Utils::pauseScreen();
                        break;
                    }

                    double refund = airline.cancelBooking(tId);
                    std::cout << "\n[+] Ticket [" << tId << "] has been cancelled successfully.\n";
                    std::cout << "    Original Fare Paid: $" << std::fixed << std::setprecision(2) << tkt->getFarePaid() << "\n";
                    std::cout << "    Polymorphic Refund: $" << refund << "\n";
                    std::cout << "    Seat on Flight [" << tkt->getFlightNumber() << "] has been restored to available inventory.\n";
                } catch (const InvalidCancellationException& e) {
                    std::cout << "\n[!] CANCELLATION ERROR: " << e.what() << "\n";
                } catch (const AirlineException& e) {
                    std::cout << "\n[!] ERROR: " << e.what() << "\n";
                }
                Utils::pauseScreen();
                break;
            }
            case 2:
                inMenu = false;
                break;
        }
    }
}

// ============================================================================
// 5. REPORTS & ANALYTICS MENU
// ============================================================================
void handleReportsMenu(Airline& airline) {
    bool inMenu = true;
    while (inMenu) {
        Utils::printHeader("REPORTS & ANALYTICS");
        std::cout << "  1. Today's Scheduled Departures\n";
        std::cout << "  2. Flight Occupancy & Capacity Utilization Report\n";
        std::cout << "  3. Top 5 Highest-Revenue Flights (STL Sort)\n";
        std::cout << "  4. Executive Financial Summary\n";
        std::cout << "  5. Return to Main Menu\n";
        Utils::printDivider('-', 75);

        int choice = Utils::readInt("Enter choice (1-5): ", 1, 5);
        switch (choice) {
            case 1: {
                std::string customDate = Utils::readLine("Enter Date (leave blank for today YYYY-MM-DD): ");
                airline.reportTodaysDepartures(customDate);
                Utils::pauseScreen();
                break;
            }
            case 2: {
                airline.reportOccupancyRates();
                Utils::pauseScreen();
                break;
            }
            case 3: {
                airline.reportTopRevenueFlights(5);
                Utils::pauseScreen();
                break;
            }
            case 4: {
                airline.reportFinancialSummary();
                Utils::pauseScreen();
                break;
            }
            case 5:
                inMenu = false;
                break;
        }
    }
}

// ============================================================================
// 6. OOP CONCEPTS SHOWCASE MENU
// ============================================================================
void handleOOPDemoMenu(Airline& airline) {
    Utils::printHeader("OBJECT-ORIENTED PROGRAMMING CONCEPTS SHOWCASE");
    std::cout << "\n--- TOPIC 1: Classes, Encapsulation, Abstraction, Aggregation ---\n";
    std::cout << " * Encapsulation: Flight & Passenger state protected; modified only via methods.\n";
    std::cout << " * Abstraction: Flight is an abstract base class with pure virtual methods.\n";
    std::cout << " * Aggregation: Airline aggregates Flights, Passengers, and Tickets.\n";

    std::cout << "\n--- TOPIC 2: Inheritance & Hierarchy ---\n";
    std::cout << " * Flight Hierarchy: Flight -> DomesticFlight, InternationalFlight, CharterFlight\n";
    std::cout << " * Passenger Hierarchy: Passenger -> EconomyPassenger, BusinessPassenger, FirstClassPassenger\n";
    std::cout << " * Exception Hierarchy: std::exception -> AirlineException -> [FlightFull, Duplicate, etc.]\n";

    std::cout << "\n--- TOPIC 3: Overloading, Overriding & Operator Overloading ---\n";
    std::cout << " * Function Overloading: Airline::searchFlights(string) vs Airline::searchFlights(orig, dest)\n";
    std::cout << " * Function Overriding: calculateBaseFare(), calculateRefund(), displayDetails()\n";
    std::cout << " * Operator Overloading: operator<< for Flight & Ticket, operator== for Ticket comparison\n";

    std::cout << "\n--- TOPIC 4: Polymorphism, Friend Functions & Friend Classes ---\n";
    std::cout << " * Runtime Polymorphism: Dynamic fare calculation & tier refund logic.\n";
    std::cout << " * Friend Function: operator<<(ostream&, const Flight&) & operator<<(ostream&, const Ticket&)\n";
    std::cout << " * Friend Class: AirlineReportHelper accesses private Flight attributes for internal audit.\n";

    std::cout << "\n--- TOPIC 5: File Handling & Persistence ---\n";
    std::cout << " * Text file serialization and deserialization in data/flights.txt, data/passengers.txt, data/tickets.txt\n";

    std::cout << "\n--- TOPIC 6: Templates (Generic Programming) ---\n";
    std::cout << " * Function Templates: genericSearch<T>(), printCollection<T>(), findById<T>()\n";
    std::cout << " * Class Template: Repository<T> container managing Flight, Passenger, and Ticket objects\n";

    std::cout << "\n--- TOPIC 7: Exception Handling ---\n";
    std::cout << " * Robust try-catch blocks with custom exceptions preventing application crash.\n";

    // Run a live Friend Class audit demo
    auto sampleFlight = airline.getFlight("SK-101");
    if (sampleFlight) {
        std::cout << "\n>>> LIVE FRIEND CLASS AUDIT DEMONSTRATION <<<";
        AirlineReportHelper::generateFlightAudit(*sampleFlight);
    }

    Utils::pauseScreen();
}
