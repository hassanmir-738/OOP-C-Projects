#include "../include/Airline.h"
#include "../include/Exceptions.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

// ============================================================================
// FRIEND CLASS DEMONSTRATION
// ============================================================================
void AirlineReportHelper::generateFlightAudit(const Flight& flight) {
    // Accessing protected/private members of Flight directly due to friendship
    std::cout << "\n[AUDIT REPORT] Flight #" << flight.flightNumber
              << " | Total Seats: " << flight.totalSeats
              << " | Available Seats: " << flight.availableSeats
              << " | Base Rate: $" << flight.baseRate
              << " | Accumulated Revenue: $" << flight.totalRevenue << "\n";
}

// ============================================================================
// AIRLINE CLASS IMPLEMENTATION
// ============================================================================

Airline::Airline(const std::string& name)
    : airlineName(name), nextTicketCounter(1001) {}

std::string Airline::getAirlineName() const {
    return airlineName;
}

std::string Airline::generateNextTicketId() {
    std::ostringstream oss;
    oss << "TKT-" << nextTicketCounter++;
    return oss.str();
}

void Airline::updateBookingIndex() {
    passengerBookingIndex.clear();
    for (const auto& ticket : ticketRepo.getAll()) {
        passengerBookingIndex[ticket->getPassengerId()].push_back(ticket->getTicketId());
    }
}

// ----------------------------------------------------------------------------
// 1. FLIGHT MANAGEMENT
// ----------------------------------------------------------------------------

void Airline::addFlight(std::shared_ptr<Flight> flight) {
    if (!flight) return;
    // Check if flight with same number exists
    auto existing = getFlight(flight->getFlightNumber());
    if (existing) {
        throw AirlineException("Flight with number [" + flight->getFlightNumber() + "] already exists.");
    }
    flightRepo.add(flight);
}

bool Airline::removeFlight(const std::string& flightNumber) {
    // Check if flight has active tickets
    for (const auto& t : ticketRepo.getAll()) {
        if (t->getFlightNumber() == flightNumber && t->isActive()) {
            throw AirlineException("Cannot remove Flight [" + flightNumber + "] because it has active passenger bookings!");
        }
    }
    return flightRepo.remove([&](const std::shared_ptr<Flight>& f) {
        return f->getFlightNumber() == flightNumber;
    });
}

std::shared_ptr<Flight> Airline::getFlight(const std::string& flightNumber) const {
    return flightRepo.find([&](const std::shared_ptr<Flight>& f) {
        return f->getFlightNumber() == flightNumber;
    });
}

void Airline::listAllFlights() const {
    printCollection(flightRepo.getAll(), "All Available Flights");
}

// Overload 1: Search by flight number
std::vector<std::shared_ptr<Flight>> Airline::searchFlights(const std::string& flightNumber) const {
    std::string upperTarget = Utils::toUpper(Utils::trim(flightNumber));
    return flightRepo.filter([&](const std::shared_ptr<Flight>& f) {
        return Utils::toUpper(f->getFlightNumber()).find(upperTarget) != std::string::npos;
    });
}

// Overload 2: Search by route (origin & destination)
std::vector<std::shared_ptr<Flight>> Airline::searchFlights(const std::string& origin, const std::string& destination) const {
    std::string origUpper = Utils::toUpper(Utils::trim(origin));
    std::string destUpper = Utils::toUpper(Utils::trim(destination));
    return flightRepo.filter([&](const std::shared_ptr<Flight>& f) {
        return (Utils::toUpper(f->getOrigin()).find(origUpper) != std::string::npos &&
                Utils::toUpper(f->getDestination()).find(destUpper) != std::string::npos);
    });
}

// Search by date prefix (e.g. "2026-10-15")
std::vector<std::shared_ptr<Flight>> Airline::searchFlightsByDate(const std::string& datePrefix) const {
    std::string dateTrimmed = Utils::trim(datePrefix);
    return flightRepo.filter([&](const std::shared_ptr<Flight>& f) {
        return f->getDepartureDateTime().find(dateTrimmed) == 0;
    });
}

// ----------------------------------------------------------------------------
// 2. PASSENGER MANAGEMENT
// ----------------------------------------------------------------------------

void Airline::registerPassenger(std::shared_ptr<Passenger> passenger) {
    if (!passenger) return;
    auto existing = getPassenger(passenger->getPassengerId());
    if (existing) {
        throw AirlineException("Passenger with ID [" + passenger->getPassengerId() + "] already registered.");
    }
    passengerRepo.add(passenger);
}

