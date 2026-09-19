#include "../include/Utils.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <limits>

std::string Utils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

std::string Utils::toUpper(const std::string& str) {
    std::string res = str;
    std::transform(res.begin(), res.end(), res.begin(), ::toupper);
    return res;
}

std::vector<std::string> Utils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

std::string Utils::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    if (!std::getline(std::cin, input)) {
        // Handle EOF gracefully
        return "";
    }
    input = trim(input);
    return input;
}

int Utils::readInt(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) {
            return minVal; // Graceful return on EOF
        }
        input = trim(input);
        try {
            if (input.empty()) {
                throw InvalidInputException("Input cannot be empty. Please enter an integer.");
            }
            // Check if every char is digit or optional leading - / +
            size_t startIdx = (input[0] == '-' || input[0] == '+') ? 1 : 0;
            if (startIdx == 1 && input.length() == 1) {
                throw InvalidInputException("Invalid numeric value.");
            }
            for (size_t i = startIdx; i < input.length(); ++i) {
                if (!isdigit(input[i])) {
                    throw InvalidInputException("Expected numeric digits only.");
                }
            }
            int val = std::stoi(input);
            if (val < minVal || val > maxVal) {
                std::cout << "[!] Error: Value must be between " << minVal << " and " << maxVal << ".\n";
                continue;
            }
            return val;
        } catch (const std::out_of_range&) {
            std::cout << "[!] Error: Number out of acceptable range.\n";
        } catch (const AirlineException& e) {
            std::cout << "[!] " << e.what() << "\n";
        } catch (...) {
            std::cout << "[!] Invalid input. Try again.\n";
        }
    }
}

double Utils::readDouble(const std::string& prompt, double minVal, double maxVal) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) {
            return minVal; // Graceful return on EOF
        }
        input = trim(input);
        try {
            if (input.empty()) {
                throw InvalidInputException("Input cannot be empty. Please enter a number.");
            }
            size_t idx = 0;
            double val = std::stod(input, &idx);
            if (idx != input.length()) {
                throw InvalidInputException("Extra characters detected in number.");
            }
            if (val < minVal || val > maxVal) {
                std::cout << "[!] Error: Value must be between " << minVal << " and " << maxVal << ".\n";
                continue;
            }
            return val;
        } catch (const AirlineException& e) {
            std::cout << "[!] " << e.what() << "\n";
        } catch (...) {
            std::cout << "[!] Invalid decimal value. Please enter a valid number.\n";
        }
    }
}

bool Utils::readBool(const std::string& prompt) {
    while (true) {
        std::cout << prompt << " (y/n): ";
        std::string input;
        if (!std::getline(std::cin, input)) {
            return false;
        }
        input = toUpper(trim(input));
        if (input == "Y" || input == "YES" || input == "1" || input == "TRUE") return true;
        if (input == "N" || input == "NO" || input == "0" || input == "FALSE") return false;
        std::cout << "[!] Please enter 'y' for Yes or 'n' for No.\n";
    }
}

bool Utils::isValidDate(const std::string& dateStr) {
    // Expected format YYYY-MM-DD or YYYY-MM-DD HH:MM
    if (dateStr.length() < 10) return false;
    if (dateStr[4] != '-' || dateStr[7] != '-') return false;
    try {
        int y = std::stoi(dateStr.substr(0, 4));
        int m = std::stoi(dateStr.substr(5, 2));
        int d = std::stoi(dateStr.substr(8, 2));
        if (y < 2020 || y > 2100) return false;
        if (m < 1 || m > 12) return false;
        if (d < 1 || d > 31) return false;
        return true;
    } catch (...) {
        return false;
    }
}

std::string Utils::getCurrentDate() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    char buf[30];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", now);
    return std::string(buf);
}

int Utils::calculateDaysDifference(const std::string& fromDate, const std::string& toDate) {
    // Simple estimation from YYYY-MM-DD
    try {
        if (fromDate.length() < 10 || toDate.length() < 10) return 0;
        int y1 = std::stoi(fromDate.substr(0, 4));
        int m1 = std::stoi(fromDate.substr(5, 2));
        int d1 = std::stoi(fromDate.substr(8, 2));

        int y2 = std::stoi(toDate.substr(0, 4));
        int m2 = std::stoi(toDate.substr(5, 2));
        int d2 = std::stoi(toDate.substr(8, 2));

        std::tm tm1 = {};
        tm1.tm_year = y1 - 1900;
        tm1.tm_mon = m1 - 1;
        tm1.tm_mday = d1;

        std::tm tm2 = {};
        tm2.tm_year = y2 - 1900;
        tm2.tm_mon = m2 - 1;
        tm2.tm_mday = d2;

        std::time_t t1 = std::mktime(&tm1);
        std::time_t t2 = std::mktime(&tm2);
        if (t1 == -1 || t2 == -1) return 0;
        double diff = std::difftime(t2, t1) / (60 * 60 * 24);
        return static_cast<int>(diff);
    } catch (...) {
        return 0;
    }
}

void Utils::printHeader(const std::string& title) {
    std::cout << "\n";
    printDivider('=', 75);
    std::cout << "  " << title << "\n";
    printDivider('=', 75);
}

void Utils::printDivider(char ch, int length) {
    std::cout << std::string(length, ch) << "\n";
}

void Utils::pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

void Utils::clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}
