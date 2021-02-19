#pragma once

#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>

typedef std::chrono::system_clock SystemClock;


inline std::string ToISOString(const SystemClock::time_point& tp) {
    auto epoch_seconds = SystemClock::to_time_t(tp);

    struct tm buf;
    gmtime_s(&buf, &epoch_seconds);

    std::stringstream ss;
    ss << std::put_time(&buf, "%FT%T");

    auto truncated = std::chrono::system_clock::from_time_t(epoch_seconds);
    auto delta_us = std::chrono::duration_cast<std::chrono::milliseconds>(tp - truncated).count();
    ss << "." << std::fixed << std::setw(3) << std::setfill('0') << delta_us;

    return ss.str();
}


inline SystemClock::time_point LogEvent(const std::string& msg) {
    auto tp_now = SystemClock::now();
    std::cout << ToISOString(tp_now) << " \t " << msg << std::endl;
    return tp_now;
}

inline SystemClock::time_point LogEvent(const std::string& msg, const SystemClock::time_point& tp_last) {
    auto tp_now = SystemClock::now();
    auto deltaMS = std::chrono::duration_cast<std::chrono::milliseconds>(tp_now - tp_last);
    std::cout << ToISOString(tp_now) << "(" << deltaMS.count() << "): " << " \t " << msg << std::endl;
    return tp_now;
}
