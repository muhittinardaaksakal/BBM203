#include "Leaderboard.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>

void Leaderboard::read_from_file(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    unsigned long score;
    time_t timestamp;
    string player_name;

    while (file >> score >> timestamp >> player_name) {
        LeaderboardEntry *new_entry = new LeaderboardEntry(score, timestamp, player_name);
        insert(new_entry);
    }

    file.close();
}

void Leaderboard::write_to_file(const string &filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        file << current->score << " " << current->last_played << " " << current->player_name << endl;
        current = current->next;
    }

    file.close();
}

void Leaderboard::print_leaderboard() {
    cout << "Leaderboard\n----------" << endl;
    int rank = 1;
    LeaderboardEntry *current = head_leaderboard_entry;

    while (current) {
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%H:%M:%S/%d.%m.%Y", localtime(&current->last_played));
        cout << rank << "." << current->player_name << current->score << " " << timestamp << endl;
        current = current->next;
        rank++;
    }
}

void Leaderboard::insert(LeaderboardEntry *new_entry) {
    // If the leaderboard is empty, add the new entry as the first element
    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *current = head_leaderboard_entry;
        while (current->next && current->next->score >= new_entry->score) {
            current = current->next;
        }
        new_entry->next = current->next;
        current->next = new_entry;
    }

    // Remove the last entry if there are more than 10 entries
    int count = 0;
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        count++;
        current = current->next;
    }

    if (count > MAX_LEADERBOARD_SIZE) {
        current = head_leaderboard_entry;
        while (current->next && current->next->next) {
            current = current->next;
        }
        delete current->next;
        current->next = nullptr;
    }
}

Leaderboard::~Leaderboard() {
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        LeaderboardEntry *next = current->next;
        delete current;
        current = next;
    }
}
