#include "Include\Patron.h"
#include <stdexcept>
#include <sstream>
#include <ncurses/ncurses.h> // Include ncurses library
using namespace std;


// ------------------------ Patron Class ------------------------

// Constructor
Patron::Patron(int id, const string& first_name, const string& last_name, const string& email, 
               const string& status, const string& phone_number, const string& address, 
               double penalty_fee, double available_fee)
    : id(id), first_name(first_name), last_name(last_name), email(email), status(status), 
      phone_number(phone_number), address(address), penalty_fee(penalty_fee), available_fee(available_fee) {}

// Getters
int Patron::getID() const { return id; }
string Patron::getFirstName() const { return first_name; }
string Patron::getLastName() const { return last_name; }
string Patron::getEmail() const { return email; }
string Patron::getStatus() const { return status; }
string Patron::getPhoneNumber() const { return phone_number; }
string Patron::getAddress() const { return address; }
double Patron::getPenaltyFee() const { return penalty_fee; }
double Patron::getAvailableFee() const { return available_fee; }

// Setters
void Patron::updateFirstName(const string& newFirstName) { first_name = newFirstName; }
void Patron::updateLastName(const string& newLastName) { last_name = newLastName; }
void Patron::updateEmail(const string& newEmail) { email = newEmail; }
void Patron::updateStatus(const string& newStatus) { status = newStatus; }
void Patron::updatePhone(const string& newPhone) { phone_number = newPhone; }
void Patron::updateAddress(const string& newAddress) { address = newAddress; }

// Fee calculation
void Patron::updatePenalty(bool type, float price) {
    if (type) penalty_fee -= price * 0.1;
    else penalty_fee += price * 0.1;
}

void Patron::updateAvailable(bool type, float price) {
    if (type) available_fee -= price;
    else available_fee += price;
}

// Display Patron details
void Patron::display() const {
    printw("Patron Details:\n");
    printw("ID: %d\n", id);
    printw("Name: %s %s\n", first_name.c_str(), last_name.c_str());
    printw("Email: %s\n", email.c_str());
    printw("Status: %s\n", status.c_str());
    printw("Phone: %s\n", phone_number.c_str());
    printw("Address: %s\n", address.c_str());
    printw("Penalty Fee: %.3f VND\n", penalty_fee);
    printw("Available Fee: %.3f VND\n", available_fee);
}

