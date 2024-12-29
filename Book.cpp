#include "Include\Book.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <ncurses/ncurses.h>

using namespace std;

// Constructor
Book::Book(string ISBN, const string& title, double price, int publisherID, Date publicationDate)
    : ISBN(ISBN), title(title), price(price), publisherID(publisherID), publicationDate(publicationDate) {copy_count = 0;}

// Getters
string Book::getISBN() const { return ISBN; }
string Book::getTitle() const { return title; }
vector<Author> Book::getAuthors() const { return authors; }
vector<Category> Book::getCategories() const { return categories; }
int Book::getPublisherID() const { return publisherID; }
string Book::getPublicationDate() const { return publicationDate.tostring(); }
double Book::getPrice() const { return price; }

string Book::getPublisherName() const {
    return publisher;
}

// Setters
void Book::setISBN(const string& newISBN) { ISBN = newISBN; }
void Book::setTitle(const string& newTitle) { title = newTitle; }
void Book::setPublisherID(int newPublisherID) { publisherID = newPublisherID; }
void Book::setPublicationDate(const string& newPublicationDate) {
    publicationDate = newPublicationDate; // Assuming Date can be constructed from a string
}
void Book::setPrice(double newPrice) { price = newPrice; }

void Book::setCopy(int newCopyCount) {
    copy_count = newCopyCount;
}

// Author management
void Book::addAuthor(const Author& author) {
    // Check if the author with the same ID already exists in the list
    auto it = find_if(authors.begin(), authors.end(), [&author](const Author& a) {
        return (a.getID() == author.getID() ); 
    });
    if (it == authors.end()) {
        authors.push_back(author);
    } else {
        throw logic_error("Author already exists in the list.");
    }
}


void Book::deleteAuthor(const Author& author) {
    auto it = find_if(authors.begin(), authors.end(), [&author](const Author& a) {
        return (a.getID() == author.getID() ); 
    });
    if (it == authors.end()) {
        authors.erase(it);
    }
}

// Category management
void Book::addCategory(const Category& category) {
    // Check if the author with the same ID already exists in the list
    auto it = find_if(categories.begin(), categories.end(), [&category](const Category& c) {
        return (c.getID() == category.getID() ); 
    });

    if (it == categories.end()) {
        categories.push_back(category);
    } else {
        throw logic_error("Category already exists in the list.");
    }
}

void Book::deleteCategory(const Category& category) {
    auto it = find_if(categories.begin(), categories.end(), [&category](const Category& c) {
        return (c.getID() == category.getID() ); 
    });
    if (it == categories.end()) {
        categories.erase(it);
    }
}


// Get number of copies of the book
int Book::getCopy() const {
    return copy_count;
}

// Display book info
void Book::getInfo() const {
    clear(); // Clear the screen
    mvprintw(0, 0, "ISBN: %s", ISBN.c_str());
    mvprintw(1, 0, "Title: %s", title.c_str());
    mvprintw(2, 0, "Publisher: %s", publisher.c_str());
    mvprintw(3, 0, "Publication Date: %s", publicationDate.tostring().c_str());
    mvprintw(4, 0, "Price: %.3f VND", price * 1000);

    mvprintw(5, 0, "Authors: ");
    int line = 6;
    for (const auto& author : authors) {
        mvprintw(line++, 4, "- %s %s", author.getFirstName().c_str(), author.getLastName().c_str());
    }

    mvprintw(line++, 0, "Categories: ");
    for (const auto& category : categories) {
        mvprintw(line++, 4, "- %s", category.getName().c_str());
    }

    mvprintw(line++, 0, "Number of available copy: %d", copy_count);
}

bool Book::loadpublisher(sqlite3* db) {
    sqlite3_stmt* stmt = nullptr; // Initialize to nullptr for safety
    try {
        // Prepare the SQL statement
        std::string sql = "SELECT Name FROM Publisher WHERE ID = " + std::to_string(publisherID) + ";";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        }

        // Execute the query and get the result
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            publisher = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        } else {
            throw std::runtime_error("Publisher ID not found in the database.");
        }

        sqlite3_finalize(stmt); // Clean up
        stmt = nullptr;         // Avoid double finalization
        return true;
    } catch (const std::exception& e) {
        if (stmt) {
            sqlite3_finalize(stmt); // Ensure resources are released
        }
        std::cerr << "Error in loadpublisher(): " << e.what() << std::endl;
        return false; // Indicate failure
    }
}