bool Airline::removePassenger(const std::string& passengerId) {
    // Check if passenger has active bookings
    for (const auto& t : ticketRepo.getAll()) {
        if (t->getPassengerId() == passengerId && t->isActive()) {
            throw AirlineException("Cannot remove Passenger [" + passengerId + "] with active flight bookings!");
        }
    }
    return passengerRepo.remove([&](const std::shared_ptr<Passenger>& p) {
        return p->getPassengerId() == passengerId;
    });
}

std::shared_ptr<Passenger> Airline::getPassenger(const std::string& passengerId) const {
    return passengerRepo.find([&](const std::shared_ptr<Passenger>& p) {
        return p->getPassengerId() == passengerId;
    });
}

void Airline::listAllPassengers() const {
    printCollection(passengerRepo.getAll(), "Registered Passengers");
}

void Airline::viewPassengerHistory(const std::string& passengerId) const {
    auto passenger = getPassenger(passengerId);
    if (!passenger) {
        throw PassengerNotFoundException(passengerId);
    }

    passenger->displayDetails();

    std::cout << "\n>>> Booking History for Passenger: " << passenger->getName() << " <<<\n";
    auto it = passengerBookingIndex.find(passengerId);
    if (it == passengerBookingIndex.end() || it->second.empty()) {
        std::cout << "  [No bookings found for this passenger]\n";
        return;
    }

    for (const auto& ticketId : it->second) {
        auto ticket = getTicket(ticketId);
        if (ticket) {
            std::cout << "  * " << *ticket << "\n";
        }
    }
}

// ----------------------------------------------------------------------------
// 3. BOOKING OPERATIONS (POLYMORPHIC FARE CALCULATION)
// ----------------------------------------------------------------------------

std::shared_ptr<Ticket> Airline::bookFlight(const std::string& passengerId,
                                            const std::string& flightNumber,
                                            const std::string& seatNumber) {
    // 1. Verify Passenger
    auto passenger = getPassenger(passengerId);
    if (!passenger) {
        throw PassengerNotFoundException(passengerId);
    }

    // 2. Verify Flight
    auto flight = getFlight(flightNumber);
    if (!flight) {
        throw FlightNotFoundException(flightNumber);
    }

    // 3. Check if Flight is Full
    if (flight->getAvailableSeats() <= 0) {
        throw FlightFullException(flightNumber);
    }

    // 4. Check for Duplicate Active Booking on the same flight
    for (const auto& t : ticketRepo.getAll()) {
        if (t->getPassengerId() == passengerId &&
            t->getFlightNumber() == flightNumber &&
            t->isActive()) {
            throw DuplicateBookingException(passengerId, flightNumber);
        }
    }

    // 5. Polymorphic Fare Calculation:
    // Base flight fare calculated by Flight subclass (Domestic, International, Charter)
    double baseFlightFare = flight->calculateBaseFare();
    // Class multiplier & fees calculated by Passenger subclass (Economy, Business, First Class)
    double finalFare = passenger->calculateFare(baseFlightFare);

    // 6. Book Seat & Update Flight Revenue
    if (!flight->bookSeat()) {
        throw FlightFullException(flightNumber);
    }
    flight->addRevenue(finalFare);

    // 7. Add Loyalty Points based on passenger loyalty multiplier
    int earnedPoints = static_cast<int>(finalFare * passenger->getLoyaltyMultiplier() * 0.05);
    passenger->addLoyaltyPoints(earnedPoints);

    // 8. Create Ticket
    std::string ticketId = generateNextTicketId();
    std::string bookingDate = Utils::getCurrentDate();
    auto newTicket = std::make_shared<Ticket>(ticketId, passengerId, flightNumber,
                                              seatNumber, finalFare, bookingDate, "CONFIRMED");

    ticketRepo.add(newTicket);
    passengerBookingIndex[passengerId].push_back(ticketId);

    return newTicket;
}

// ----------------------------------------------------------------------------
// 4. CANCELLATION & REFUND OPERATIONS (POLYMORPHIC REFUND CALCULATION)
// ----------------------------------------------------------------------------

