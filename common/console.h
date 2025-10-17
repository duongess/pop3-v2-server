#pragma once
#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <sstream>
#include <utility>
#include "../types/color.h"


class Console {
private:
    static string currentTime() {
        using namespace chrono;
        auto now = system_clock::now();
        time_t t = system_clock::to_time_t(now);
        tm tm;
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        ostringstream oss;
        oss << put_time(&tm, "%H:%M:%S");
        return oss.str();
    }

public:
    ~Console() {
        reset();
    }

    static void reset() {
        cout << COLOR::RESET;
    }

    template<typename... Args>
    static void log(Args&&... args) {
        print(COLOR::WHITE, forward<Args>(args)...);
    }

    template<typename... Args>
    static void log(char* color, Args&&... args) {
        print(color, forward<Args>(args)...);
    }

    template<typename... Args>
    static void info(Args&&... args) {
        println(COLOR::BLUE, "INFO", forward<Args>(args)...);
    }

    template<typename... Args>
    static void success(Args&&... args) {
        println(COLOR::GREEN, "SUCCESS", forward<Args>(args)...);
    }

    template<typename... Args>
    static void running(Args&&... args) {
        println(COLOR::BRIGHT_BLACK, "RUNNING", forward<Args>(args)...);
    }
    
    template<typename... Args>
    static void stopping(Args&&... args) {
        println(COLOR::BRIGHT_BLACK, "STOPPING", forward<Args>(args)...);
    }

    template<typename... Args>
    static void warn(Args&&... args) {
        println(COLOR::YELLOW, "WARN", forward<Args>(args)...);
    }

    template<typename... Args>
    static void error(Args&&... args) {
        println(COLOR::RED, "ERROR", forward<Args>(args)...);
    }
    template<typename... Args>
    static void debug(Args&&... args) {
        running("[DEBUG] ", forward<Args>(args)...);
    }
private:
    template<typename... Args>
    static void println(const char* color, const string& tag, Args&&... args) {
        ostringstream msg;
        (msg << ... << args); // fold expression (C++17)
        cout << color
                  << "[" << currentTime() << "] "
                  << "[" << tag << "] "
                  << msg.str()
                  << COLOR::RESET << endl;
    }

    template<typename... Args>
    static void print(const char* color, Args&&... args) {
        ostringstream msg;
        (msg << ... << args); // fold expression (C++17)
        cout << color
                  << msg.str()
                  << COLOR::RESET;
    }
};
