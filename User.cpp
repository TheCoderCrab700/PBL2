#include "Include\User.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <ncurses/ncurses.h>

// ------------------------ UserAccount Class ------------------------

// Constructor
UserAccount::UserAccount(int id, const string& first_name, const string& last_name,
                         const string& user_name, const string& password, const string& email)
    : id(id), first_name(first_name), last_name(last_name), user_name(user_name), password(password), email(email) {}

// Getters
int UserAccount::getID() const { return id; }
string UserAccount::getFirstName() const { return first_name; }
string UserAccount::getLastName() const { return last_name; }
string UserAccount::getEmail() const { return email; }
string UserAccount::getUserName() const { return user_name; }
string UserAccount::getPassword() const { return password; }

// Setters
void UserAccount::Update_FirstName(const string& new_name) { first_name = new_name; }
void UserAccount::Update_LastName(const string& new_name) { last_name = new_name; }
void UserAccount::Update_Email(const string& new_email) { email = new_email; }
void UserAccount::Update_User_Name(const string& new_user_name) { user_name = new_user_name; }
void UserAccount::Update_Password(const string& new_password) { password = new_password; }

// Display Method
void UserAccount::display() const {
    printw("User ID: %d\n", id);
    printw("Name   : %s %s\n", first_name.c_str(), last_name.c_str());
    printw("Email  : %s\n", email.c_str());
    printw("Username: %s\n", user_name.c_str());
}

// Save to Database
bool UserAccount::saveToDatabase(sqlite3* db) {
    try {
        string sql = "INSERT INTO User_account (ID, first_name, last_name, Email, User_name, Password) "
                     "VALUES (" + to_string(id) + ", '" + first_name + "', '" + last_name + "', '" + email +
                     "', '" + user_name + "', '" + password + "');";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            throw runtime_error("SQL error: " + string(errMsg));
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception while saving user: " << e.what() << endl;
        return false;
    }
}

// Load from Database
bool UserAccount::loadFromDatabase(sqlite3* db, int user_id) {
    try {
        sqlite3_stmt* stmt;
        string sql = "SELECT first_name, last_name, Email, User_name, Password FROM User_account WHERE ID = " + to_string(user_id) + ";";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare SQL statement.");
        }

        id = user_id;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            first_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            last_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            user_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        } else {
            sqlite3_finalize(stmt);
            throw runtime_error("No record found with ID: " + to_string(user_id));
        }

        sqlite3_finalize(stmt);
        return true;
    } catch (const exception& e) {
        cerr << "Exception while loading user: " << e.what() << endl;
        return false;
    }
}

// Update in Database
bool UserAccount::updateToDatabase(sqlite3* db) {
    try {
        string sql = "UPDATE User_account SET first_name = '" + first_name + "', last_name = '" + last_name + 
                     "', Email = '" + email + "', User_name = '" + user_name + "', Password = '" + password + 
                     "' WHERE ID = " + to_string(id) + ";";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            throw runtime_error("SQL error: " + string(errMsg));
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception while updating user: " << e.what() << endl;
        return false;
    }
}

// ------------------------ Admin Class ------------------------

Admin::Admin(int id, const string& first_name, const string& last_name, const string& user_name, const string& password, const string& email)
    : UserAccount(id, first_name, last_name, user_name, password, email) {}

string Admin::getRole(){
    string role = "Administrator";
    return role;
}
void Admin::display() const {
    UserAccount::display();
    printw("Role   : Administrator\n");
}

bool Admin::saveToDatabase(sqlite3* db) {
    if (!UserAccount::saveToDatabase(db)) return false;
    try {
        string sql = "UPDATE User_account SET Role = 'Administrator' WHERE ID = " + to_string(id) + ";";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            throw runtime_error("SQL error: " + string(errMsg));
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception while setting role: " << e.what() << endl;
        return false;
    }
}

bool Admin::Update_role(sqlite3* db, int user_id, const string& new_role) {
    try {
        string sql = "UPDATE User_account SET Role = '" + new_role + "' WHERE ID = " + to_string(user_id) + ";";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            throw runtime_error("SQL error: " + string(errMsg));
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception while updating role: " << e.what() << endl;
        return false;
    }
}

bool Admin::Delete_account(sqlite3* db, int user_id) {
    try {
        string sql = "DELETE FROM User_account WHERE ID = " + to_string(user_id) + ";";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            throw runtime_error("SQL error: " + string(errMsg));
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception while deleting account: " << e.what() << endl;
        return false;
    }
}

// ------------------------ Assistant Class ------------------------

Assistant::Assistant(int id, const string& first_name, const string& last_name, const string& user_name, const string& password, const string& email)
    : UserAccount(id, first_name, last_name, user_name, password, email) {}

string Assistant::getRole(){
    string role = "Assistant";
    return role;
}
void Assistant::display() const {
    UserAccount::display();
    printw("Role   : Assistant\n");
}

bool Assistant::saveToDatabase(sqlite3* db) {
    if (!UserAccount::saveToDatabase(db)) return false;

    try {
        string sql = "UPDATE User_account SET Role = 'Assistant' WHERE ID = " + to_string(id) + ";";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            throw runtime_error("SQL error: " + string(errMsg));
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception while setting role: " << e.what() << endl;
        return false;
    }
}

// Add a new book to the database
bool Assistant::Add_book(sqlite3* db, Book& book) {
    try {
        if (!book.saveToDatabase(db)) {
            throw runtime_error("Failed to save book to the database.");
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception while adding book: " << e.what() << endl;
        return false;
    }
}

// Edit an existing book in the database
bool Assistant::Edit_book(sqlite3* db, Book& book) {
    try {
        if (!book.saveToDatabase(db)) {
            throw runtime_error("Failed to update book in the database.");
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception while editing book: " << e.what() << endl;
        return false;
    }
}

// ------------------------ Librarian Class ------------------------

Librarian::Librarian(int id, const string& first_name, const string& last_name, const string& user_name, const string& password, const string& email)
    : UserAccount(id, first_name, last_name, user_name, password, email) {}

string Librarian::getRole(){
    string role = "Librarian";
    return role;
}
void Librarian::display() const {
    UserAccount::display();
    printw("Role   : Librarian\n");
}

bool Librarian::saveToDatabase(sqlite3* db) {
    if (!UserAccount::saveToDatabase(db)) return false;

    try {
        string sql = "UPDATE User_account SET Role = 'Librarian' WHERE ID = " + to_string(id) + ";";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            throw runtime_error("SQL error: " + string(errMsg));
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception while setting role: " << e.what() << endl;
        return false;
    }
}

// Lend a book (CheckOut process)
void Librarian::lendbook(sqlite3* db, CheckOut& Check) {
    try {
        if (!Check.saveToDB(db)) {
            throw runtime_error("Failed to save check-out information to the database.");
        }

        printw("Book successfully lent to the user.\n");



    } catch (const exception& e) {
        cerr << "Exception while lending book: " << e.what() << endl;
    }
}
