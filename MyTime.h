#pragma once
#include <string>
class Time {
public:
    int hours;
    int minutes;

    Time();
    Time(int h, int m);
    static Time from_string(const std::string& s);
    std::string to_string() const;

    int to_minutes() const;
    void add_minutes(int m);

    bool operator<(const Time& other) const;
    bool operator==(const Time& other) const;
    bool operator<=(const Time& other) const;

    Time operator-(const Time& other) const;
};
