#ifndef INFOCLASSES_H
#define INFOCLASSES_H

#include <string>

class Author { 
private:
    std::string firstName;
    std::string lastName;
    int authorID;

    // Static variable to keep track of the number of Author objects
    static int authorCount;

public:
    // Constructor
    Author(const std::string& firstName = "", const std::string& lastName = "", int authorID = 0);

    // Getters
    std::string getFirstName() const;
    std::string getLastName() const;
    int getID() const;

    // Setters
    void setFirstName(const std::string& firstName);
    void setLastName(const std::string& lastName);
    void setID(int authorID);

    
    // Static method to get the number of Author objects
    static int getAuthorCount();

    int getNumBooks() const; // New method to get the number of books for the author
};


class Category {
private:
    std::string categoryName;
    int categoryID;

    // Static variable to keep track of the number of Category objects
    static int categoryCount;

public:
    // Constructor
    Category(const std::string& categoryName = "", int categoryID = 0);

    // Getters
    std::string getName() const;
    int getID() const;

    // Setters
    void setName(const std::string& categoryName);
    void setID(int categoryID);

    // Static method to get the number of Category objects
    static int getCategoryCount();

    int getNumBooks() const; // New method to get the number of books for the category
};

#endif // INFOCLASSES_H
