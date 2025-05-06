#include <regex>
#include "ClubState.h"
#include "MyTime.h"



void print_error_and_exit(int line_number) {
    std::cerr << "Error at line " << line_number << std::endl;
    std::exit(1);  // Завершаем программу с кодом ошибки
}

void ClubState::process_events(const std::vector<Event>& events) {
    output_log.push_back(open_time.to_string());

    for (const auto& e : events) {
        handle_event(e);
    }

    finish_day();  // автоматический уход всех клиентов
    output_log.push_back(close_time.to_string());

    for (const auto& line : output_log) {
        std::cout << line << std::endl;
    }

    print_summary();
}

void ClubState::log_event(const Time& t, int id, const std::string& msg) {
    std::ostringstream oss;
    oss << t.to_string() << " " << id << " " << msg;
    output_log.push_back(oss.str());
}

void ClubState::handle_event(const Event& e) {
    auto& name = e.client;
    Client& client = clients[name];
    output_log.push_back(e.to_string());
    // ID 1 — клиент пришел
    if (e.id == 1) {
        if (e.time < open_time) {
            log_event(e.time, 13, "NotOpenYet");
            return;
        }
        if (client.is_in_club) {
            log_event(e.time, 13, "YouShallNotPass");
            return;
        }
        client.name = name;
        client.is_in_club = true;
        client.is_waiting = false;
        client.table = -1;
    }

    // ID 2 — клиент сел за стол
    else if (e.id == 2) {
        int table_id = e.table;
        if (!client.is_in_club) {
            log_event(e.time, 13, "ClientUnknown");
            return;
        }
        if (tables[table_id].occupied && tables[table_id].current_client != name) {
            log_event(e.time, 13, "PlaceIsBusy");
            return;
        }
        // Освобождаем предыдущий стол, если клиент уже сидел
        if (client.table != -1 && client.table != table_id) {
            int prev = client.table;
            Time dur = e.time - client.start_time;
            int mins = dur.to_minutes();
            tables[prev].total_minutes += mins;
            tables[prev].total_revenue += ((mins + 59) / 60) * price;
            tables[prev].occupied = false;
            tables[prev].current_client.clear();
        }
        client.table = table_id;
        client.start_time = e.time;
        tables[table_id].occupied = true;
        tables[table_id].current_client = name;
    }

    // ID 3 — клиент встал в очередь
    else if (e.id == 3) {
        if (!client.is_in_club) {
            log_event(e.time, 13, "ClientUnknown");
            return;
        }
        bool has_free = false;
        for (size_t i = 1; i < tables.size(); ++i) {
            if (!tables[i].occupied) {
                has_free = true;
                break;
            }
        }
        if (has_free) {
            log_event(e.time, 13, "ICanWaitNoLonger!");
            return;
        }
        if (waiting_queue.size() >= tables.size() - 1) {
            log_event(e.time, 11, name);
            client.is_in_club = false;
            return;
        }
        waiting_queue.push(name);
        client.is_waiting = true;
    }

    // ID 4 — клиент ушел
    else if (e.id == 4) {
        if (!client.is_in_club) {
            log_event(e.time, 13, "ClientUnknown");
            return;
        }
        if (client.table != -1) {
            int tid = client.table;
            Time dur = e.time - client.start_time;
            int mins = dur.to_minutes();
            tables[tid].total_minutes += mins;
            tables[tid].total_revenue += ((mins + 59) / 60) * price;
            tables[tid].occupied = false;
            tables[tid].current_client.clear();
            client.table = -1;
            // Пересадим из очереди
            if (!waiting_queue.empty()) {
                std::string next = waiting_queue.front();
                waiting_queue.pop();
                clients[next].is_waiting = false;
                clients[next].table = tid;
                clients[next].start_time = e.time;
                tables[tid].occupied = true;
                tables[tid].current_client = next;
                log_event(e.time, 12, next + " " + std::to_string(tid));
            }
        }
        client.is_in_club = false;
        client.is_waiting = false;
    }
}

void ClubState::finish_day() {
    std::vector<std::string> to_leave;
    for (const auto& [name, client] : clients) {
        if (client.is_in_club) {
            to_leave.push_back(name);
        }
    }
    std::sort(to_leave.begin(), to_leave.end());
    for (const std::string& name : to_leave) {
        Client& client = clients[name];
        if (client.table != -1) {
            int tid = client.table;
            Time dur = close_time - client.start_time;
            int mins = dur.to_minutes();
            tables[tid].total_minutes += mins;
            tables[tid].total_revenue += ((mins + 59) / 60) * price;
            tables[tid].occupied = false;
            tables[tid].current_client.clear();
        }
        client.is_in_club = false;
        client.table = -1;
        client.is_waiting = false;
        log_event(close_time, 11, name);
    }
}

void ClubState::print_summary() const {
    for (size_t i = 1; i < tables.size(); ++i) {
        const Table& t = tables[i];
        Time total_time;
        total_time.add_minutes(t.total_minutes);
        std::cout << i << " " << t.total_revenue << " " << total_time.to_string() << "\n";
    }
}



