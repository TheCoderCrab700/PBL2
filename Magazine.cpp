#include "Include/Magazine.h"
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

// Constructor
Magazine::Magazine(string ISSN, const std::string& title, double price, int publisherID, 
                   const std::string& establishedDate)
    : ISSN(ISSN), title(title), price(price), publisherID(publisherID),
      establishedDate(establishedDate) {}

// Getters
string Magazine::getISSN() const { return ISSN; }
string Magazine::getTitle() const { return title; }
double Magazine::getPrice() const { return price; }
int Magazine::getPublisherID() const { return publisherID; }
string Magazine::getEstablishedDate() const { return establishedDate; }
vector<string> Magazine::getCategories() const {return categories;}


// Setters
void Magazine::setISSN(const string& newISSN) { ISSN = newISSN; }
void Magazine::setTitle(const string& newTitle) { title = newTitle; }
void Magazine::setPublisherID(int newPublisherID) {publisherID = newPublisherID; }
void Magazine::setestablishedDate(const string& newestablishedDate) {establishedDate = newestablishedDate; }
void Magazine::setPrice(double newPrice) {price = newPrice; }

// Category management
void Magazine::addCategory(const string& category) {
    // Check if the category already exists in the vector
    if (std::find(categories.begin(), categories.end(), category) == categories.end()) {
        // Category does not exist, so add it
        categories.push_back(category);
    } else {
        // Category already exists, print a message or handle as needed
        std::cout << "Category \"" << category << "\" already exists in the list." << std::endl;
    }
}
void Magazine::deleteCategory(const string& category) {
    auto it = remove(categories.begin(), categories.end(), category);
    if (it != categories.end()) {
        categories.erase(it);
    }
}

bool Magazine::loadCategoriesFromDB(sqlite3* db) {
    std::string query = "SELECT Name FROM Category "
                        "JOIN Magazine_category ON Category.ID = Magazine_category.Category_id "
                        "WHERE Magazine_category.ISSN = ?;";

    sqlite3_stmt* stmt;
    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind the ISSN as a text parameter
    if (sqlite3_bind_text(stmt, 1, ISSN.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cerr << "Failed to bind ISSN: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    categories.clear();
    // Execute the statement and load results into categories vector
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* categoryName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (categoryName) {
            categories.push_back(categoryName);
        }
    }

    // Finalize the statement to clean up
    sqlite3_finalize(stmt);

    return true;
}

bool Magazine::saveCategoriesToDB(sqlite3* db) {
    // Begin a transaction
    char* errorMsg;
    if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &errorMsg) != SQLITE_OK) {
        std::cerr << "Failed to begin transaction: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
        return false;
    }

    // Step 1: Remove existing category relationships for this magazine
    std::string deleteQuery = "DELETE FROM Magazine_category WHERE ISSN = ?;";
    sqlite3_stmt* deleteStmt;
    if (sqlite3_prepare_v2(db, deleteQuery.c_str(), -1, &deleteStmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare delete statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    sqlite3_bind_text(deleteStmt, 1, ISSN.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(deleteStmt) != SQLITE_DONE) {
        std::cerr << "Failed to delete old categories for ISSN " << ISSN << ": " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(deleteStmt);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    sqlite3_finalize(deleteStmt);

    // Step 2: Insert the new categories
    std::string insertQuery = "INSERT INTO Magazine_category (ISSN, Category_id) "
                              "VALUES (?, (SELECT ID FROM Category WHERE Name = ?));";
    sqlite3_stmt* insertStmt;
    if (sqlite3_prepare_v2(db, insertQuery.c_str(), -1, &insertStmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    for (const auto& category : categories) {
        sqlite3_bind_text(insertStmt, 1, ISSN.c_str(), -1, SQLITE_STATIC);  
        sqlite3_bind_text(insertStmt, 2, category.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(insertStmt) != SQLITE_DONE) {
            std::cerr << "Error saving category: " << category << " - " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(insertStmt);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }
        sqlite3_reset(insertStmt);  // Reset for the next category
    }
    sqlite3_finalize(insertStmt);

    // Commit the transaction
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &errorMsg) != SQLITE_OK) {
        std::cerr << "Failed to commit transaction: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    return true;
}


// Save to database
bool Magazine::saveToDatabase(sqlite3* db) {
    char* errMsg = 0;
    string sql = "INSERT INTO Magazine (ISSN, Title, Price, Publisher_ID, Established_date) "
                 "VALUES ('" + ISSN + "', '" + title + "', " + to_string(price) + ", '" + 
                 to_string(publisherID) + "', '" + establishedDate + "');";
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    if(!saveCategoriesToDB(db)) return false;
    return true;
}

// Load from database
bool Magazine::loadFromDatabase(sqlite3* db, string ISSN) {
    sqlite3_stmt* stmt;
    string sql = "SELECT Title, Price, Publisher_ID, Established_date FROM Magazine WHERE ISSN = '" + ISSN + "';";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cerr << "Failed to prepare statement\n";
        return false;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        this->ISSN = ISSN;
        title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        price = sqlite3_column_double(stmt, 1);
        publisherID = sqlite3_column_int(stmt, 2);
        establishedDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    } else {
        cerr << "No record found for ISSN: " << ISSN << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    if(!loadCategoriesFromDB(db)) return false;
    sqlite3_finalize(stmt);
    return true;
}

// Display magazine details
void Magazine::display() const {
    cout << "Magazine Details:" << endl;
    cout << "ISSN: " << ISSN << endl;
    cout << "Title: " << title << endl;
    cout << "Price: " << price << endl;
    cout << "Publisher ID: " << publisherID << endl;
    cout << "Categories: ";
    for (const auto& category : categories) {
        cout << category;
        if(category!=categories.back()) cout<<"; ";
    }
    cout << "\nEstablished Date: " << establishedDate << endl;
}


bool Magazine::updateToDatabase(sqlite3* db) {
    char* errMsg = 0;
    stringstream sql;
    sql << "UPDATE MAGAZINE SET Title = '" << title << "', Price = " << price
        << ", Publisher_ID = " << publisherID << ", Established_date = '" << establishedDate
        << "' WHERE ISSN = '" << ISSN << "';";

    if (sqlite3_exec(db, sql.str().c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    if(!saveCategoriesToDB(db)) return false;
    cout << "Magazine updated successfully." << endl;
    return true;
}
