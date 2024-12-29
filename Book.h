// Book.h
#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
#include <sqlite\sqlite3.h>
#include "InfoClasses.h"
#include "Datetime.h"
using namespace std;

class Book {
private:
    string ISBN;
    string title;
    vector<Author> authors;
    vector<Category> categories;
    int publisherID;
    string publisher;
    Date publicationDate;
    double price;
    int copy_count;

public:
    // Constructor
    Book(string ISBN = "", const string& title = "", double price = 0.0, int publisherID = 0,Date publicationDate = Date(1999,3,7));

    // Getters
    string getISBN() const;
    string getTitle() const;
    vector<Author> getAuthors() const;
    vector<Category> getCategories() const;
    int getPublisherID() const;
    string getPublicationDate() const;
    double getPrice() const;
    string get1Author() const;
    string get1Category() const;
    string getPublisherName() const;
    // Setters
    void setISBN(const string& newISBN);
    void setTitle(const string& newTitle);
    void setPublisherID(int newPublisherID);
    void setPublicationDate(const string& newpublicationDate);
    void setPrice(double newPrice);
    void setCopy(int newCopyCount);

    // Methods for managing authors
    void addAuthor(const Author& author);
    void deleteAuthor(const Author& author);

    // Methods for managing categories
    void addCategory(const Category& category);
    void deleteCategory(const Category& category);

    // Copy and Info methods
    int  getCopy() const;
    void getInfo() const;

    //Save and load book
    bool calCopy(sqlite3* db);
    bool loadpublisher(sqlite3* db);
    bool loadAuthorsFromDB(sqlite3* db);
    bool loadCategoriesFromDB(sqlite3* db);
    bool saveCategoriesToDB(sqlite3* db);
    bool saveAuthorsToDB(sqlite3* db);
    bool saveToDatabase(sqlite3* db);
    bool loadFromDatabase(sqlite3* db, const string& ISBN);
    bool deleteFromDatabase(sqlite3* db); 

    Book& operator=(const Book& other);

};

#endif // BOOK_H