double Airline::cancelBooking(const std::string& ticketId) {
    auto ticket = getTicket(ticketId);
    if (!ticket) {
        throw AirlineException("Ticket ID [" + ticketId + "] not found.");
    }

    if (!ticket->isActive()) {
        throw InvalidCancellationException("Ticket [" + ticketId + "] has already been cancelled!");
    }

    auto flight = getFlight(ticket->getFlightNumber());
    auto passenger = getPassenger(ticket->getPassengerId());

    if (!flight || !passenger) {
        throw AirlineException("Associated flight or passenger records are missing for ticket [" + ticketId + "].");
    }

    // Calculate days before departure
    std::string currentDate = Utils::getCurrentDate();
    int daysBeforeDeparture = Utils::calculateDaysDifference(currentDate, flight->getDepartureDateTime());
    if (daysBeforeDeparture < 0) {
        throw InvalidCancellationException("Cannot cancel ticket for a flight that has already departed!");
    }

    // Polymorphic Refund Calculation based on Passenger Class and notice period
    double refundAmount = passenger->calculateRefund(ticket->getFarePaid(), daysBeforeDeparture);

    // Update Ticket, Flight Seats, and Flight Revenue
    ticket->cancelTicket(refundAmount);
    flight->releaseSeat();
    flight->refundRevenue(refundAmount);

    return refundAmount;
}

std::shared_ptr<Ticket> Airline::getTicket(const std::string& ticketId) const {
    return ticketRepo.find([&](const std::shared_ptr<Ticket>& t) {
        return t->getTicketId() == ticketId;
    });
}

void Airline::listAllTickets() const {
    printCollection(ticketRepo.getAll(), "All Tickets / Bookings");
}

// ----------------------------------------------------------------------------
// 5. REPORTS
// ----------------------------------------------------------------------------

void Airline::reportTodaysDepartures(const std::string& targetDate) const {
    std::string dateToSearch = targetDate.empty() ? Utils::getCurrentDate().substr(0, 10) : targetDate;
    Utils::printHeader("TODAY'S SCHEDULED DEPARTURES (" + dateToSearch + ")");

    auto departures = searchFlightsByDate(dateToSearch);
    if (departures.empty()) {
        std::cout << "  No departures scheduled for date: " << dateToSearch << "\n";
    } else {
        for (size_t i = 0; i < departures.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << *departures[i] << "\n";
        }
    }
}

void Airline::reportOccupancyRates() const {
    Utils::printHeader("FLIGHT OCCUPANCY & CAPACITY REPORT");
    std::cout << std::left
              << std::setw(12) << "Flight No"
              << std::setw(16) << "Type"
              << std::setw(32) << "Route"
              << std::setw(10) << "Booked"
              << std::setw(10) << "Total"
              << std::setw(14) << "Occupancy %"
              << "Status" << "\n";
    Utils::printDivider('-', 95);

    for (const auto& f : flightRepo.getAll()) {
        double occ = f->getOccupancyRate();
        std::string status = (f->getAvailableSeats() == 0) ? "[FULL]" : (occ >= 75.0 ? "[HIGH]" : "[NORMAL]");
        std::string route = f->getOrigin() + " -> " + f->getDestination();

        std::cout << std::left
                  << std::setw(12) << f->getFlightNumber()
                  << std::setw(16) << f->getFlightType()
                  << std::setw(32) << route
                  << std::setw(10) << f->getBookedSeats()
                  << std::setw(10) << f->getTotalSeats()
                  << std::fixed << std::setprecision(1) << std::setw(14) << occ
                  << status << "\n";
    }
}

void Airline::reportTopRevenueFlights(size_t topN) const {
    Utils::printHeader("TOP " + std::to_string(topN) + " HIGHEST-REVENUE FLIGHTS");

    // Copy flights vector for sorting using STL sort algorithm
    auto flights = flightRepo.getAll();
    std::sort(flights.begin(), flights.end(), [](const std::shared_ptr<Flight>& a, const std::shared_ptr<Flight>& b) {
        return a->getTotalRevenue() > b->getTotalRevenue();
    });

    size_t limit = std::min(topN, flights.size());
    std::cout << std::left
              << std::setw(6)  << "Rank"
              << std::setw(12) << "Flight No"
              << std::setw(16) << "Type"
              << std::setw(32) << "Route"
              << std::setw(14) << "Booked/Total"
              << "Total Revenue" << "\n";
    Utils::printDivider('-', 90);

    for (size_t i = 0; i < limit; ++i) {
        const auto& f = flights[i];
        std::string route = f->getOrigin() + " -> " + f->getDestination();
        std::string seats = std::to_string(f->getBookedSeats()) + "/" + std::to_string(f->getTotalSeats());
        std::cout << std::left
                  << std::setw(6)  << (i + 1)
                  << std::setw(12) << f->getFlightNumber()
                  << std::setw(16) << f->getFlightType()
                  << std::setw(32) << route
                  << std::setw(14) << seats
                  << "$" << std::fixed << std::setprecision(2) << f->getTotalRevenue() << "\n";
    }
}

