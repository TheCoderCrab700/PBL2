#include "Include\InfoClasses.h"
#include <stdexcept> // For exceptions
#include <iostream>  // For logging exceptions
#include <sqlite3.h> // Include SQLite library

// Initialize static variables
int Author::authorCount = 0;
int Category::categoryCount = 0;

// ** Author Class Implementation **

Author::Author(const std::string& firstName, const std::string& lastName, int authorID) {
    if (firstName.empty() || lastName.empty() || authorID < 0) {
        throw std::invalid_argument("Author constructor: Invalid data provided!");
    }
    this->firstName = firstName;
    this->lastName = lastName;
    this->authorID = authorID;
    authorCount++; // Increment static count whenever an object is created
}

// Getters
std::string Author::getFirstName() const { return firstName; }
std::string Author::getLastName() const { return lastName; }
int Author::getID() const { return authorID; }

int Author::getNumBooks() const {
    sqlite3* db;
    sqlite3_open("Data/Database.db", &db);
    std::string sql = "SELECT COUNT(*) FROM Book_author WHERE Author_ID = " + std::to_string(authorID);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    int numBooks = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        numBooks = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return numBooks;
}

// Setters
void Author::setFirstName(const std::string& firstName) {
    if (firstName.empty()) throw std::invalid_argument("setFirstName: First name cannot be empty!");
    this->firstName = firstName;
}

void Author::setLastName(const std::string& lastName) {
    if (lastName.empty()) throw std::invalid_argument("setLastName: Last name cannot be empty!");
    this->lastName = lastName;
}

void Author::setID(int authorID) {
    if (authorID <= 0) throw std::invalid_argument("setAuthorID: Author ID must be positive!");
    this->authorID = authorID;
}

// Static Getter
int Author::getAuthorCount() { return authorCount; }

Category::Category(const std::string& categoryName, int categoryID) {
    if (categoryName.empty() || categoryID < 0) {
        throw std::invalid_argument("Category constructor: Invalid data provided!");
    }
    this->categoryName = categoryName;
    this->categoryID = categoryID;
    categoryCount++;
}

// Getters
std::string Category::getName() const { return categoryName; }
int Category::getID() const { return categoryID; }

int Category::getNumBooks() const {
    sqlite3* db;
    sqlite3_open("Data/Database.db", &db);
    std::string sql = "SELECT COUNT(*) FROM Book_category WHERE Category_id = " + std::to_string(categoryID);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    int numBooks = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        numBooks = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return numBooks;
}

// Setters
void Category::setName(const std::string& categoryName) {
    if (categoryName.empty()) throw std::invalid_argument("setCategoryName: Category name cannot be empty!");
    this->categoryName = categoryName;
}

void Category::setID(int categoryID) {
    if (categoryID <= 0) throw std::invalid_argument("setCategoryID: Category ID must be positive!");
    this->categoryID = categoryID;
}

// Static Getter
int Category::getCategoryCount() { return categoryCount; }

