#include <string>
#include <iostream>
#include <sqlite\sqlite3.h>
#include "Include\ExtendMagazine.h"

// Implementation of loadFromDB, saveToDB, and display methods...

bool ExtendMagazine::loadFromDB(sqlite3* db) {
    std::string query = "SELECT ISSN, Volume, Issue, Publication_date, Special_edition FROM EXTEND_MAGAZINE WHERE EISSN = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, EISSN.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cerr << "Failed to bind EISSN: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        ISSN = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        volume = sqlite3_column_int(stmt, 1);
        issue = sqlite3_column_int(stmt, 2);
        publication_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        special_edition = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    } else {
        std::cerr << "No record found for EISSN: " << EISSN << std::endl;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool ExtendMagazine::saveToDB(sqlite3* db) {
    std::string query = "INSERT OR REPLACE INTO EXTEND_MAGAZINE (EISSN, ISSN, Volume, Issue, Publication_date, Special_edition) "
                        "VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, EISSN.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, ISSN.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, volume);
    sqlite3_bind_int(stmt, 4, issue);
    sqlite3_bind_text(stmt, 5, publication_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, special_edition.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to save to EXTEND_MAGAZINE: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

void ExtendMagazine::display() const {
    std::cout << "EISSN: " << EISSN << "\nISSN: " << ISSN
              << "\nVolume: " << volume << "\nIssue: " << issue
              << "\nPublication Date: " << publication_date
              << "\nSpecial Edition: " << special_edition << std::endl;
}
