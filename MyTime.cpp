#include "MyTime.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
Time::Time() : hours(0), minutes(0) {}

Time::Time(int h, int m) : hours(h), minutes(m) {}

Time Time::from_string(const std::string& s) {
    int h, m;
    char sep;
    std::istringstream iss(s);
    iss >> h >> sep >> m;
    return Time(h, m);
}

std::string Time::to_string() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;
    return oss.str();
}

int Time::to_minutes() const {
    return hours * 60 + minutes;
}

void Time::add_minutes(int m) {
    int total = to_minutes() + m;
    hours = total / 60;
    minutes = total % 60;
}

bool Time::operator<(const Time& other) const {
    return to_minutes() < other.to_minutes();
}

bool Time::operator==(const Time& other) const {
    return to_minutes() == other.to_minutes();
}

bool Time::operator<=(const Time& other) const {
    return to_minutes() <= other.to_minutes();
}

Time Time::operator-(const Time& other) const {
    int diff = this->to_minutes() - other.to_minutes();
    return Time(diff / 60, diff % 60);
}