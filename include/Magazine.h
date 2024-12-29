#pragma once
#include <string>
#include <vector>
#include <sqlite\sqlite3.h>
using namespace std;
class Magazine {
protected:
    string ISSN;
    string title;
    double price;
    int publisherID;
    string establishedDate;
    vector<string> categories;

public:
    Magazine(string ISSN = "", const string& title = "", double price = 0.0, int publisherID = 0, 
             const string& establishedDate = "");

    // Getters
    string getISSN() const;
    string getTitle() const;
    double getPrice() const;
    int getPublisherID() const;
    vector<string> getCategories() const;
    string getEstablishedDate() const;

    // Setters
    void setISSN(const string& newISSN);
    void setTitle(const string& newTitle);
    void setPublisherID(int newPublisherID);
    void setestablishedDate(const string& newestablishedDate);
    void setPrice(double newPrice);
    // Category management
    void addCategory(const string& category);
    void deleteCategory(const string& category);
    // Database functions
    bool loadCategoriesFromDB(sqlite3* db);
    bool saveCategoriesToDB(sqlite3* db);
    bool saveToDatabase(sqlite3* db);
    bool loadFromDatabase(sqlite3* db, string ISSN);
    bool updateToDatabase(sqlite3* db);
    void display() const;

};
