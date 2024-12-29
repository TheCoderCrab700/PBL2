#ifndef PATRON_H
#define PATRON_H

#include <string>
#include <iostream>
#include <sqlite\sqlite3.h>
using namespace std;
class Patron {
protected:
    int id;
    string first_name;
    string last_name;
    string email;
    string status;
    string phone_number;
    string address;
    double penalty_fee;
    double available_fee;

public:
    Patron(int id = 0, const string& first_name = "", const string& last_name = "", 
           const string& email = "", const string& status = "", const string& phone_number = "", 
           const string& address = "", double penalty_fee = 0.0, double available_fee = 0.0);

    // Getters
    int getID() const;
    string getFirstName() const;
    string getLastName() const;
    string getEmail() const;
    string getStatus() const;
    string getPhoneNumber() const;
    string getAddress() const;
    double getPenaltyFee() const;
    double getAvailableFee() const;

    // Setters
    void updateFirstName(const string& newFirstName);
    void updateLastName(const string& newLastName);
    void updateEmail(const string& newEmail);
    void updateStatus(const string& newStatus);
    void updatePhone(const string& newPhone);
    void updateAddress(const string& newAddress);
    // Display method to show Patron details
    virtual void display() const = 0;

    // Get the role type as a string
    virtual string getRole() const = 0; 
    
    //type = 0 (increase), type = 1 (pay)
    void updatePenalty(bool type = 0, float price = 0.0 ); 
    void updateAvailable(bool type = 0, float price = 0.0) ;

    // Database methods
    virtual bool saveToDatabase(sqlite3* db) = 0;  // Save to DB
    virtual bool loadFromDatabase(sqlite3* db, int patron_id) = 0;  // Load from DB
    virtual bool updateToDatabase(sqlite3* db) = 0;// Update to DB
    virtual ~Patron() = default; // Virtual destructor
};

// Derived classes for specific roles

class Student : public Patron {
private:
    string faculty;
    int year_of_enrolment;

public:
    // Constructor
    Student(int id = 0, const string& first_name = "", const string& last_name = "", const string& email = "", 
            const string& status = "", const string& phone_number = "", 
            const string& address = "", double penalty_fee = 0.0, double available_fee = 0.0,
            const string& faculty = "", int year_of_enrolment = 0);

    // Getters
    string getFaculty() const;
    int getYearOfEnrolment() const;

    // Setters
    void updateFaculty(const string& newFaculty);
    void updateYearOfEnrolment(int newYear);

    // Methods 
    void display() const override;
    string getRole() const override;

    // Database methods
    bool saveToDatabase(sqlite3* db) override;
    bool loadFromDatabase(sqlite3* db, int patron_id) override;
    bool updateToDatabase(sqlite3* db) override;
};

class Lecturer : public Patron {
private:
    string faculty;
    string degree;

public:
    // Constructor
    Lecturer(int id = 0, const string& first_name = "", const string& last_name = "", const string& email = "", 
            const string& status = "", const string& phone_number = "", 
            const string& address = "", double penalty_fee = 0.0, double available_fee = 0.0,
            const string& faculty = "", const string& degree = "");

    // Getters
    string getFaculty() const;
    string getDegree() const;

    // Setters
    void updateFaculty(const string& newFaculty);
    void updateDegree(const string& newDegree);

    // Methods
    void display() const override;
    string getRole() const override;

    // Database methods
    bool saveToDatabase(sqlite3* db) override;
    bool loadFromDatabase(sqlite3* db, int patron_id) override;
    bool updateToDatabase(sqlite3* db) override;
};

class Staff : public Patron {
private:
    string department;

public:
    // Constructor
    Staff(  int id = 0, const string& first_name = "", const string& last_name = "", const string& email = "", 
            const string& status = "", const string& phone_number = "", 
            const string& address = "", double penalty_fee = 0.0, double available_fee = 0.0,
            const string& department = "");

    // Getters
    string getDepartment() const;

    // Setters
    void updateDepartment(const string& newDepartment);

    // Methods
    void display() const override;
    string getRole() const override;

    // Database methods
    bool saveToDatabase(sqlite3* db) override;
    bool loadFromDatabase(sqlite3* db, int patron_id) override;
    bool updateToDatabase(sqlite3* db) override;
};

#endif // PATRON_H