bool Book::calCopy(sqlite3* db) {
    sqlite3_stmt* stmt = nullptr; // Initialize to nullptr for safety
    try {
        // Prepare the SQL statement
        std::string sql = "SELECT COUNT(*) AS copy_count FROM Copy WHERE ISBN = '" + ISBN + "' AND Status = 'Available';";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        }

        // Execute the query and get the result
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            copy_count = sqlite3_column_int(stmt, 0);
        } else {
            throw std::runtime_error("Failed to retrieve count of copies from the database.");
        }

        sqlite3_finalize(stmt); // Clean up
        stmt = nullptr;         // Avoid double finalization
        return true;
    } catch (const std::exception& e) {
        if (stmt) {
            sqlite3_finalize(stmt); // Ensure resources are released
        }
        std::cerr << "Error in calCopy(): " << e.what() << std::endl;
        return false; // Indicate failure
    }
}


// Database methods
bool Book::loadAuthorsFromDB(sqlite3* db) {
    const string query = R"(
        SELECT Author.ID, Author.First_name, Author.Last_name
        FROM Author
        JOIN Book_author ON Author.ID = Book_author.Author_ID
        WHERE Book_author.ISBN = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    try {
        // Prepare the SQL query
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }

        // Bind the ISBN to the query
        if (sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
            throw runtime_error("Failed to bind ISBN: " + string(sqlite3_errmsg(db)));
        }

        authors.clear();

        // Execute the query and load the results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int authorID = sqlite3_column_int(stmt, 0);
            string firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

            authors.emplace_back(firstName, lastName, authorID);
        }
        sqlite3_finalize(stmt);
        return true;
    } catch (const exception& e) {
        cerr << "Error in loadAuthorsFromDB: " << e.what() << endl;
        if (stmt) sqlite3_finalize(stmt);
        return false;
    }
}


bool Book::saveAuthorsToDB(sqlite3* db) {
    const string query = R"(
        INSERT INTO Book_author (ISBN, Author_ID)
        VALUES (?, ?)
        ON CONFLICT DO NOTHING;
    )";

    sqlite3_stmt* stmt = nullptr;

    try {
        // Prepare the SQL query
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }

        // Iterate through authors and bind their IDs to the query
        for (const auto& author : authors) {
            if (sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
                sqlite3_bind_int(stmt, 2, author.getID()) != SQLITE_OK) {
                throw runtime_error("Failed to bind data: " + string(sqlite3_errmsg(db)));
            }

            // Execute the query
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw runtime_error("Failed to execute query: " + string(sqlite3_errmsg(db)));
            }

            sqlite3_reset(stmt); // Reset the statement for the next iteration
        }

        sqlite3_finalize(stmt);
        return true;
    } catch (const exception& e) {
        cerr << "Error in saveAuthorsToDB: " << e.what() << endl;
        if (stmt) sqlite3_finalize(stmt);
        return false;
    }
}


bool Book::loadCategoriesFromDB(sqlite3* db) {
    const string query = R"(
        SELECT Category.ID, Category.Name
        FROM Category
        JOIN Book_category ON Category.ID = Book_category.Category_ID
        WHERE Book_category.ISBN = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    try {
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }

        if (sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
            throw runtime_error("Failed to bind ISBN: " + string(sqlite3_errmsg(db)));
        }

        categories.clear();

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int categoryID = sqlite3_column_int(stmt, 0);
            string categoryName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            categories.emplace_back(categoryName,categoryID); // Assuming Category constructor matches
        }

        sqlite3_finalize(stmt);
        return true;
    } catch (const exception& e) {
        cerr << "Error in loadCategoriesFromDB: " << e.what() << endl;
        if (stmt) sqlite3_finalize(stmt);
        return false;
    }
}


