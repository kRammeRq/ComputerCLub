#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <iomanip>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include "MyTime.h"

struct Client {
    std::string name;
    bool is_in_club = false;
    bool is_waiting = false;
    int table = -1;
    Time start_time;
};

struct Table {
    bool occupied = false;
    std::string current_client;
    int total_minutes = 0;
    int total_revenue = 0;
};

struct Event {
    Time time;
    int id;
    std::string client;
    int table = -1;

    std::string to_string() const {
        std::ostringstream oss;
        oss << time.to_string() << " " << id << " " << client;
        if (id == 2) oss << " " << table;
        return oss.str();
    }
};

//Event parse_event(const std::string& line, int line_number, int num_tables, Time& prev);

class ClubState {
public:
    ClubState(int table_count, Time open, Time close, int price_per_hour)
        : tables(table_count + 1),  // index from 1
        open_time(open), close_time(close),
        price(price_per_hour) {}

    void process_events(const std::vector<Event>& events);
    void print_summary() const;

private:
    std::map<std::string, Client> clients;
    std::queue<std::string> waiting_queue;
    std::vector<Table> tables;
    std::vector<std::string> output_log;

    Time open_time, close_time;
    int price;

    void handle_event(const Event& e);
    void log_event(const Time& t, int id, const std::string& msg);
    void generate_error(const Time& time, const std::string& message);
    void finish_day();
};

void print_error_and_exit(int line_number);
