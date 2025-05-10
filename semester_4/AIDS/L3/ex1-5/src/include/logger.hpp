#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

class Logger {
public:
    // Static variable to enable or disable logging
    static bool enabled;

    static void logArray(const std::vector<int>& arr, const std::string& label, int maxSize = 30) {
        if (!enabled || arr.size() > maxSize) return;
        std::cout << label << ": ";
        for (int x : arr) {
            std::cout << std::setw(2) << x << " ";
        }
        std::cout << "\n\n";
    }

    template <typename Iterator>
    static void logArray(Iterator begin, Iterator end, const std::string& label, int maxSize = 30) {
        if (!enabled || std::distance(begin, end) > maxSize) return;
        std::cout << label << ": ";
        for (Iterator it = begin; it != end; ++it) {
            std::cout << std::setw(2) << *it << " ";
        }
        std::cout << "\n\n";
    }

    static void logMessage(const std::string& message) {
        if (!enabled) return;
        std::cout << message << "\n";
    }
};

// Initialize the static variable
bool Logger::enabled = true;

#endif // LOGGER_HPP