bool Book::saveCategoriesToDB(sqlite3* db) {
    const string query = R"(
        INSERT INTO Book_category (ISBN, Category_ID)
        VALUES (?, ?)
        ON CONFLICT DO NOTHING;
    )";

    sqlite3_stmt* stmt = nullptr;

    try {
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }

        for (const auto& category : categories) {
            if (sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
                sqlite3_bind_int(stmt, 2, category.getID()) != SQLITE_OK) {
                throw runtime_error("Failed to bind data: " + string(sqlite3_errmsg(db)));
            }

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw runtime_error("Failed to execute query: " + string(sqlite3_errmsg(db)));
            }

            sqlite3_reset(stmt);
        }

        sqlite3_finalize(stmt);
        return true;
    } catch (const exception& e) {
        cerr << "Error in saveCategoriesToDB: " << e.what() << endl;
        if (stmt) sqlite3_finalize(stmt);
        return false;
    }
}

bool Book::saveToDatabase(sqlite3* db) {
    const string query = R"(
        INSERT INTO Book (ISBN, Title, Price, Publisher_ID , Publication_date)
        VALUES (?, ?, ?, ?, ?)
        ON CONFLICT(ISBN) DO UPDATE SET
            Title = excluded.Title,
            Price = excluded.Price,
            Publisher_ID = excluded.Publisher_ID,
            Publication_date = excluded.Publication_date;
    )";

    sqlite3_stmt* stmt = nullptr;

    try {
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare saveToDatabase statement: " + string(sqlite3_errmsg(db)));
        }

        // Bind book attributes to the query
        sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, price);
        sqlite3_bind_int(stmt, 4, publisherID);
        sqlite3_bind_text(stmt, 5, publicationDate.tostring().c_str(),-1, SQLITE_STATIC);

        // Execute the query
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw runtime_error("Failed to save book data: " + string(sqlite3_errmsg(db)));
        }

        sqlite3_finalize(stmt);

        // Save authors and categories
        return saveAuthorsToDB(db) && saveCategoriesToDB(db) && loadpublisher(db);
    } catch (const exception& e) {
        cerr << "Error in saveToDatabase: " << e.what() << endl;
        if (stmt) sqlite3_finalize(stmt);
        return false;
    }
}

bool Book::loadFromDatabase(sqlite3* db, const string& isbn) {
    const string query = R"(
        SELECT Title, Price, Publisher_ID, Publication_date
        FROM Book
        WHERE ISBN = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    try {
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare loadFromDatabase statement: " + string(sqlite3_errmsg(db)));
        }

        // Bind the ISBN to the query
        sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_STATIC);

        // Execute the query and load data
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            price = sqlite3_column_double(stmt, 1);
            publisherID = sqlite3_column_int(stmt, 2);
            publicationDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            this->ISBN = isbn; // Store ISBN in the object
        } else {
            cerr << "No book found with ISBN: " << isbn << endl;
            sqlite3_finalize(stmt);
            return false;
        }

        sqlite3_finalize(stmt);

        // Load authors and categories
        return loadAuthorsFromDB(db) && loadCategoriesFromDB(db) && calCopy(db) && loadpublisher(db);
    } catch (const exception& e) {
        cerr << "Error in loadFromDatabase: " << e.what() << endl;
        if (stmt) sqlite3_finalize(stmt);
        return false;
    }
}

bool Book::deleteFromDatabase(sqlite3* db) {
    const string query = R"(
        DELETE FROM Book WHERE ISBN = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    try {
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare deleteFromDatabase statement: " + string(sqlite3_errmsg(db)));
        }

        // Bind the ISBN to the query
        sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_STATIC);

        // Execute the query
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw runtime_error("Failed to delete book data: " + string(sqlite3_errmsg(db)));
        }

        sqlite3_finalize(stmt);
        return true;
    } catch (const exception& e) {
        cerr << "Error in deleteFromDatabase: " << e.what() << endl;
        if (stmt) sqlite3_finalize(stmt);
        return false;
    }
}

string Book::get1Author() const {
    if (!authors.empty()) {
        return authors.front().getFirstName() + " " + authors.front().getLastName();
    }
    return "No authors available";
}

string Book::get1Category() const {
    if (!categories.empty()) {
        return categories.front().getName();
    }
    return "No categories available";
}

Book& Book::operator=(const Book& other) {
    if (this != &other) {
        ISBN = other.ISBN;
        title = other.title;
        authors = other.authors;
        categories = other.categories;
        publisherID = other.publisherID;
        publisher = other.publisher;
        publicationDate = other.publicationDate;
        price = other.price;
        copy_count = other.copy_count;
    }
    return *this;
}