// Save Patron to database
bool Patron::saveToDatabase(sqlite3* db) {
    try {
        if (!db) throw runtime_error("Database connection is null.");

        string sql = "INSERT INTO Patron (first_name, last_name, email, status, phone_number, address, penalty_fee, available_fee, role) "
                     "VALUES ('" + first_name + "', '" + last_name + "', '" + email + "', '" + status +
                     "', '" + phone_number + "', '" + address + "', " + to_string(penalty_fee) + ", " + to_string(available_fee) +",'New'"+ ");";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in saveToDatabase: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

// Load Patron from database
bool Patron::loadFromDatabase(sqlite3* db, int patron_id) {
    try {
        if (!db) throw runtime_error("Database connection is null.");

        sqlite3_stmt* stmt;
        string sql = "SELECT first_name, last_name, email, status, phone_number, address, penalty_fee, available_fee "
                     "FROM Patron WHERE ID = " + to_string(patron_id) + ";";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare SQL statement in loadFromDatabase.");
        }

        id = patron_id; // Set the ID
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            first_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            last_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            phone_number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            penalty_fee = sqlite3_column_double(stmt, 6);
            available_fee = sqlite3_column_double(stmt, 7);
        } else {
            sqlite3_finalize(stmt);
            throw runtime_error("No record found for the provided Patron ID.");
        }

        sqlite3_finalize(stmt);
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

// Update Patron in database
bool Patron::updateToDatabase(sqlite3* db) {
    try {
        if (!db) throw runtime_error("Database connection is null.");

        string sql = "UPDATE Patron SET first_name = '" + first_name + "', last_name = '" + last_name + "', email = '" + email +
                     "', status = '" + status + "', phone_number = '" + phone_number + "', address = '" + address + 
                     "', penalty_fee = " + to_string(penalty_fee) + ", available_fee = " + to_string(available_fee) +
                     " WHERE ID = " + to_string(id) + ";";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in updateToDatabase: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

// ------------------------ Student Class ------------------------


// Constructor
Student::Student(int id, const string& first_name, const string& last_name, const string& email, const string& status,
                 const string& phone_number, const string& address, double penalty_fee, double available_fee,
                 const string& faculty, int year_of_enrolment)
    : Patron(id, first_name, last_name, email, status, phone_number, address, penalty_fee, available_fee),
      faculty(faculty), year_of_enrolment(year_of_enrolment) {}

// Getters
string Student::getFaculty() const { return faculty; }
int Student::getYearOfEnrolment() const { return year_of_enrolment; }

// Setters
void Student::updateFaculty(const string& newFaculty) { faculty = newFaculty; }
void Student::updateYearOfEnrolment(int newYear) { year_of_enrolment = newYear; }

// Save to database
bool Student::saveToDatabase(sqlite3* db) {
    try {
        if (!db) throw runtime_error("Database connection is null.");
        char* errMsg = nullptr;
        // Save base Patron data
        if (!Patron::saveToDatabase(db)) return false;
        // Retrieve the new ID from the Patron table
        sqlite3_stmt* stmt;
        string getIdSql = "SELECT ID FROM Patron WHERE role = 'New';";
        if (sqlite3_prepare_v2(db, getIdSql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare SQL statement to retrieve new ID.");
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int newId = sqlite3_column_int(stmt, 0);
            this->id = newId; // Set the new ID
        } else {
            sqlite3_finalize(stmt);
            throw runtime_error("No new ID found in the Patron table.");
        }

        sqlite3_finalize(stmt);
        // Save Student-specific data
        string sql = "INSERT INTO Student (Patron_ID, Faculty, Year_of_Enrolment) VALUES (" + to_string(getID()) +
                     ", '" + faculty + "', " + to_string(year_of_enrolment) + ");";

        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in Student::saveToDatabase: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        // Update role to 'Student' in Patron table
        string updateRoleSql = "UPDATE Patron SET role = 'Student' WHERE role = 'New'  ;";
        if (sqlite3_exec(db, updateRoleSql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in updating role: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

// Load from database
bool Student::loadFromDatabase(sqlite3* db, int patron_id) {
    try {
        if (!db) throw runtime_error("Database connection is null.");

        // Load base Patron data
        if (!Patron::loadFromDatabase(db, patron_id)) return false;

        sqlite3_stmt* stmt;
        string sql = "SELECT Faculty, Year_of_Enrolment FROM Student WHERE Patron_ID = " + to_string(patron_id) + ";";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare SQL statement in Student::loadFromDatabase.");
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            faculty = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            year_of_enrolment = sqlite3_column_int(stmt, 1);
        } else {
            sqlite3_finalize(stmt);
            throw runtime_error("No record found for the provided Student ID.");
        }

        sqlite3_finalize(stmt);
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

// Update in database
bool Student::updateToDatabase(sqlite3* db) {
    try {
        if (!db) throw runtime_error("Database connection is null.");

        // Update base Patron data
        if (!Patron::updateToDatabase(db)) return false;

        string sql = "UPDATE Student SET Faculty = '" + faculty + "', Year_of_Enrolment = '" + to_string(year_of_enrolment) +
                     "' WHERE Patron_ID = " + to_string(getID()) + ";";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in Student::updateToDatabase: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

void Student::display() const {
    Patron::display();
    printw("Role: Student\n");
    printw("Faculty: %s\n", faculty.c_str());
    printw("Year of Enrolment: %d\n", year_of_enrolment); 

}
string Student::getRole() const {return "Student";}



// ------------------------ Lecturer Class ------------------------

// Constructor
Lecturer::Lecturer(int id, const string& first_name, const string& last_name, const string& email, const string& status,
                   const string& phone_number, const string& address, double penalty_fee, double available_fee,
                   const string& faculty, const string& degree)
    : Patron(id, first_name, last_name, email, status, phone_number, address, penalty_fee, available_fee),
      faculty(faculty), degree(degree) {}

// Getters
string Lecturer::getFaculty() const { return faculty; }
string Lecturer::getDegree() const { return degree; }

// Setters
void Lecturer::updateFaculty(const string& newFaculty) { faculty = newFaculty; }
void Lecturer::updateDegree(const string& newdegree) { degree = newdegree; }

// Save to database
bool Lecturer::saveToDatabase(sqlite3* db) {
    try {
        if (!db) throw runtime_error("Database connection is null.");
        char* errMsg = nullptr;
        // Save base Patron data
        if (!Patron::saveToDatabase(db)) return false;
        // Retrieve the new ID from the Patron table
        sqlite3_stmt* stmt;
        string getIdSql = "SELECT ID FROM Patron WHERE role = 'New';";
        if (sqlite3_prepare_v2(db, getIdSql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare SQL statement to retrieve new ID.");
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int newId = sqlite3_column_int(stmt, 0);
            this->id = newId; // Set the new ID
        } else {
            sqlite3_finalize(stmt);
            throw runtime_error("No new ID found in the Patron table.");
        }

        sqlite3_finalize(stmt);
        // Update role to 'Lecturer' in Patron table
        string updateRoleSql = "UPDATE Patron SET role = 'Lecturer' WHERE role = 'New'  ;";
        if (sqlite3_exec(db, updateRoleSql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in updating role: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        // Save Lecturer-specific data
        string sql = "INSERT INTO Lecturer (Patron_ID, faculty, degree) VALUES (" + to_string(getID()) +
                     ", '" + faculty + "', '" + degree + "');";

        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in Lecturer::saveToDatabase: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

// Load from database
bool Lecturer::loadFromDatabase(sqlite3* db, int patron_id) {
    try {
        if (!db) throw runtime_error("Database connection is null.");

        // Load base Patron data
        if (!Patron::loadFromDatabase(db, patron_id)) return false;

        sqlite3_stmt* stmt;
        string sql = "SELECT faculty, degree FROM Lecturer WHERE Patron_ID = " + to_string(patron_id) + ";";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare SQL statement in Lecturer::loadFromDatabase.");
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            faculty = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            degree = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        } else {
            sqlite3_finalize(stmt);
            throw runtime_error("No record found for the provided Lecturer ID.");
        }

        sqlite3_finalize(stmt);
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

// Update in database
bool Lecturer::updateToDatabase(sqlite3* db) {
    try {
        if (!db) throw runtime_error("Database connection is null.");

        // Update base Patron data
        if (!Patron::updateToDatabase(db)) return false;

        string sql = "UPDATE Lecturer SET faculty = '" + faculty + "', degree = '" + degree +
                     "' WHERE Patron_ID = " + to_string(getID()) + ";";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in Lecturer::updateToDatabase: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}
//Display
void Lecturer::display() const {
    Patron::display();
    printw("Role: Lecturer\n");
    printw("Faculty: %s\n", faculty.c_str());
    printw("Degree: %s\n", degree.c_str()); 

}
string Lecturer::getRole() const { return "Lecturer"; }


// ------------------------ Staff Class ------------------------

// Constructor
Staff::Staff(int id, const string& first_name, const string& last_name, const string& email, const string& status,
             const string& phone_number, const string& address, double penalty_fee, double available_fee,
             const string& department)
    : Patron(id, first_name, last_name, email, status, phone_number, address, penalty_fee, available_fee),
      department(department) {}

// Getters
string Staff::getDepartment() const { return department; }

// Setters
void Staff::updateDepartment(const string& newDepartment) { department = newDepartment; }

// Save to database
bool Staff::saveToDatabase(sqlite3* db) {
    try {
        if (!db) throw runtime_error("Database connection is null.");
        char* errMsg = nullptr;
        // Save base Patron data
        if (!Patron::saveToDatabase(db)) return false;
        // Retrieve the new ID from the Patron table
        sqlite3_stmt* stmt;
        string getIdSql = "SELECT ID FROM Patron WHERE role = 'New';";
        if (sqlite3_prepare_v2(db, getIdSql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare SQL statement to retrieve new ID.");
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int newId = sqlite3_column_int(stmt, 0);
            this->id = newId; // Set the new ID
        } else {
            sqlite3_finalize(stmt);
            throw runtime_error("No new ID found in the Patron table.");
        }

        sqlite3_finalize(stmt);
        // Update role to 'Staff' in Patron table
        string updateRoleSql = "UPDATE Patron SET role = 'Staff' WHERE role = 'New'  ;";
        if (sqlite3_exec(db, updateRoleSql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in updating role: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        // Save Staff-specific data
        string sql = "INSERT INTO Staff (Patron_ID, department) VALUES (" + to_string(getID()) +
                     ", '" + department + "');";

        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in Staff::saveToDatabase: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

// Load from database
bool Staff::loadFromDatabase(sqlite3* db, int patron_id) {
    try {
        if (!db) throw runtime_error("Database connection is null.");

        // Load base Patron data
        if (!Patron::loadFromDatabase(db, patron_id)) return false;

        sqlite3_stmt* stmt;
        string sql = "SELECT department FROM Staff WHERE Patron_ID = " + to_string(patron_id) + ";";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare SQL statement in Staff::loadFromDatabase.");
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            department = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        } else {
            sqlite3_finalize(stmt);
            throw runtime_error("No record found for the provided Staff ID.");
        }

        sqlite3_finalize(stmt);
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

// Update in database
bool Staff::updateToDatabase(sqlite3* db) {
    try {
        if (!db) throw runtime_error("Database connection is null.");

        // Update base Patron data
        if (!Patron::updateToDatabase(db)) return false;

        string sql = "UPDATE Staff SET department = '" + department + 
                     "' WHERE Patron_ID = " + to_string(getID()) + ";";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            stringstream errorStream;
            errorStream << "SQL error in Staff::updateToDatabase: " << errMsg;
            sqlite3_free(errMsg);
            throw runtime_error(errorStream.str());
        }
        return true;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}

// Display

void Staff::display() const {
    Patron::display();
    printw("Role: Staff\n");
    printw("Department: %s\n", department.c_str()); 

}

string Staff::getRole() const { return "Staff"; }