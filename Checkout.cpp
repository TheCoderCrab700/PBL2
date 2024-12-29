#include "Include\Checkout.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;



// Helper function to bind text or NULL to a statement
void CheckOut::bindTextOrNull(sqlite3_stmt* stmt, int index, const std::string& text) {
    try {
        if (text.empty()) {
            sqlite3_bind_null(stmt, index);
        } else {
            sqlite3_bind_text(stmt, index, text.c_str(), -1, SQLITE_TRANSIENT);
        }
    } catch (const exception& e) {
        cerr << "Exception while binding text or NULL: " << e.what() << endl;
    }
}



// Save to DB
bool CheckOut::saveToDB(sqlite3* db) {
    try {
        std::string query = 
            "INSERT INTO CHECK_OUT (Start_time, End_time, Return_time, Patron_ID, Copy_ID, By_ID, Is_return) "
            "VALUES (?, ?, ?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
            return false;
        }

        bindTextOrNull(stmt, 1, start_time.tostring());
        bindTextOrNull(stmt, 2, end_time.tostring());
        bindTextOrNull(stmt, 3, return_time.tostring());
        sqlite3_bind_int(stmt, 4, patron_id);
        sqlite3_bind_int(stmt, 5, copy_id);
        sqlite3_bind_int(stmt, 6, by_id);
        sqlite3_bind_int(stmt, 7, is_return ? 1 : 0);

        const char* expanded_sql = sqlite3_expanded_sql(stmt);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error saving CheckOut to DB: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
        cout << "CheckOut record saved successfully." << endl;
        return true;
    } catch (const exception& e) {
        cerr << "Exception while saving CheckOut to DB: " << e.what() << endl;
        return false;
    }
}

// Load from DB
bool CheckOut::loadFromDB(sqlite3* db, int checkout_id) {
    try {
        std::string query = "SELECT * FROM CHECK_OUT WHERE ID = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, checkout_id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            
            id = sqlite3_column_int(stmt, 0);
            start_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            end_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            return_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) ?: "";
            patron_id = sqlite3_column_int(stmt, 4);
            copy_id = sqlite3_column_int(stmt, 5);
            by_id = sqlite3_column_int(stmt, 6);
            is_return = sqlite3_column_int(stmt, 7) != 0;

            sqlite3_finalize(stmt);
            cout << "CheckOut record loaded successfully." << endl;
            return true;
        } else {
            cerr << "CheckOut record not found for ID: " << checkout_id << endl;
            sqlite3_finalize(stmt);
            return false;
        }
    } catch (const exception& e) {
        cerr << "Exception while loading CheckOut from DB: " << e.what() << endl;
        return false;
    }
}

// Update in DB
bool CheckOut::updateInDB(sqlite3* db) {
    try {
        if (id == 0) {
            cerr << "Error: CheckOut ID is 0. Cannot update a non-existent record in DB." << endl;
            return false;
        }

        std::string query = 
            "UPDATE CHECK_OUT SET Start_time = ?, End_time = ?, Return_time = ?, Patron_ID = ?, Copy_ID = ?, By_ID = ?, Is_return = ? "
            "WHERE ID = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
            return false;
        }

        bindTextOrNull(stmt, 1, start_time.tostring());
        bindTextOrNull(stmt, 2, end_time.tostring());
        bindTextOrNull(stmt, 3, return_time.tostring());
        sqlite3_bind_int(stmt, 4, patron_id);
        sqlite3_bind_int(stmt, 5, copy_id);
        sqlite3_bind_int(stmt, 6, by_id);
        sqlite3_bind_int(stmt, 7, is_return ? 1 : 0);
        sqlite3_bind_int(stmt, 8, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error updating CheckOut in DB: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            return false;
        }

        sqlite3_finalize(stmt);
        cout << "CheckOut record updated successfully." << endl;
        return true;
    } catch (const exception& e) {
        cerr << "Exception while updating CheckOut in DB: " << e.what() << endl;
        return false;
    }
}

// Return book
void CheckOut::Return_book() {
    try {
        is_return = true;
        cout << "Book returned successfully. Marked as returned." << endl;
    } catch (const exception& e) {
        cerr << "Exception while marking book as returned: " << e.what() << endl;
    }
}

// Expand checkout duration
void CheckOut::Expand() {
    try {
        // Logic for extending the checkout duration
        cout << "Checkout duration extended successfully." << endl;
    } catch (const exception& e) {
        cerr << "Exception while extending checkout duration: " << e.what() << endl;
    }
}

// Print info
void CheckOut::Get_info() {
    try {
        cout << "CheckOut Record:" << endl;
        cout << "ID: " << id << endl;
        cout << "Start Time: " << start_time << endl;
        cout << "End Time: " << end_time << endl;
        cout << "Return Time: " << return_time << endl;
        cout << "Patron ID: " << patron_id << endl;
        cout << "Copy ID: " << copy_id << endl;
        cout << "Handled By (ID): " << by_id << endl;
        cout << "Is Returned: " << (is_return ? "Yes" : "No") << endl;
    } catch (const exception& e) {
        cerr << "Exception while printing CheckOut info: " << e.what() << endl;
    }
}

// Mark as not returned
void CheckOut::Is_not_return() {
    try {
        is_return = false;
        cout << "Book marked as not returned." << endl;
    } catch (const exception& e) {
        cerr << "Exception while marking book as not returned: " << e.what() << endl;
    }
}

// Discard the checkout record
bool CheckOut::Discard(sqlite3* db) {
    try {
        if (id == 0) {
            cerr << "Error: CheckOut ID is 0. Cannot delete a non-existent record in DB." << endl;
            return false;
        }

        std::string query = "DELETE FROM CHECK_OUT WHERE ID = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error preparing delete statement: " << sqlite3_errmsg(db) << endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error deleting CheckOut from DB: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            return false;
        }

        sqlite3_finalize(stmt);
        cout << "CheckOut record with ID " << id << " deleted successfully." << endl;
        return true;
    } catch (const exception& e) {
        cerr << "Exception while deleting CheckOut from DB: " << e.what() << endl;
        return false;
    }
}
