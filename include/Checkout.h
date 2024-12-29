
#pragma once
#include <iostream>
#include <string>
#include <sqlite3.h>
#include "Datetime.h"
class CheckOut {
private:
    int id;
    DateTime start_time;
    DateTime end_time;
    DateTime return_time;
    int patron_id;
    int copy_id;
    int by_id;
    bool is_return;

public:
    // Full constructor
    CheckOut(int id = 0, int patron_id = 0, int copy_id = 0, int by_id = 0, bool is_return = 0)
        : id(id), patron_id(patron_id), copy_id(copy_id), by_id(by_id), is_return(is_return) {
            start_time = DateTime::now().tostring();
            end_time = start_time;
            end_time +=(long) (14*86400); //14 days after
            return_time = DateTime(9999);  // NULL DateTime
        }

    // Default constructor
    CheckOut() : id(0), patron_id(0), copy_id(0), by_id(0), is_return(false) {}

    // Getters
    int getId() const { return id; }
    DateTime getStartTime() const { return start_time; }
    DateTime getEndTime() const { return end_time; }
    DateTime getReturnTime() const { return return_time; }
    int getPatronId() const { return patron_id; }
    int getCopyId() const { return copy_id; }
    int getById() const { return by_id; }
    bool getIsReturn() const { return is_return; }

    // Setters
    void setId(int id) { this->id = id; }
    void setStartTime(const std::string& start_time) { this->start_time = start_time; }
    void setEndTime(const std::string& end_time) { this->end_time = end_time; }
    void setReturnTime(const std::string& return_time) { this->return_time = return_time; }
    void setPatronId(int patron_id) { this->patron_id = patron_id; }
    void setCopyId(int copy_id) { this->copy_id = copy_id; }
    void setById(int by_id) { this->by_id = by_id; }
    void setIsReturn(bool is_return) { this->is_return = is_return; }

    // Save the CheckOut record to the database
    bool saveToDB(sqlite3* db);
    // Load a CheckOut record from the database
    bool loadFromDB(sqlite3* db, int checkout_id);
    // Update the CheckOut record in the database
    bool updateInDB(sqlite3* db);

    // Methods as per your schema
    void Return_book();
    void Expand();
    void Get_info();
    void Is_not_return();
    bool Discard(sqlite3* db);
    void bindTextOrNull(sqlite3_stmt* stmt, int index, const std::string& text);
};

