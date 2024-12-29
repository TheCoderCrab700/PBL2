#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <sqlite3.h>

using namespace std;

class Copy {
private:
    int id;
    string ISBN;
    string EISSN;
    string status;

    // Helper to bind text or NULL to a SQLite statement
    void bindTextOrNull(sqlite3_stmt* stmt, int index, const string& value) const {
        if (value.empty()) {
            sqlite3_bind_null(stmt, index);
        } else {
            sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_STATIC);
        }
    }

public:
    // Constructor
    Copy(int id = 0, const string& ISBN = "", const string& EISSN = "", const string& status = "")
        : id(id), ISBN(ISBN), EISSN(EISSN), status(status) {}

    // Getters
    int getID() const { return id; }
    string getISBN() const { return ISBN; }
    string getEISSN() const { return EISSN; }
    string getStatus() const { return status; }

    // Setters (Updates)
    void Update_ISBN(const string& newIsbn) { ISBN = newIsbn; }
    void Update_EISSIN(const string& newEissn) { EISSN = newEissn; }
    void Update_status(const string& newStatus) { status = newStatus; }

    // Display information
    void Get_info() const {
        cout << "Copy ID: " << id << "\n"
             << "ISBN: " << ISBN << "\n"
             << "EISSN: " << EISSN << "\n"
             << "Status: " << status << "\n";
    }

    // Save this Copy to the database
    bool saveToDB(sqlite3* db) {
        const string query = "INSERT INTO Copy (ID, ISBN, EISSN, Status) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt = nullptr;

        try {
            // Prepare the statement
            if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                throw runtime_error("Failed to prepare saveToDB statement: " + string(sqlite3_errmsg(db)));
            }

            // Bind parameters
            sqlite3_bind_int(stmt, 1, id);
            bindTextOrNull(stmt, 2, ISBN);
            bindTextOrNull(stmt, 3, EISSN);
            bindTextOrNull(stmt, 4, status);

            // Execute the query
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw runtime_error("Failed to save Copy: " + string(sqlite3_errmsg(db)));
            }

            sqlite3_finalize(stmt);
            return true;

        } catch (const exception& e) {
            cerr << "Error in saveToDB: " << e.what() << endl;
            if (stmt) sqlite3_finalize(stmt);
            return false;
        }
    }

    // Load a Copy from the database by ID
    bool loadFromDB(sqlite3* db, int copyID) {
        const string query = "SELECT ID, ISBN, EISSN, Status FROM Copy WHERE ID = ?;";
        sqlite3_stmt* stmt = nullptr;

        try {
            // Prepare the statement
            if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                throw runtime_error("Failed to prepare loadFromDB statement: " + string(sqlite3_errmsg(db)));
            }

            // Bind the ID parameter
            sqlite3_bind_int(stmt, 1, copyID);

            // Execute and retrieve the data
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                id = sqlite3_column_int(stmt, 0);
                ISBN = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) ?: "";
                EISSN = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) ?: "";
                status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) ?: "";
                sqlite3_finalize(stmt);
                return true;
            } else {
                throw runtime_error("Copy with ID " + to_string(copyID) + " not found in the database.");
            }

        } catch (const exception& e) {
            cerr << "Error in loadFromDB: " << e.what() << endl;
            if (stmt) sqlite3_finalize(stmt);
            return false;
        }
    }

    // Update the current Copy's record in the database
    bool updateInDB(sqlite3* db) {
        if (id == 0) {
            cerr << "Error: Copy ID is 0. Cannot update a non-existent record in the database." << endl;
            return false;
        }

        const string query = "UPDATE Copy SET ISBN = ?, EISSN = ?, Status = ? WHERE ID = ?;";
        sqlite3_stmt* stmt = nullptr;

        try {
            // Prepare the statement
            if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                throw runtime_error("Failed to prepare updateInDB statement: " + string(sqlite3_errmsg(db)));
            }

            // Bind parameters
            bindTextOrNull(stmt, 1, ISBN);
            bindTextOrNull(stmt, 2, EISSN);
            sqlite3_bind_text(stmt, 3, status.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 4, id);

            // Execute the query
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw runtime_error("Failed to update Copy: " + string(sqlite3_errmsg(db)));
            }

            sqlite3_finalize(stmt);
            return true;

        } catch (const exception& e) {
            cerr << "Error in updateInDB: " << e.what() << endl;
            if (stmt) sqlite3_finalize(stmt);
            return false;
        }
    }
};