void Airline::reportFinancialSummary() const {
    Utils::printHeader("AIRLINE EXECUTIVE FINANCIAL SUMMARY");
    double totalRevenue = 0.0;
    int totalSeatsAll = 0;
    int totalBookedAll = 0;

    for (const auto& f : flightRepo.getAll()) {
        totalRevenue += f->getTotalRevenue();
        totalSeatsAll += f->getTotalSeats();
        totalBookedAll += f->getBookedSeats();
    }

    double overallOccupancy = (totalSeatsAll > 0) ? (static_cast<double>(totalBookedAll) / totalSeatsAll) * 100.0 : 0.0;

    std::cout << "  * Airline Fleet Routes      : " << flightRepo.count() << " Active Flights\n";
    std::cout << "  * Registered Passengers     : " << passengerRepo.count() << " Passengers\n";
    std::cout << "  * Total Issued Tickets      : " << ticketRepo.count() << " Bookings\n";
    std::cout << "  * Fleet Capacity Utilization: " << totalBookedAll << " / " << totalSeatsAll
              << " (" << std::fixed << std::setprecision(2) << overallOccupancy << "%)\n";
    std::cout << "  * Gross Operating Revenue   : $" << std::fixed << std::setprecision(2) << totalRevenue << "\n";
    Utils::printDivider('=', 75);
}

// ----------------------------------------------------------------------------
// 6. PERSISTENCE (FILE I/O)
// ----------------------------------------------------------------------------

bool Airline::saveToFile(const std::string& flightFile,
                         const std::string& passengerFile,
                         const std::string& ticketFile) const {
    // 1. Save Flights
    std::ofstream fOut(flightFile);
    if (!fOut.is_open()) {
        throw FileIOException(flightFile, "Failed to open flights file for saving.");
    }
    for (const auto& f : flightRepo.getAll()) {
        fOut << f->serialize() << "\n";
    }
    fOut.close();

    // 2. Save Passengers
    std::ofstream pOut(passengerFile);
    if (!pOut.is_open()) {
        throw FileIOException(passengerFile, "Failed to open passengers file for saving.");
    }
    for (const auto& p : passengerRepo.getAll()) {
        pOut << p->serialize() << "\n";
    }
    pOut.close();

    // 3. Save Tickets
    std::ofstream tOut(ticketFile);
    if (!tOut.is_open()) {
        throw FileIOException(ticketFile, "Failed to open tickets file for saving.");
    }
    for (const auto& t : ticketRepo.getAll()) {
        tOut << t->serialize() << "\n";
    }
    tOut.close();

    return true;
}

