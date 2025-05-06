#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include "MyTime.h"
#include "ClubState.h"

enum EventType {
    ARRIVE = 1,
    SIT = 2,
    WAIT = 3,
    LEAVE = 4,
    LEAVE_AUTO = 11,
    SIT_AUTO = 12,
    ERROR = 13
};

bool is_valid_client_name(const std::string& name) {
    static const std::regex pattern("^[a-z0-9_-]+$");
    return std::regex_match(name, pattern);
}
bool is_valid_time_format(const std::string& s) {
    static const std::regex time_regex("^(?:[01][0-9]|2[0-3]):[0-5][0-9]$");
    return std::regex_match(s, time_regex);
}

bool has_only_single_spaces(const std::string& s) {
    static const std::regex pattern("^[^ ]+( [^ ]+)*$");
    return std::regex_match(s, pattern);
}

Event parse_event(const std::string& line, int line_number, int num_tables, Time& prev_time) {
    std::istringstream iss(line);
    std::string time_str;
    int id;
    std::string name;
    int table = -1;
    if (!(iss >> time_str >> id >> name)) print_error_and_exit(line_number);
    if (!is_valid_time_format(time_str)) {
        print_error_and_exit(line_number);
    }
    Time t = Time::from_string(time_str);
    if (!is_valid_client_name(name) || id < 1 || id > 4) {
        print_error_and_exit(line_number);
    }
    if (t < prev_time) {
        print_error_and_exit(line_number);
    }
    prev_time = t;
    if (id == 2) {
        iss >> table;
    }
    return Event{ t, id, name, table };
}



int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    std::string input_file = argv[1];
    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << input_file << std::endl;
        return 1;
    }
    int num_tables;
    int line_number = 0;
    std::string line;
    // Чтение количества столов
    if (!std::getline(file, line)) print_error_and_exit(++line_number);
    try {
        num_tables = std::stoi(line);
        if (num_tables <= 0) {
            print_error_and_exit(line_number);
        }
    }
    catch (...) {
        print_error_and_exit(line_number);
    }
    // Чтение второй строки: время начала и окончания работы
    std::string open_time_str, close_time_str;
    std::getline(file, open_time_str);
    std::istringstream time_stream(open_time_str);
    time_stream >> open_time_str >> close_time_str;
    if (!is_valid_time_format(open_time_str) || !is_valid_time_format(close_time_str)) {
        print_error_and_exit(1);
    }
    Time open_time = Time::from_string(open_time_str);
    Time close_time = Time::from_string(close_time_str);
    // Чтение стоимости часа
    int price;
    line_number = 2;
    if (!std::getline(file, line)) print_error_and_exit(line_number);
    try {
        price = std::stoi(line);
        if (price <= 0) {
            print_error_and_exit(line_number);
        }
    }
    catch (...) {
        print_error_and_exit(line_number);
    }
    std::vector<Table> tables(num_tables);
    std::vector<Event> events;
    ClubState club_state(num_tables, open_time, close_time, price);
    line_number = 2;
    Time prev(00, 00);
    while (std::getline(file, line)) {
        if (!line.empty())
        {
            if (!has_only_single_spaces(line)) {
                print_error_and_exit(++line_number);
            }
            events.push_back(parse_event(line, ++line_number, num_tables,prev));
        }
    }
    // Обработка событий
    club_state.process_events(events);
    return 0;
}
