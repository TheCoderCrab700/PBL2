#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include "Book.h"
#include "Checkout.h"
#include <sqlite\sqlite3.h>
using namespace std;
class UserAccount {
protected:
    int id;
    string first_name;
    string last_name;
    string email;
    string user_name;
    string password;

public:
    // Constructor
    UserAccount(int id = 0, const string& first_name = "", const string& last_name = "",
                const string& user_name = "", const string& password = "", const string& email = "");

    // Getters
    int getID() const;
    string getFirstName() const;
    string getLastName() const;
    string getEmail() const;
    string getUserName() const;
    string getPassword() const;

    // Setters
    void Update_FirstName(const string& new_name);
    void Update_LastName(const string& new_name);
    void Update_Email(const string& new_email);
    void Update_User_Name(const string& new_user_name);
    void Update_Password(const string& new_password);

    // Database interaction methods
    virtual bool saveToDatabase(sqlite3* db) = 0;
    bool loadFromDatabase(sqlite3* db, int user_id);
    bool updateToDatabase(sqlite3* db);

    // Pure virtual display method for derived classes
    virtual string getRole() = 0;
    virtual void display() const = 0;

    // Virtual destructor for proper cleanup in derived classes
    virtual ~UserAccount() {}
};


class Admin : public UserAccount {
public:
    // Constructor
    Admin(int id = 0 , const string& name ="", const string& role ="",
          const string& user_name ="", const string& password ="", const string& email = "");
    // Display method
    string getRole() override;
    void display() const override;
    bool saveToDatabase(sqlite3* db) override;
    bool Update_role(sqlite3* db, int user_id, const std::string& new_role);
    bool Delete_account(sqlite3* db, int user_id);
    bool Add_account(sqlite3* db, const std::string& first_name, const std::string& last_name,
                     const std::string& user_name, const std::string& password, 
                     const std::string& email, const std::string& role);
};
class Assistant : public UserAccount {
public:
    // Constructor
    Assistant(int id = 0 , const string& name ="", const string& role ="",
          const string& user_name ="", const string& password ="", const string& email = "");
    // Display method
    string getRole() override;
    void display() const override;
    bool saveToDatabase(sqlite3* db) override;

    // Functions to add and edit books
    bool Add_book(sqlite3* db, Book& book);
    bool Edit_book(sqlite3* db, Book& book);
};
class Librarian : public UserAccount {
public:
    // Constructor
    Librarian(int id = 0 , const string& name ="", const string& role ="",
          const string& user_name ="", const string& password ="", const string& email = "");
    // Display method
    string getRole() override;
    void display() const override;
    bool saveToDatabase(sqlite3* db) override;
    void lendbook(sqlite3* db, CheckOut& Check);
};
#endif // USER_H