bool Airline::loadFromFile(const std::string& flightFile,
                           const std::string& passengerFile,
                           const std::string& ticketFile) {
    flightRepo.clear();
    passengerRepo.clear();
    ticketRepo.clear();
    passengerBookingIndex.clear();

    // 1. Load Flights
    std::ifstream fIn(flightFile);
    if (fIn.is_open()) {
        std::string line;
        while (std::getline(fIn, line)) {
            line = Utils::trim(line);
            if (line.empty() || line[0] == '#') continue;
            auto tokens = Utils::split(line, '|');
            try {
                if (tokens.size() >= 10) {
                    std::string type = tokens[0];
                    std::string fNum = tokens[1];
                    std::string orig = tokens[2];
                    std::string dest = tokens[3];
                    std::string depTime = tokens[4];
                    int totSeats = std::stoi(tokens[5]);
                    int availSeats = std::stoi(tokens[6]);
                    double baseRate = std::stod(tokens[7]);
                    double revenue = std::stod(tokens[8]);

                    std::shared_ptr<Flight> flt;
                    if (type == "DOMESTIC" && tokens.size() >= 12) {
                        double tax = std::stod(tokens[9]);
                        bool sub = (std::stoi(tokens[10]) != 0);
                        double lugg = std::stod(tokens[11]);
                        flt = std::make_shared<DomesticFlight>(fNum, orig, dest, depTime, totSeats, baseRate, tax, sub, lugg);
                    } else if (type == "INTERNATIONAL" && tokens.size() >= 12) {
                        bool visa = (std::stoi(tokens[9]) != 0);
                        double customs = std::stod(tokens[10]);
                        int validMonths = std::stoi(tokens[11]);
                        flt = std::make_shared<InternationalFlight>(fNum, orig, dest, depTime, totSeats, baseRate, visa, customs, validMonths);
                    } else if (type == "CHARTER" && tokens.size() >= 13) {
                        std::string holder = tokens[9];
                        bool catering = (std::stoi(tokens[10]) != 0);
                        double hrRate = std::stod(tokens[11]);
                        int hrs = std::stoi(tokens[12]);
                        flt = std::make_shared<CharterFlight>(fNum, orig, dest, depTime, totSeats, baseRate, holder, catering, hrRate, hrs);
                    }

                    if (flt) {
                        // Restore available seats and revenue
                        while (flt->getAvailableSeats() > availSeats) {
                            flt->bookSeat();
                        }
                        flt->addRevenue(revenue);
                        flightRepo.add(flt);
                    }
                }
            } catch (...) {
                // Ignore malformed line gracefully
            }
        }
        fIn.close();
    }

    // 2. Load Passengers
    std::ifstream pIn(passengerFile);
    if (pIn.is_open()) {
        std::string line;
        while (std::getline(pIn, line)) {
            line = Utils::trim(line);
            if (line.empty() || line[0] == '#') continue;
            auto tokens = Utils::split(line, '|');
            try {
                if (tokens.size() >= 7) {
                    std::string type = tokens[0];
                    std::string pId = tokens[1];
                    std::string name = tokens[2];
                    std::string passport = tokens[3];
                    std::string email = tokens[4];
                    std::string phone = tokens[5];
                    int points = std::stoi(tokens[6]);

                    std::shared_ptr<Passenger> psg;
                    if (type == "ECONOMY" && tokens.size() >= 8) {
                        bool vegan = (std::stoi(tokens[7]) != 0);
                        psg = std::make_shared<EconomyPassenger>(pId, name, passport, email, phone, points, vegan);
                    } else if (type == "BUSINESS" && tokens.size() >= 9) {
                        bool lounge = (std::stoi(tokens[7]) != 0);
                        std::string company = tokens[8];
                        psg = std::make_shared<BusinessPassenger>(pId, name, passport, email, phone, points, lounge, company);
                    } else if (type == "FIRSTCLASS" && tokens.size() >= 9) {
                        bool chauffeur = (std::stoi(tokens[7]) != 0);
                        std::string suite = tokens[8];
                        psg = std::make_shared<FirstClassPassenger>(pId, name, passport, email, phone, points, chauffeur, suite);
                    }

                    if (psg) {
                        passengerRepo.add(psg);
                    }
                }
            } catch (...) {
                // Ignore malformed line gracefully
            }
        }
        pIn.close();
    }

    // 3. Load Tickets
    std::ifstream tIn(ticketFile);
    int maxTicketNum = 1000;
    if (tIn.is_open()) {
        std::string line;
        while (std::getline(tIn, line)) {
            line = Utils::trim(line);
            if (line.empty() || line[0] == '#') continue;
            auto tokens = Utils::split(line, '|');
            try {
                if (tokens.size() >= 9 && tokens[0] == "TICKET") {
                    std::string tId = tokens[1];
                    std::string pId = tokens[2];
                    std::string fNum = tokens[3];
                    std::string seat = tokens[4];
                    double fare = std::stod(tokens[5]);
                    std::string status = tokens[6];
                    std::string date = tokens[7];
                    double refund = std::stod(tokens[8]);

                    auto tkt = std::make_shared<Ticket>(tId, pId, fNum, seat, fare, date, status, refund);
                    ticketRepo.add(tkt);
                    passengerBookingIndex[pId].push_back(tId);

                    // Track highest ticket ID for generator
                    if (tId.find("TKT-") == 0) {
                        int num = std::stoi(tId.substr(4));
                        if (num > maxTicketNum) maxTicketNum = num;
                    }
                }
            } catch (...) {
                // Ignore malformed line
            }
        }
        tIn.close();
    }
    nextTicketCounter = maxTicketNum + 1;

    // If repositories are empty, populate default sample data
    if (flightRepo.count() == 0 || passengerRepo.count() == 0) {
        populateSampleData();
    }

    updateBookingIndex();
    return true;
}

