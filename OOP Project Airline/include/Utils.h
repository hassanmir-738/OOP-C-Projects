#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <iostream>

/**
 * Utility functions for input validation, date/time operations, and formatting.
 */
class Utils {
public:
    // Input helpers
    static std::string readLine(const std::string& prompt);
    static int readInt(const std::string& prompt, int minVal = 0, int maxVal = 1000000);
    static double readDouble(const std::string& prompt, double minVal = 0.0, double maxVal = 1000000.0);
    static bool readBool(const std::string& prompt);

    // String manipulation
    static std::string trim(const std::string& str);
    static std::string toUpper(const std::string& str);
    static std::vector<std::string> split(const std::string& str, char delimiter);

    // Date & Time validation (Format: YYYY-MM-DD HH:MM)
    static bool isValidDate(const std::string& dateStr);
    static std::string getCurrentDate();
    static int calculateDaysDifference(const std::string& fromDate, const std::string& toDate);

    // UI Formatting
    static void printHeader(const std::string& title);
    static void printDivider(char ch = '=', int length = 75);
    static void pauseScreen();
    static void clearScreen();
};

#endif // UTILS_H