void Airline::populateSampleData() {
    flightRepo.clear();
    passengerRepo.clear();
    ticketRepo.clear();
    passengerBookingIndex.clear();

    // 10+ Initial Flights (Domestic, International, Charter)
    // 1. Domestic Flights
    addFlight(std::make_shared<DomesticFlight>("SK-101", "New York", "Chicago", "2026-10-15 08:00", 50, 180.0, 0.05, false, 15.0));
    addFlight(std::make_shared<DomesticFlight>("SK-102", "Los Angeles", "San Francisco", "2026-10-15 11:30", 40, 120.0, 0.04, true, 10.0));
    addFlight(std::make_shared<DomesticFlight>("SK-103", "Dallas", "Miami", "2026-10-16 14:15", 60, 210.0, 0.06, false, 20.0));
    addFlight(std::make_shared<DomesticFlight>("SK-104", "Seattle", "Denver", "2026-10-16 17:45", 45, 160.0, 0.05, true, 12.0));

    // 2. International Flights
    addFlight(std::make_shared<InternationalFlight>("SK-201", "New York", "London", "2026-10-18 20:00", 80, 650.0, true, 45.0, 6));
    addFlight(std::make_shared<InternationalFlight>("SK-202", "Los Angeles", "Tokyo", "2026-10-19 13:00", 90, 850.0, true, 50.0, 6));
    addFlight(std::make_shared<InternationalFlight>("SK-203", "Chicago", "Frankfurt", "2026-10-20 18:30", 75, 720.0, false, 40.0, 6));
    addFlight(std::make_shared<InternationalFlight>("SK-204", "Miami", "Sao Paulo", "2026-10-21 22:15", 70, 580.0, true, 35.0, 6));

    // 3. Charter Flights
    addFlight(std::make_shared<CharterFlight>("SK-301", "Las Vegas", "Aspen", "2026-10-22 09:00", 12, 400.0, "Apex Tech Summit", true, 1500.0, 2));
    addFlight(std::make_shared<CharterFlight>("SK-302", "Boston", "Nantucket", "2026-10-23 15:30", 8, 300.0, "Northeast Executives", true, 1200.0, 1));
    addFlight(std::make_shared<CharterFlight>("SK-303", "Houston", "Cancun", "2026-10-24 10:00", 16, 500.0, "Global Oil Partners", true, 1800.0, 3));

    // 8+ Initial Passengers (Economy, Business, First Class)
    registerPassenger(std::make_shared<EconomyPassenger>("P-101", "Alice Johnson", "US984210", "alice.j@example.com", "+1-555-0101", 120, true));
    registerPassenger(std::make_shared<EconomyPassenger>("P-102", "Bob Smith", "GB441029", "bob.smith@example.com", "+44-20-7946-0102", 50, false));
    registerPassenger(std::make_shared<EconomyPassenger>("P-103", "Carlos Mendes", "BR781033", "carlos.m@example.com", "+55-11-9876-0103", 200, false));

    registerPassenger(std::make_shared<BusinessPassenger>("P-201", "Diana Prince", "US459021", "diana.p@enterprise.com", "+1-555-0201", 600, true, "Wayne Enterprises"));
    registerPassenger(std::make_shared<BusinessPassenger>("P-202", "Evan Wright", "CA883012", "evan.w@techcorp.ca", "+1-416-555-0202", 450, true, "TechCorp Canada"));
    registerPassenger(std::make_shared<BusinessPassenger>("P-203", "Fiona Gallagher", "IE667104", "fiona.g@global.ie", "+353-1-496-0203", 800, true, "Dublin Global"));

    registerPassenger(std::make_shared<FirstClassPassenger>("P-301", "George Harrison", "UK112944", "george.h@royalclub.uk", "+44-20-7946-0301", 1500, true, "Presidential Suite"));
    registerPassenger(std::make_shared<FirstClassPassenger>("P-302", "Helena Bonham", "FR339018", "helena.b@luxury.fr", "+33-1-4268-0302", 2200, true, "Empress Imperial Suite"));

    // Pre-book a few tickets for realistic demonstration
    bookFlight("P-101", "SK-101", "14A");
    bookFlight("P-201", "SK-201", "02B");
    bookFlight("P-301", "SK-201", "01A");
    bookFlight("P-102", "SK-102", "18C");
    bookFlight("P-202", "SK-301", "04A");
    bookFlight("P-302", "SK-202", "01B");
}
