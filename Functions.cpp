#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sqlite3.h>
#include "Include\User.h"
#include "Include\Book.h"
#include "Include\CheckOut.h"
#include "Include\Patron.h"
#include <ncurses/ncurses.h>
#include <vector>
using namespace std;
//--------------Display Functions-----------------------

/**
 * @brief Draws a table of user accounts on the screen and allows navigation through the list.
 * 
 * This function displays a table of user accounts starting at the specified coordinates (startY, startX).
 * It allows the user to navigate through the list using the UP and DOWN arrow keys, select an entry with the Enter key,
 * and exit the table view with the 'q' or 'Q' key.
 * 
 * @param startY The starting Y coordinate for the table.
 * @param startX The starting X coordinate for the table.
 * @param users A vector of pointers to UserAccount objects to be displayed in the table.
 * @return int The index of the selected user account, or -1 if no selection was made or the user exited.
 */
int drawUserTable(int startY, int startX, const vector<UserAccount*>& users) {
    if (users.empty()) {
        mvprintw(startY, startX, "No users found.");
        return -1;
    }
    int linenum = 10;
    int currentSelection = 0;
    int displayStart = 0;
    const int displayRows = linenum - startY - 1; // Leave some space at bottom

    while (true) {
        clear();
        // Draw table header
        attron(A_BOLD);
        mvprintw(startY, startX, "ID");
        mvprintw(startY, startX + 5, "First Name");
        mvprintw(startY, startX + 25, "Last Name");
        mvprintw(startY, startX + 40, "Role");
        mvprintw(startY, startX + 55, "User Name");
        mvprintw(startY, startX + 75, "Password");
        attroff(A_BOLD);

        // Draw table content
        int row = startY + 2;
        for (int i = displayStart; i < min(displayStart + displayRows, (int)users.size()); i++) {
            if (i == currentSelection) {
                attron(A_REVERSE); // Highlight selected row
            }
            
            mvprintw(row, startX     , "%d", users[i]->getID());
            mvprintw(row, startX +  5, "%s", users[i]->getFirstName().c_str());
            mvprintw(row, startX + 25, "%s", users[i]->getLastName().c_str());
            mvprintw(row, startX + 40, "%s", users[i]->getRole().c_str());
            mvprintw(row, startX + 55, "%s", users[i]->getUserName().c_str());
            mvprintw(row, startX + 75, "%s", users[i]->getPassword().c_str());
            
            if (i == currentSelection) {
                attroff(A_REVERSE);
            }
            row++;
        }

        // Navigation instructions
        mvprintw(linenum + 2, 1, "Use UP/DOWN arrows to navigate, 'q' to exit");
        refresh();

        // Handle input
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (currentSelection > 0) {
                    currentSelection--;
                    if (currentSelection < displayStart) {
                        displayStart = currentSelection;
                    }
                }
                break;
                
            case KEY_DOWN:
                if (currentSelection < users.size() - 1) {
                    currentSelection++;
                    if (currentSelection >= displayStart + displayRows) {
                        displayStart = currentSelection - displayRows + 1;
                    }
                }
                break;

            case '\n': // Enter key
                return currentSelection;

            case 'q':
            case 'Q':
                return -1;
        }
    }
}

/**
 * @brief Draws a table of patrons on the screen and allows user to navigate and select a patron.
 * 
 * @param startY The starting Y coordinate for the table.
 * @param startX The starting X coordinate for the table.
 * @param patrons A vector of pointers to Patron objects to be displayed in the table.
 * @return int The index of the selected patron, or -1 if no selection was made or 'q' was pressed.
 * 
 * This function uses ncurses to display a table of patrons with their details such as ID, first name, last name, role, available fee, and penalty fee.
 * The user can navigate through the list using the UP and DOWN arrow keys, select a patron by pressing Enter, or exit the table by pressing 'q'.
 * If the patrons vector is empty, a message "No patrons found." is displayed and the function returns -1.
 */
int drawPatronTable(int startY, int startX, const vector<Patron*>& patrons) {
    if (patrons.empty()) {
        mvprintw(startY, startX, "No patrons found.");
        return -1;
    }
    int linenum = 15;
    int currentSelection = 0;
    int displayStart = 0;
    const int displayRows = linenum - startY - 3;

    while (true) {
        clear();
        attron(A_BOLD);
        mvprintw(startY, startX, "ID");
        mvprintw(startY, startX + 8, "First Name");
        mvprintw(startY, startX + 25, "Last Name");
        mvprintw(startY, startX + 35, "Role");
        mvprintw(startY, startX + 45, "Available Fee");
        mvprintw(startY, startX + 60, "Penalty Fee");
        attroff(A_BOLD);

        int row = startY + 2;
        for (int i = displayStart; i < min(displayStart + displayRows, (int)patrons.size()); i++) {
            if (i == currentSelection) {
                attron(A_REVERSE);
            }
            mvprintw(row, startX, "%d", patrons[i]->getID());
            mvprintw(row, startX + 8, "%s", patrons[i]->getFirstName().c_str());
            mvprintw(row, startX + 25, "%s", patrons[i]->getLastName().c_str());
            mvprintw(row, startX + 35, "%s", patrons[i]->getRole().c_str());
            mvprintw(row, startX + 45, "%.3f VND", patrons[i]->getAvailableFee());
            mvprintw(row, startX + 60, "%.3f VND", patrons[i]->getPenaltyFee());
            if (i == currentSelection) {
                attroff(A_REVERSE);
            }
            row++;
        }
        mvprintw(linenum + 2, 1, "Use UP/DOWN arrows to navigate, Enter to select, 'q' to exit");
        refresh();

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (currentSelection > 0) {
                    currentSelection-=1;
                    if (currentSelection < displayStart) {
                        displayStart = currentSelection;
                    }
                }
                break;
            case KEY_DOWN:
                if (currentSelection < patrons.size() - 1) {
                    currentSelection++;
                    if (currentSelection >= displayStart + displayRows) {
                        displayStart = currentSelection - displayRows + 1;
                    }
                }
                break;
            case '\n':
                return currentSelection;
            case 'q':
            case 'Q':
                return -1;
        }
    }
}

/**
 * @brief Draws a table of books on the screen and allows the user to navigate and select a book.
 *
 * This function displays a table of books starting at the specified coordinates (startY, startX).
 * It allows the user to navigate through the list using the UP and DOWN arrow keys, and select a book
 * by pressing the Enter key. The user can also exit the table by pressing 'q' or 'Q'.
 *
 * @param startY The starting Y coordinate for the table.
 * @param startX The starting X coordinate for the table.
 * @param books A vector of Book objects to be displayed in the table.
 * @return The index of the selected book in the vector, or -1 if no book is selected or the user exits.
 */
int drawBookTable(int startY, int startX, const vector<Book>& books) {
    if (books.empty()) {
        mvprintw(startY, startX, "No books available in the database.");
        return -1;
    }
    int linenum = 15;
    int currentSelection = 0;
    int displayStart = 0;
    const int displayRows = linenum - startY - 1;
    while (true) {
        clear();
        attron(A_BOLD);
        mvprintw(startY, startX + 3, "ISBN");
        mvprintw(startY, startX + 18, "Title");
        mvprintw(startY, startX + 48, "Author");
        mvprintw(startY, startX + 73, "Category");
        mvprintw(startY, startX + 103, "Publisher");
        mvprintw(startY, startX + 132, "Price");
        mvprintw(startY, startX + 145, "Copies");
        attroff(A_BOLD);

        int row = startY + 2;
        for (int i = displayStart; i < min(displayStart + displayRows, (int)books.size()); i++) {
            if (i == currentSelection) {
                attron(A_REVERSE);
            }
            mvprintw(row, startX, "%s", books[i].getISBN().c_str());
            mvprintw(row, startX + 15, "%.28s", books[i].getTitle().c_str());
            mvprintw(row, startX + 45, "%.28s", books[i].get1Author().c_str());
            mvprintw(row, startX + 70, "%.28s", books[i].get1Category().c_str());
            mvprintw(row, startX + 100, "%.28s", books[i].getPublisherName().c_str());
            mvprintw(row, startX + 130, "%.3f VND", books[i].getPrice());
            mvprintw(row, startX + 145, "%4d", books[i].getCopy());
            if (i == currentSelection) {
                attroff(A_REVERSE);
            }
            row++;
        }

        mvprintw(linenum + 2, 1, "Use UP/DOWN arrows to navigate, Enter to select, 'q' to exit");
        refresh();

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (currentSelection > 0) {
                    currentSelection-=1;
                    if (currentSelection < displayStart) {
                        displayStart = currentSelection;
                    }
                }
                break;
            case KEY_DOWN:
                if (currentSelection < books.size() - 1) {
                    currentSelection++;
                    if (currentSelection >= displayStart + displayRows) {
                        displayStart = currentSelection - displayRows + 1;
                    }
                }
                break;
            case '\n':
                return currentSelection;
            case 'q':
            case 'Q':
                return -1;
        }
    }
}

/**
 * @brief Draws a table of authors on the screen and allows user to navigate and select an author.
 *
 * This function displays a table of authors with their ID, first name, last name, and number of books.
 * The user can navigate through the list using the UP and DOWN arrow keys, select an author with the Enter key,
 * or exit the table with the 'q' key.
 *
 * @param startY The starting Y position on the screen for the table.
 * @param startX The starting X position on the screen for the table.
 * @param authors A vector of Author objects to be displayed in the table.
 * @return The index of the selected author in the vector, or -1 if no author is selected or if the list is empty.
 */
int drawAuthorTable(int startY, int startX, const vector<Author>& authors) {
    if (authors.empty()) {
        mvprintw(startY, startX, "No authors available in the database.");
        return -1;
    }
    int linenum = 15;
    int currentSelection = 0;
    int displayStart = 0;
    const int displayRows = linenum - startY - 1;

    while (true) {
        clear();
        attron(A_BOLD);
        mvprintw(startY, startX, "ID");
        mvprintw(startY, startX + 8, "First Name");
        mvprintw(startY, startX + 25, "Last Name");
        mvprintw(startY, startX + 40, "Book Number");
        attroff(A_BOLD);

        int row = startY + 2;
        for (int i = displayStart; i < min(displayStart + displayRows, (int)authors.size()); i++) {
            if (i == currentSelection) {
                attron(A_REVERSE);
            }
            mvprintw(row, startX, "%d", authors[i].getID());
            mvprintw(row, startX + 8, "%s", authors[i].getFirstName().c_str());
            mvprintw(row, startX + 25, "%s", authors[i].getLastName().c_str());
            mvprintw(row, startX + 40, "%10d", authors[i].getNumBooks( ));
            if (i == currentSelection) {
                attroff(A_REVERSE);
            }
            row++;
        }

        mvprintw(linenum + 2, 1, "Use UP/DOWN arrows to navigate, Enter to select, 'q' to exit");
        refresh();

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (currentSelection > 0) {
                    currentSelection--;
                    if (currentSelection < displayStart) {
                        displayStart = currentSelection;
                    }
                }
                break;
            case KEY_DOWN:
                if (currentSelection < authors.size() - 1) {
                    currentSelection++;
                    if (currentSelection >= displayStart + displayRows) {
                        displayStart = currentSelection - displayRows + 1;
                    }
                }
                break;
            case '\n':
                return currentSelection;
            case 'q':
            case 'Q':
                return -1;
        }
    }
}

/**
 * @brief Draws a table of categories and allows the user to navigate and select a category.
 *
 * This function displays a table of categories starting at the specified coordinates (startY, startX).
 * The table includes columns for ID, Name, and Book Number. The user can navigate through the table
 * using the UP and DOWN arrow keys, select a category with the Enter key, or exit the table with the 'q' key.
 *
 * @param startY The starting Y coordinate for the table.
 * @param startX The starting X coordinate for the table.
 * @param categories A vector of Category objects to be displayed in the table.
 * @return The index of the selected category, or -1 if no category is selected or if the user exits.
 */
int drawCategoryTable(int startY, int startX, const vector<Category>& categories) {
    if (categories.empty()) {
        mvprintw(startY, startX, "No categories available in the database.");
        return -1;
    }
    int linenum = 15;
    int currentSelection = 0;
    int displayStart = 0;
    const int displayRows = linenum - startY - 1;

    while (true) {
        clear();
        attron(A_BOLD);
        mvprintw(startY, startX, "ID");
        mvprintw(startY, startX + 8, "Name");
        mvprintw(startY, startX + 40, "Book Number");
        attroff(A_BOLD);

        int row = startY + 2;
        for (int i = displayStart; i < min(displayStart + displayRows, (int)categories.size()); i++) {
            if (i == currentSelection) {
                attron(A_REVERSE);
            }
            mvprintw(row, startX, "%d", categories[i].getID());
            mvprintw(row, startX + 8, "%s", categories[i].getName().c_str());
            mvprintw(row, startX + 40, "%10d", categories[i].getNumBooks( ));
            if (i == currentSelection) {
                attroff(A_REVERSE);
            }
            row++;
        }

        mvprintw(linenum + 2, 1, "Use UP/DOWN arrows to navigate, Enter to select, 'q' to exit");
        refresh();

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (currentSelection > 0) {
                    currentSelection--;
                    if (currentSelection < displayStart) {
                        displayStart = currentSelection;
                    }
                }
                break;
            case KEY_DOWN:
                if (currentSelection < categories.size() - 1) {
                    currentSelection++;
                    if (currentSelection >= displayStart + displayRows) {
                        displayStart = currentSelection - displayRows + 1;
                    }
                }
                break;
            case '\n':
                return currentSelection;
            case 'q':
            case 'Q':
                return -1;
        }
    }
}

/**
 * @brief Draws a checkout table for a specific patron and allows navigation through the list.
 * 
 * This function queries the database for checkout records of a specific patron and displays them
 * in a table format using ncurses. The user can navigate through the list using the UP and DOWN
 * arrow keys, select an item with the Enter key, or exit with the 'q' key.
 * 
 * @param startY The starting Y position for the table display.
 * @param startX The starting X position for the table display.
 * @param db A pointer to the SQLite database connection.
 * @param patronId The ID of the patron whose checkout records are to be displayed.
 * @return The selected item's ISBN if an item is selected, or an empty string if the user exits.
 * @throws std::runtime_error If there is an error preparing the SQL statement.
 */
int drawCheckoutTable(int startY, int startX, const vector<vector<string>>& Items){
    // Handle empty results
    if (Items.empty()) {
        mvprintw(startY + 2, startX, "No checkouts found for this patron.");
        getch();
        return -1;
    }

    // Navigation setup
    int linenum = 15;
    int currentSelection = 0;
    int displayStart = 0;
    const int displayRows = linenum - startY - 1;

    while (true) {
        clear();

        // Header
        attron(A_BOLD);
        mvprintw(startY, startX, "Identifier");
        mvprintw(startY, startX + 15, "Item Title");
        mvprintw(startY, startX + 48, "Copy count");
        mvprintw(startY, startX + 60, "First checkout date");
        mvprintw(startY, startX + 80, "Last due date");
        mvprintw(startY, startX + 100, "Librarian Name");

        attroff(A_BOLD);

        // Display rows
        int row = startY + 2;
        for (int i = displayStart; i < min(displayStart + displayRows, (int)Items.size()); ++i) {
            if (i == currentSelection) {
                attron(A_REVERSE);
            }
            mvprintw(row, startX     , "%.32s", Items[i][0].c_str()); // Row number
            mvprintw(row, startX + 15, "%.28s", Items[i][1].c_str());
            mvprintw(row, startX + 50, "%.28s", Items[i][2].c_str()); 
            mvprintw(row, startX + 60, "%.28s", Items[i][3].c_str()); 
            mvprintw(row, startX + 80, "%.28s", Items[i][4].c_str()); 
            mvprintw(row, startX + 100,"%.28s", Items[i][5].c_str());
            if (i == currentSelection) {
                attroff(A_REVERSE);
            }
            row++;
        }

        // Footer instructions
        mvprintw(linenum + 2, startX, "Use UP/DOWN arrows to navigate, Enter to select, 'q' to exit");
        refresh();

        // Handle user input
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (currentSelection > 0) {
                    currentSelection--;
                    if (currentSelection < displayStart) {
                        displayStart = currentSelection;
                    }
                }
                break;

            case KEY_DOWN:
                if (currentSelection < Items.size() - 1) {
                    currentSelection++;
                    if (currentSelection >= displayStart + displayRows) {
                        displayStart = currentSelection - displayRows + 1;
                    }
                }
                break;

            case '\n':
                return currentSelection;

            case 'q':
            case 'Q':
                return -1; // Exit without selection
                break;
            default:
                break; // Ignore other keys
        }
    }
}
//--------------Users processing Functions---------------
vector<UserAccount*> loadAllUsers(sqlite3* db) {
    vector<UserAccount*> users;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, Role FROM User_account";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            UserAccount* user = nullptr;
            if (role == "Admin") {
                user = new Admin();
            } else if (role == "Assistant") {
                user = new Assistant();
            } else if (role == "Librarian") {
                user = new Librarian();
            }

            if (user && user->loadFromDatabase(db, id)) {
                users.push_back(user);
            }
        }

    } catch (const exception& e) {
        cerr << "Error loading users: " << e.what() << endl;
    }

    sqlite3_finalize(stmt);
    return users;
}

vector<UserAccount*> loadAllUsers_name(sqlite3* db) {
    vector<UserAccount*> users;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, Role FROM User_account ORDER BY Last_name";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            UserAccount* user = nullptr;
            if (role == "Admin") {
                user = new Admin();
            } else if (role == "Assistant") {
                user = new Assistant();
            } else if (role == "Librarian") {
                user = new Librarian();
            }

            if (user && user->loadFromDatabase(db, id)) {
                users.push_back(user);
            }
        }

    } catch (const exception& e) {
        cerr << "Error loading users: " << e.what() << endl;
    }

    sqlite3_finalize(stmt);
    return users;
}

vector<UserAccount*> loadAllUsers_role(sqlite3* db) {
    vector<UserAccount*> users;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, Role FROM User_account ORDER BY Role, Last_name";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            UserAccount* user = nullptr;
            if (role == "Admin") {
                user = new Admin();
            } else if (role == "Assistant") {
                user = new Assistant();
            } else if (role == "Librarian") {
                user = new Librarian();
            }

            if (user && user->loadFromDatabase(db, id)) {
                users.push_back(user);
            }
        }

    } catch (const exception& e) {
        cerr << "Error loading users: " << e.what() << endl;
    }

    sqlite3_finalize(stmt);
    return users;
}

vector<UserAccount*> findUsersByName(sqlite3* db, const string& name) {
    vector<UserAccount*> users;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, Role FROM User_account WHERE First_name LIKE ? OR Last_name LIKE ?";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        string search = "%" + name + "%";
        sqlite3_bind_text(stmt, 1, search.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, search.c_str(), -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            UserAccount* user = nullptr;
            if (role == "Admin") user = new Admin();
            else if (role == "Assistant") user = new Assistant();
            else if (role == "Librarian") user = new Librarian();

            if (user && user->loadFromDatabase(db, id)) {
                users.push_back(user);
            }
        }

    } catch (const exception& e) {
        cerr << "Error finding users: " << e.what() << endl;
    }

    sqlite3_finalize(stmt);
    return users;
}

UserAccount* findUserByID(sqlite3* db, int searchID) {
    sqlite3_stmt* stmt;
    UserAccount* user = nullptr;

    try {
        string sql = "SELECT Role FROM User_account WHERE ID = ?";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, searchID);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            if (role == "Admin") user = new Admin();
            else if (role == "Assistant") user = new Assistant();
            else if (role == "Librarian") user = new Librarian();

            if (user && !user->loadFromDatabase(db, searchID)) {
                delete user;
                user = nullptr;
            }
        }

    } catch (const exception& e) {
        cerr << "Error finding user: " << e.what() << endl;
        delete user;
        user = nullptr;
    }

    sqlite3_finalize(stmt);
    return user;
}


//--------------Patron processing Functions---------------
vector<Patron*> loadAllPatrons(sqlite3* db) {
    vector<Patron*> patrons;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, Role FROM Patron WHERE Status = 'Active'";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            Patron* patron = nullptr;
            if (role == "Student") {
                patron = new Student();
            } else if (role == "Lecturer") {
                patron = new Lecturer();
            } else if (role == "Staff") {
                patron = new Staff();
            }

            if (patron && patron->loadFromDatabase(db, id)) {
                patrons.push_back(patron);
            }
        }

    } catch (const exception& e) {
        cerr << "Error loading patrons: " << e.what() << endl;
    }

    sqlite3_finalize(stmt);
    return patrons;
}

vector<Patron*> loadAllPatrons_name(sqlite3* db) {
    vector<Patron*> patrons;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, Role FROM Patron WHERE Status = 'Active' ORDER BY Last_name";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            Patron* patron = nullptr;
            if (role == "Student") {
                patron = new Student();
            } else if (role == "Lecturer") {
                patron = new Lecturer();
            } else if (role == "Staff") {
                patron = new Staff();
            }

            if (patron && patron->loadFromDatabase(db, id)) {
                patrons.push_back(patron);
            }
        }

    } catch (const exception& e) {
        cerr << "Error loading patrons: " << e.what() << endl;
    }

    sqlite3_finalize(stmt);
    return patrons;
}

vector<Patron*> findPatronsByName(sqlite3* db, const string& name) {
    vector<Patron*> patrons;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, Role FROM Patron WHERE Status = 'Active' AND (First_name LIKE ? OR Last_name LIKE ?) ";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        string search = "%" + name + "%";
        sqlite3_bind_text(stmt, 1, search.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, search.c_str(), -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            Patron* patron = nullptr;
            if (role == "Student") patron = new Student();
            else if (role == "Lecturer") patron = new Lecturer();
            else if (role == "Staff") patron = new Staff();

            if (patron && patron->loadFromDatabase(db, id)) {
                patron->display();
                patrons.push_back(patron);
            }
        }

    } catch (const exception& e) {
        cerr << "Error finding patrons: " << e.what() << endl;
    }

    sqlite3_finalize(stmt);
    return patrons;
}

Patron* findPatronByID(sqlite3* db, int searchID) {
    sqlite3_stmt* stmt;
    Patron* patron = nullptr;

    try {
        string sql = "SELECT Role FROM Patron WHERE ID = ? AND Status = 'Active'";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, searchID);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            if (role == "Student") patron = new Student();
            else if (role == "Lecturer") patron = new Lecturer();
            else if (role == "Staff") patron = new Staff();

            if (patron && !patron->loadFromDatabase(db, searchID)) {
                delete patron;
                patron = nullptr;
            }
        }

    } catch (const exception& e) {
        cerr << "Error finding patron: " << e.what() << endl;
        delete patron;
        patron = nullptr;
    }

    sqlite3_finalize(stmt);
    return patron;
}

vector<vector<string>> loadCheckoutByPatronID(sqlite3* db, int patronId) {
    // SQL query to fetch checkout data
    string sql = 
                "SELECT "
                "    COALESCE(b.ISBN, em.EISSN) AS Identifier, "
                "    COALESCE(b.Title, m.Title) AS ItemTitle, "
                "    COUNT(c.ID) AS copy_count, "
                "    MIN(c.Start_time) AS First_checkout_date, "
                "    MAX(c.End_time) AS Last_due_date, "
                "    u.First_name || ' ' || u.Last_name AS LibrarianName "
                "FROM Check_out c "
                "LEFT JOIN Copy co ON c.Copy_ID = co.ID "
                "LEFT JOIN Book b ON co.ISBN = b.ISBN "
                "LEFT JOIN Extend_magazine em ON co.EISSN = em.EISSN "
                "LEFT JOIN Magazine m ON em.ISSN = m.ISSN "
                "JOIN User_account u ON c.By_ID = u.ID "
                "WHERE c.Patron_ID = ? AND c.Is_return = FALSE "
                "GROUP BY COALESCE(b.ISBN, em.EISSN), COALESCE(b.Title, m.Title), u.First_name, u.Last_name "
                "ORDER BY First_checkout_date DESC;";
    sqlite3_stmt* stmt = nullptr;
    vector<vector<string>> Items;

    try {
        // Prepare SQL statement
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }

        // Bind parameters
        if (sqlite3_bind_int(stmt, 1, patronId) != SQLITE_OK) {
            throw runtime_error("Failed to bind parameter: " + string(sqlite3_errmsg(db)));
        }

        // Fetch data
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            vector<string> item;
            for(int i=0;i<6;i++) item.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
            if (!item.empty()) {
                Items.push_back(item);
            }
        }
    } catch (const exception& e) {
        mvprintw(1,1,"Error: %s", e.what());
        getch();
        if (stmt) sqlite3_finalize(stmt);
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    return Items;
}

//--------------Books processing Functions-----------------
vector<Book> findBooksByTitle(sqlite3* db, const string& title) {
    vector<Book> books;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ISBN FROM Book WHERE title LIKE ?;";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        string search_query = "%" + title + "%";
        sqlite3_bind_text(stmt, 1, search_query.c_str(), -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string isbn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            Book book;
            if (!book.loadFromDatabase(db, isbn)) {
                cerr << "Failed to load book details for ISBN: " << isbn << "\n";
            } else {
                books.push_back(book);
            }
        }

    } catch (const exception& e) {
        cerr << "Error in findBooksByTitle: " << e.what() << "\n";
    }

    sqlite3_finalize(stmt);
    return books;
}

vector<Book> loadAllBooks(sqlite3* db) {
    vector<Book> books;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ISBN FROM Book";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string isbn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            Book book;
            book.loadFromDatabase(db, isbn);
            books.push_back(book);
        }

    } catch (const exception& e) {
        cerr << "Error in loadAllBooksFromDatabase: " << e.what() << "\n";
    }

    sqlite3_finalize(stmt);
    return books;
}

vector<Book> loadAllBooks_title(sqlite3* db) {
    vector<Book> books;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ISBN FROM Book ORDER BY Title";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string isbn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            Book book;
            book.loadFromDatabase(db, isbn);
            books.push_back(book);
        }

    } catch (const exception& e) {
        cerr << "Error in loadAllBooksFromDatabase: " << e.what() << "\n";
    }

    sqlite3_finalize(stmt);
    return books;
}

//--------------- Infomation groups processing functions-------------
vector<Author> loadAllAuthors(sqlite3* db) {
    vector<Author> authors;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, First_name, Last_name FROM Author";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

            Author author(firstName, lastName, id);
            authors.push_back(author);
        }

    } catch (const exception& e) {
        cerr << "Error in loadAllAuthorsFromDatabase: " << e.what() << "\n";
    }

    sqlite3_finalize(stmt);
    return authors;
}

vector<Author> loadAllAuthors_name(sqlite3* db) {
    vector<Author> authors;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, First_name, Last_name FROM Author ORDER BY Last_name";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

            Author author(firstName, lastName, id);
            authors.push_back(author);
        }

    } catch (const exception& e) {
        cerr << "Error in loadAllAuthorsFromDatabase: " << e.what() << "\n";
    }

    sqlite3_finalize(stmt);
    return authors;
}

vector<Category> loadAllCategories(sqlite3* db) {
    vector<Category> categories;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, Name FROM Category";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            Category category(name, id);
            categories.push_back(category);
        }

    } catch (const exception& e) {
        cerr << "Error in loadAllCategoriesFromDatabase: " << e.what() << "\n";
    }

    sqlite3_finalize(stmt);
    return categories;
}

vector<Category> loadAllCategories_name(sqlite3* db) {
    vector<Category> categories;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT ID, Name FROM Category ORDER BY Name";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            Category category(name, id);
            categories.push_back(category);
        }

    } catch (const exception& e) {
        cerr << "Error in loadAllCategoriesFromDatabase: " << e.what() << "\n";
    }

    sqlite3_finalize(stmt);
    return categories;
}

vector<Book> findBooksByAuthor(sqlite3* db, int authorID) {
    vector<Book> books;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT Book.ISBN FROM Book "
                          "JOIN Book_author ON Book.ISBN = Book_author.ISBN "
                          "WHERE Book_author.Author_ID = ?;";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, authorID);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string isbn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            Book book;
            if (book.loadFromDatabase(db, isbn)) {
                books.push_back(book);
            }
        }

    } catch (const exception& e) {
        cerr << "Error in findBooksByAuthor: " << e.what() << "\n";
    }

    sqlite3_finalize(stmt);
    return books;
}

vector<Book> findBooksByCategory(sqlite3* db, int categoryID) {
    vector<Book> books;
    sqlite3_stmt* stmt;

    try {
        string sql = "SELECT Book.ISBN FROM Book "
                          "JOIN Book_category ON Book.ISBN = Book_category.ISBN "
                          "WHERE Book_category.Category_id = ?;";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error(sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, categoryID);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string isbn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            Book book;
            if (book.loadFromDatabase(db, isbn)) {
                books.push_back(book);
            }
        }

    } catch (const exception& e) {
        cerr << "Error in findBooksByCategory: " << e.what() << "\n";
    }

    sqlite3_finalize(stmt);
    return books;
}


//-------------Librarian Protocol Functions------------
void lendBookToPatron(sqlite3* db, int patronId, int byId, string ISBN, int numCopies) {
    try {
        // Get book price from database
            string sql = "SELECT Price FROM Book WHERE ISBN = ?;";
            sqlite3_stmt* stmt;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
            }
            sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_STATIC);

            double price = 0.0;
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                price = sqlite3_column_double(stmt, 0);
            } else {
                sqlite3_finalize(stmt);
                throw runtime_error("Failed to get book price");
            }
            sqlite3_finalize(stmt);
        // Get copies that are currently available
        sql = "SELECT ID FROM Copy WHERE ISBN = ? AND Status = 'Available' ORDER BY ID LIMIT ?;";
        vector<int> copyIds;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }

        sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, numCopies);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            copyIds.push_back(sqlite3_column_int(stmt, 0));
        }
        sqlite3_finalize(stmt);

        if (copyIds.size() < numCopies) {
            throw runtime_error("Not enough copies available");
        }

        // Create checkout records and update copy status for each copy
        for (int copyId : copyIds) {
            // Create and save checkout record
            CheckOut checkout(0, patronId, copyId, byId, false);
            if (!checkout.saveToDB(db)) {
                throw runtime_error("Failed to create checkout record");
            }

            // Update copy status to 'Checked Out'
            sql = "UPDATE Copy SET Status = 'Checked Out' WHERE ID = ?;";
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                throw runtime_error("Failed to prepare update statement");
            }
            sqlite3_bind_int(stmt, 1, copyId);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                throw runtime_error("Failed to update copy status");
            }
            sqlite3_finalize(stmt);
            // Update patron available fee
            sql = "UPDATE Patron SET Available_Fee = Available_Fee - ? WHERE ID = ?;";
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                throw runtime_error("Failed to prepare update statement");
            }
            sqlite3_bind_double(stmt, 1, price); // Decrease by book price
            sqlite3_bind_int(stmt, 2, patronId);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                throw runtime_error("Failed to update patron available fee");
            }
            sqlite3_finalize(stmt);
        }

        cout << numCopies << " copies successfully lent to patron ID " << patronId << endl;

    } catch (const exception& e) {
        cerr << "Error lending books: " << e.what() << endl;
    }
}

void expandCheckout(sqlite3* db, int patronId, const string& ISBN, int numCopies) {
    try {
        string sql = 
            "UPDATE Check_out "
            "SET End_time = DATETIME(End_time, '+14 days') "
            "WHERE ID IN ("
            "    SELECT Check_out.ID "
            "    FROM Check_out "
            "    LEFT JOIN Copy ON Check_out.Copy_ID = Copy.ID "
            "    WHERE Patron_ID = ? AND ISBN = ? AND Is_return = FALSE "
            "    ORDER BY Check_out.ID "
            "    LIMIT ?"
            ");";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }

        sqlite3_bind_int(stmt, 1, patronId);
        sqlite3_bind_text(stmt, 2, ISBN.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, numCopies);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw runtime_error("Failed to update checkout records");
        }

        sqlite3_finalize(stmt);

        cout << numCopies << " borrowed copies successfully extended for patron ID " << patronId << endl;

    } catch (const exception& e) {
        cerr << "Error extending checkout: " << e.what() << endl;
    }
}

void returnBook(sqlite3* db, int patronId, const string& ISBN, float price, int numCopies) {
    try {
        string sql = 
            "UPDATE Check_out "
            "SET Return_time = DATETIME('now','+7 hours'), Is_return = TRUE "
            "WHERE ID IN ("
            "    SELECT Check_out.ID "
            "    FROM Check_out "
            "    LEFT JOIN Copy ON Check_out.Copy_ID = Copy.ID "
            "    WHERE Patron_ID = ? AND ISBN = ? AND Is_return = FALSE "
            "    ORDER BY Check_out.ID "
            "    LIMIT ?"
            ");";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }

        sqlite3_bind_int(stmt, 1, patronId);
        sqlite3_bind_text(stmt, 2, ISBN.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, numCopies);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw runtime_error("Failed to update checkout records");
        }

        sqlite3_finalize(stmt);

        // Update copy status to 'Available'
        sql = "UPDATE Copy SET Status = 'Available' WHERE ID IN ("
              "    SELECT Copy_ID FROM Check_out "
              "    WHERE Patron_ID = ? AND ISBN = ? AND Is_return = TRUE "
              "    ORDER BY ID DESC "
              "    LIMIT ?"
              ");";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare update statement");
        }

        sqlite3_bind_int(stmt, 1, patronId);
        sqlite3_bind_text(stmt, 2, ISBN.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, numCopies);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw runtime_error("Failed to update copy status");
        }

        sqlite3_finalize(stmt);
        // Update patron available fee
        sql = "UPDATE Patron SET Available_Fee = Available_Fee + ? WHERE ID = ?;";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare update statement");
        }
        sqlite3_bind_double(stmt, 1, price*numCopies); // Decrease by book price
        sqlite3_bind_int(stmt, 2, patronId);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw runtime_error("Failed to update patron available fee");
        }
        sqlite3_finalize(stmt);
        cout << numCopies << " copies successfully returned by patron ID " << patronId << endl;

    } catch (const exception& e) {
        cerr << "Error returning books: " << e.what() << endl;
    }
}


//-------------Assistant Protocol Fuctions--------------
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first != string::npos && last != string::npos) ? str.substr(first, last - first + 1) : "";
}

vector<string> split(const string& str, char delim) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

pair<string, string> splitName(const string& fullName) {
    size_t pos = fullName.find_last_of(" ");
    if (pos == string::npos) return {fullName, ""};
    return {fullName.substr(0, pos), fullName.substr(pos + 1)};
}

bool confirmChoice() {
    char ch = getch();
    return ch == 'Y' || ch == 'y';
}

int ensureAuthorExists(sqlite3* db, const string& firstName, const string& lastName) {
    sqlite3_stmt* stmt;
    string sql = "SELECT ID FROM Author WHERE First_name = ? AND Last_name = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return id;
    }
    
    sqlite3_finalize(stmt);
    sql = "INSERT INTO Author (First_name, Last_name) VALUES (?, ?);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    int id = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);
    return id;
}

int ensureCategoryExists(sqlite3* db, const string& name) {
    sqlite3_stmt* stmt;
    string sql = "SELECT ID FROM Category WHERE Name = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return id;
    }
    
    sqlite3_finalize(stmt);
    sql = "INSERT INTO Category (Name) VALUES (?);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    int id = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);
    return id;
}

int ensurePublisherExists(sqlite3* db, const string& name) {
    sqlite3_stmt* stmt;
    string sql = "SELECT ID FROM Publisher WHERE Name = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return id;
    }
    
    sqlite3_finalize(stmt);
    sql = "INSERT INTO Publisher (Name) VALUES (?);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    int id = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);
    return id;
}

void addBooksFromCSV(sqlite3* db, const string& filename) {
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Could not open file: " + filename);
        }
        vector<Book> books;
        string line;
        // Skip header line
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            vector<string> row;
            string cell;

            // Parse CSV line
            while (getline(ss, cell, ',')) {
                row.push_back(trim(cell));
            }

            if (row.size() < 8) {
                throw runtime_error("Invalid CSV format");
            }

            // Extract data from CSV
            string isbn = row[0];
            string title = row[1];
            double price = stod(row[2]);
            vector<string> authorNames = split(row[3], '/');
            vector<string> categoryNames = split(row[4], '/');
            string publisherName = row[5];
            string date = row[6]; 
            Date publicationDate;
            publicationDate = date;
            int numCopies = stoi(row[7]);
            // Check/Add Publisher
            int publisherId = ensurePublisherExists(db, publisherName);

            // Create Book
            Book book(isbn, title, price, publisherId, publicationDate);

            // Check/Add Authors
            for (const auto& authorName : authorNames) {
                auto [firstName, lastName] = splitName(authorName);
                int authorId = ensureAuthorExists(db, firstName, lastName);
                Author author(firstName, lastName, authorId);
                book.addAuthor(author);
            }

            // Check/Add Categories
            for (const auto& catName : categoryNames) {
                int categoryId = ensureCategoryExists(db, catName);
                Category category(catName, categoryId);
                book.addCategory(category);
            }
            // Save book to database
            if (!book.saveToDatabase(db)) {
                throw runtime_error("Failed to save book: " + isbn);
            }
            else books.push_back(book);

            // Create copies
            for (int i = 0; i < numCopies; i++) {
                string sql = "INSERT INTO Copy (ISBN, Status) VALUES (?, 'Available');";
                sqlite3_stmt* stmt;
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                    throw runtime_error(sqlite3_errmsg(db));
                }
                sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_STATIC);
                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    sqlite3_finalize(stmt);
                    throw runtime_error("Failed to create copy");
                }
                sqlite3_finalize(stmt);
            }
        }

        file.close();
        mvprintw(3, 1, "Books added successfully from %s", filename.c_str());
        getch();
        clear();
        // Load and display all books
        drawBookTable(1,1,books);


    } catch (const exception& e) {
        mvprintw(3, 1, "Error processing CSV file: %s\n", e.what());
        getch();
        clear();
    }
}

void addSingleBook(sqlite3* db) {
    try {
        string isbn, title, publisherName, date;
        double price;
        vector<string> authorNames, categoryNames;
        int numCopies;

        cout << "Enter ISBN: ";
        getline(cin, isbn);
        cout << "Enter title: ";
        getline(cin, title);
        cout << "Enter price: ";
        cin >> price;
        cin.ignore();

        cout << "Enter authors (separated by '/'): ";
        string authors;
        getline(cin, authors);
        authorNames = split(authors, '/');

        cout << "Enter categories (separated by '/'): ";
        string categories;
        getline(cin, categories);
        categoryNames = split(categories, '/');

        cout << "Enter publisher name: ";
        getline(cin, publisherName);

        cout << "Enter publication date (YYYY-MM-DD): ";
        getline(cin, date);
        Date publicationDate ;
        publicationDate = date;

        cout << "Enter number of copies: ";
        cin >> numCopies;

        int publisherId = ensurePublisherExists(db, publisherName);
        Book book(isbn, title, price, publisherId, publicationDate);

        for (const auto& authorName : authorNames) {
            auto [firstName, lastName] = splitName(authorName);
            int authorId = ensureAuthorExists(db, firstName, lastName);
            Author author(firstName, lastName, authorId);
            book.addAuthor(author);
        }

        for (const auto& catName : categoryNames) {
            int categoryId = ensureCategoryExists(db, catName);
            Category category(catName, categoryId);
            book.addCategory(category);
        }

        if (!book.saveToDatabase(db)) {
            throw runtime_error("Failed to save book: " + isbn);
        }

        for (int i = 0; i < numCopies; i++) {
            string sql = "INSERT INTO Copy (ISBN, Status) VALUES (?, 'Available');";
            sqlite3_stmt* stmt;
            sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
            sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_STATIC);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }

        cout << "Book added successfully!\n";

    } catch (const exception& e) {
        cerr << "Error adding book: " << e.what() << endl;
    }
}

// Cho khung nằm giữa màn hình
void draw_box(int height, int width, const char* title) {
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;

    // Vẽ viền khung
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == 0 || i == height - 1) {
                mvprintw(start_y + i, start_x + j, "-");
            } else if (j == 0 || j == width - 1) {
                mvprintw(start_y + i, start_x + j, "|");
            }
        }
    }
    // Vẽ tiêu đề
    mvprintw(start_y, start_x + (width - strlen(title)) / 2, "%s", title);
}

// Hàm nhập mật khẩu an toàn
string custom_getpass(const string& prompt) {
    printw(prompt.c_str());
    refresh();
    string password;
    int ch;

    while (true) {
        ch = getch();
        if (ch == '\r' || ch == '\n') { // Nhấn Enter để kết thúc
            printw("\n");
            break;
        } else if (ch == 127 || ch == '\b' || ch == KEY_BACKSPACE) { // Xử lý phím Backspace
            if (!password.empty()) {
                password.pop_back(); // Xóa ký tự cuối cùng khỏi chuỗi
                int x, y;
                getyx(stdscr, y, x); // Lấy vị trí hiện tại của con trỏ
                mvprintw(y, x - 1, " "); // Ghi đè ký tự cuối bằng khoảng trắng
                move(y, x - 1); // Đưa con trỏ về vị trí đã xóa
            }
        } else {
            password += static_cast<char>(ch);
            printw("*"); // Hiển thị dấu *
        }
        refresh();
    }
    return password;
}

// Hàm đăng nhập
bool login(sqlite3* db, string& role, int& id) {
    char username[50];
    string password;
    int attempts_remaining = 3; // Số lần thử tối đa

    while (true) {
        clear();
        draw_box(12, 40, ""); // Vẽ khung với kích thước 12x40
        mvprintw((LINES - 12) / 2, (COLS - 40) / 2 + 16, "LOGIN"); // Hiển thị tiêu đề "LOGIN"
        
        mvprintw((LINES - 12) / 2 + 3, (COLS - 40) / 2 + 2, "Username: ");
        mvprintw((LINES - 12) / 2 + 5, (COLS - 40) / 2 + 2, "Password: ");
        
        move((LINES - 12) / 2 + 3, (COLS - 40) / 2 + 12); // Di chuyển con trỏ đến vị trí nhập username
        refresh();

        // Nhập Username
        echo();
        getnstr(username, sizeof(username) - 1); // Nhận input cho username
        noecho();

        // Kiểm tra xem Username có tồn tại trong database
        sqlite3_stmt* check_stmt;
        const char* check_query = "SELECT 1 FROM User_account WHERE User_name = ?";
        sqlite3_prepare_v2(db, check_query, -1, &check_stmt, nullptr);
        sqlite3_bind_text(check_stmt, 1, username, -1, SQLITE_STATIC);

        if (sqlite3_step(check_stmt) != SQLITE_ROW) {
            mvprintw((LINES - 12) / 2 + 7, (COLS - 40) / 2 + 2, "Account does not exist. Please create a new one.");
            refresh();
            sqlite3_finalize(check_stmt);
            getch(); // Chờ phím bất kỳ
            endwin();
            return false; // Dừng chương trình
        }
        sqlite3_finalize(check_stmt);

        // Nếu username tồn tại, kiểm tra mật khẩu
        while (attempts_remaining > 0) {
            move((LINES - 12) / 2 + 5, (COLS - 40) / 2 + 12); // Đưa con trỏ đến vị trí nhập mật khẩu
            clrtoeol(); // Xóa vùng nhập cũ
            refresh();

            password = custom_getpass(""); // Gọi hàm nhập mật khẩu an toàn

            // Kiểm tra thông tin đăng nhập
            sqlite3_stmt* stmt;
            const char* query = "SELECT Role, First_name, Last_name, ID FROM User_account WHERE User_name = ? AND Password = ?";
            sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
            sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                // Đăng nhập thành công
                role = (const char*)sqlite3_column_text(stmt, 0);
                string first_name = (const char*)sqlite3_column_text(stmt, 1);
                string last_name = (const char*)sqlite3_column_text(stmt, 2);
                id = sqlite3_column_int(stmt,3);
                sqlite3_finalize(stmt);

                clear();
                int center_y = LINES / 2;
                int center_x = COLS / 2;
                mvprintw(center_y - 1, center_x - (first_name.length() + last_name.length() + 20) / 2,
                         "Welcome, %s %s (%s)!", first_name.c_str(), last_name.c_str(), username);
                mvprintw(center_y, center_x - (role.length() + 27) / 2, "You have logged as a(n) %s.", role.c_str());
                refresh();
                napms(3000); // Dừng 3 giây
                return true;
            } else {
                attempts_remaining--;
                mvprintw((LINES - 12) / 2 + 7, (COLS - 40) / 2 + 2, 
                         "Invalid password. Retry in: 3 seconds.");
                mvprintw((LINES - 12) / 2 + 8, (COLS - 40) / 2 + 2, "Attempts remaining: %d", attempts_remaining);
                refresh();
                napms(3000); // Dừng 3 giây để hiển thị thông báo
                move((LINES - 12) / 2 + 7, (COLS - 40) / 2 + 2);
                clrtoeol(); // Xóa thông báo "Retry in: 3 seconds."
                move((LINES - 12) / 2 + 8, (COLS - 40) / 2 + 2);
                clrtoeol(); // Xóa thông báo "Attempts remaining."
                refresh();
            }
            sqlite3_finalize(stmt);

            // Nếu hết lần thử
            if (attempts_remaining <= 0) {
                mvprintw((LINES - 12) / 2 + 9, (COLS - 40) / 2 + 2, "Too many failed attempts. Exiting...");
                getch();
                return false;
            }
        }
    }
}

// Function to navigate the menu
int navigate_menu(const char* menu[], int n_choices) {
    int highlight = 0;
    int choice = -1;
    int c;

    while (true) {
        clear();
        draw_box(n_choices + 4, 40, "Menu"); // Vẽ khung menu
        for (int i = 0; i < n_choices; i++) {
            int x_offset = (COLS - 40) / 2 + 2; // Căn chỉnh
            int y_offset = (LINES - (n_choices + 4)) / 2 + i + 2;

            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(y_offset, x_offset, "%s", menu[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        c = getch(); // Get user input
        switch (c) {
            case KEY_UP:
                if (highlight > 0) highlight--;
                break;
            case KEY_DOWN:
                if (highlight < n_choices - 1) highlight++;
                break;
            case '\n': // User pressed Enter
                choice = highlight;
                return choice;
        }
    }
}

// Admin menu function
void admin_menu(sqlite3* db) {
    const char* menu[] = {
        "1. List accounts",
        "2. Search account",
        "3. Add account",
        "4. Edit account",
        "5. Show statistics",
        "6. Logout"
    };
    int n_choices = sizeof(menu) / sizeof(menu[0]);
    bool running = true;

    while (running) {
        int choice = navigate_menu(menu, n_choices);
        switch (choice) {
            case 0:
            {
                clear();
                drawUserTable(5,5,loadAllUsers(db));
                getch();
                clear();
                break;
            }
            case 1:
            {
                // search_account(db);
                clear();
                mvprintw(20, 1, "User Name ( or part of name):");
                char input[50];
                echo();
                getnstr(input, sizeof(input) - 1);
                noecho();
                string searchName(input);
                drawUserTable(2, 5, findUsersByName(db,searchName));
                getch();
                clear();
                break;
            }
            case 2:
            {
                // Add account
                clear();
                char firstName[50], lastName[50], userName[50], password[50], email[50];
                int roleChoice;
                mvprintw(3, 1, "Enter first name: ");
                echo();
                getnstr(firstName, sizeof(firstName) - 1);
                mvprintw(4, 1, "Enter last name: ");
                getnstr(lastName, sizeof(lastName) - 1);
                mvprintw(5, 1, "Enter email: ");
                getnstr(email, sizeof(email) - 1);
                mvprintw(6, 1, "Enter username: ");
                getnstr(userName, sizeof(userName) - 1);
                noecho();
                mvprintw(7, 1, "Enter password: ");
                strcpy(password, custom_getpass("").c_str());
                mvprintw(8, 1, "Choose role (1-Admin, 2-Assistant, 3-Librarian): ");
                echo();
                scanw("%d", &roleChoice);
                noecho();

                string role;
                switch (roleChoice) {
                    case 1:
                        role = "Admin";
                        break;
                    case 2:
                        role = "Assistant";
                        break;
                    case 3:
                        role = "Librarian";
                        break;
                    default:
                        mvprintw(9, 1, "Invalid role choice.");
                        getch();
                        clear();
                        break;
                }

                if (!role.empty()) {
                    sqlite3_stmt* stmt;
                    string sql = "INSERT INTO User_account (First_name, Last_name, User_name, Password, Email, Role) VALUES (?, ?, ?, ?, ?, ?);";
                    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                        sqlite3_bind_text(stmt, 1, firstName, -1, SQLITE_STATIC);
                        sqlite3_bind_text(stmt, 2, lastName, -1, SQLITE_STATIC);
                        sqlite3_bind_text(stmt, 3, userName, -1, SQLITE_STATIC);
                        sqlite3_bind_text(stmt, 4, password, -1, SQLITE_STATIC);
                        sqlite3_bind_text(stmt, 5, email, -1, SQLITE_STATIC);
                        sqlite3_bind_text(stmt, 6, role.c_str(), -1, SQLITE_STATIC);

                        if (sqlite3_step(stmt) == SQLITE_DONE) {
                            mvprintw(9, 1, "Account added successfully.");
                        } else {
                            mvprintw(9, 1, "Failed to add account.");
                        }
                        sqlite3_finalize(stmt);
                    } else {
                        mvprintw(9, 1, "Failed to prepare statement.");
                    }
                    getch();
                    clear();
                }
                break;
            }
            case 3:
            {
                clear();
                mvprintw(20, 1, "User Name ( or part of name):");
                char input[50];
                echo();
                getnstr(input, sizeof(input) - 1);
                noecho();
                string searchName(input);
                vector<UserAccount*> finded_users=findUsersByName(db,searchName);
                int selected = drawUserTable(2, 5, finded_users);
                if(selected==-1){
                    clear();
                    break;
                }
                UserAccount* user= finded_users[selected];
                clear();
                user->display();
                mvprintw(19, 1, "Do you want to edit this account Y/n");
                if (!confirmChoice()) {
                    clear();
                    break;
                }
                bool editing = true;
                while (editing) {
                    clear();
                    mvprintw(3, 1, "Editing account: %s", user->getUserName().c_str());
                    mvprintw(5, 1, "1. Change first name");
                    mvprintw(6, 1, "2. Change last name");
                    mvprintw(7, 1, "3. Change password");
                    mvprintw(8, 1, "4. Change role");
                    mvprintw(9, 1, "5. Delete account");
                    mvprintw(10, 1, "6. Save and exit");
                    mvprintw(11, 1, "7. Cancel and exit");
                    mvprintw(13, 1, "Choose an option: ");
                    refresh();

                    int choice = getch() - '0';
                    switch (choice) {
                        case 1:
                            mvprintw(15, 1, "Enter new first name: ");
                            echo();
                            getnstr(input, sizeof(input) - 1);
                            noecho();
                            user->Update_FirstName(input);
                            break;
                        case 2:
                            mvprintw(15, 1, "Enter new last name: ");
                            echo();
                            getnstr(input, sizeof(input) - 1);
                            noecho();
                            user->Update_LastName(input);
                            break;
                        case 3:
                            mvprintw(15, 1, "Enter new password: ");
                            echo();
                            getnstr(input, sizeof(input) - 1);
                            noecho();
                            user->Update_Password(input);
                            break;
                        case 4:
                        {
                            mvprintw(15, 1, "Choose new role (1-Admin, 2-Assistant, 3-Librarian): ");
                            echo();
                            int roleChoice;
                            string role;
                            scanw("%d", &roleChoice);
                            noecho();
                            switch (roleChoice) {
                                case 1:
                                    role = "Admin";
                                    break;
                                case 2:
                                    role = "Assistant";
                                    break;
                                case 3:
                                    role = "Librarian";
                                    break;
                                default:
                                    mvprintw(17, 1, "Invalid role choice.");
                                    getch();
                                    break;
                                Admin ad;
                                ad.Update_role(db,user->getID(),role);
                            }
                            break;
                        }
                        case 5:
                        {
                            mvprintw(15, 1, "Are you sure you want to delete this account? (Y/n): ");
                            if (getch() == 'Y' || getch() == 'y') {
                                Admin ad;
                                ad.Delete_account(db,user->getID());
                                mvprintw(17, 1, "Account deleted.");
                                getch();
                                editing = false;
                            }
                            break;
                        }
                        case 6:
                        {
                            user->updateToDatabase(db);
                            mvprintw(17, 1, "Changes saved.");
                            getch();
                            editing = false;
                            break;
                        }
                        case 7:
                        {
                            editing = false;
                            break;
                        }
                        default:
                            mvprintw(15, 1, "Invalid choice. Try again.");
                            getch();
                            break;
                    }
                }
                clear();
                break;
            }
            case 4:
                // show_statistics(db);
                mvprintw(20, 1, "Show statistics functionality not implemented...");
                getch();
                break;
            case 5:
                running = false;
                break;
            default:
                break;
        }
    }
}

// Staff menu function
void assistant_menu(sqlite3* db) {
    const char* menu[] = {
        "1. Bulk add books",
        "2. List books",
        "3. Delete a book",
        "4. Show statistics",
        "5. Logout"
    };
    int n_choices = sizeof(menu) / sizeof(menu[0]);
    bool running = true;

    while (running) {
        int choice = navigate_menu(menu, n_choices);
        switch (choice) {
            case 0:
            {
                clear();
                mvprintw(3, 1, "Enter the path to the CSV file: ");
                char input[256];
                echo();
                getnstr(input, sizeof(input) - 1);
                noecho();
                clear();
                string fileName(input);
                addBooksFromCSV(db, fileName);
                clear();
                break;
            }
            case 1: 
            {
                // list_books(db);
                drawBookTable(1, 1, loadAllBooks(db));
                mvprintw(20, 1, "Press 1 to print in Title order");
                refresh();
                int ch = getch();
                if (ch == '1') {
                    drawBookTable(1, 1, loadAllBooks_title(db));
                }
                break;
            }
            case 2: 
            {
                // Find and delete a book by title
                clear();
                mvprintw(3, 1, "Enter the title of the book to delete: ");
                char input[256];
                echo();
                getnstr(input, sizeof(input) - 1);
                noecho();
                string title(input);
                vector<Book> books = findBooksByTitle(db, title);
                int index = drawBookTable(1, 1, books);
                if (index != -1) {
                    mvprintw(19, 1, "Do you want to delete this book Y/n");
                    char ch = getch();
                    if(ch=='Y' || ch=='y') {
                        Book bookToDelete = books[index];
                        if (bookToDelete.deleteFromDatabase(db)) {
                            mvprintw(21, 1, "Book deleted successfully.");
                        } else {
                            mvprintw(21, 1, "Failed to delete the book.");
                        }
                    }
                } else {
                    mvprintw(20, 1, "No book selected.");
                }
                getch();
                clear();
                break;
            }
            case 3:
            {
                clear();
                mvprintw(1, 1, "Thong ke:");

                // Count Patrons by Role
                mvprintw(3, 1, "So luong Patrons");
                sqlite3_stmt* stmt;
                string sql = "SELECT Role, COUNT(*) AS Count FROM Patron GROUP BY Role ORDER BY Count DESC;";
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                    int row = 4;
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                        int count = sqlite3_column_int(stmt, 1);
                        mvprintw(row++, 1, "%s: %d", role.c_str(), count);
                    }
                }
                sqlite3_finalize(stmt);

                    // Count Books
                    
                    sql = "SELECT COUNT(*) AS Total_Count FROM Book;";
                    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                        if (sqlite3_step(stmt) == SQLITE_ROW) {
                            int count = sqlite3_column_int(stmt, 0);
                            mvprintw(7, 1, "Number of Books: %d", count);
                        }
                    }
                    sqlite3_finalize(stmt);

                // Count Magazines
                
                sql = "SELECT COUNT(*) AS Total_Count FROM Magazine;";
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                    if (sqlite3_step(stmt) == SQLITE_ROW) {
                        int count = sqlite3_column_int(stmt, 0);
                        mvprintw(10, 1, "Number of Magazines: %d",count);
                    }
                }
                sqlite3_finalize(stmt);

                // Count Extend Magazines
                mvprintw(13, 1, "Number of Extend Magazines: ");
                sql = "SELECT COUNT(*) AS Total_Count FROM Extend_magazine;";
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                    if (sqlite3_step(stmt) == SQLITE_ROW) {
                        int count = sqlite3_column_int(stmt, 0);
                        mvprintw(13, 1, "Number of Extend Magazines: %d", count);
                    }
                }
                sqlite3_finalize(stmt);

                mvprintw(16, 1, "Press any key to return to the menu...");
                getch();
                clear();
                break;
            }
            case 4:
                running = false;
                break;
            default:
                break;
        }
    }
}

// Librarian menu function
string getInput(size_t maxLength) {
    char input[maxLength];
    echo();
    getnstr(input, maxLength - 1);
    noecho();
    return string(input);
}


void librarian_menu(sqlite3* db, int ID) {
    const char* menu[] = {
        "1. Search for a book",
        "2. Lend book",
        "3. List borrowers",
        "4. Return book",
        "5. Expand borrowing period ",
        "6. Add patron",
        "7. Edit patron",
        "6. Exit"
    };
    int n_choices = sizeof(menu) / sizeof(menu[0]);
    bool running = true;

    while (running) {
        int choice = navigate_menu(menu, n_choices);
        switch (choice) {
            case 0:
            {
                clear();
                mvprintw(3, 1, "Enter the title of the book to find: ");
                char input[256];
                echo();
                getnstr(input, sizeof(input) - 1);
                noecho();
                string title(input);
                vector<Book> books = findBooksByTitle(db, title);
                int index = drawBookTable(1, 1, books);
                if (index != -1) {
                    Book bookToShow = books[index];
                    clear();
                    bookToShow.getInfo();
                } else {
                    mvprintw(20, 1, "No book selected.");
                }
                getch();
                clear();
                break;
            }
            case 1: 
            {
                clear();

                // Step 1: Search for the Patron
                mvprintw(3, 1, "Enter the name of borrowers (or part of it): ");
                string patronName = getInput(256);
                vector<Patron*> patrons = findPatronsByName(db, patronName);

                int patronIndex = drawPatronTable(1, 1, patrons);
                if (patronIndex == -1) {
                    mvprintw(20, 1, "No patron selected.");
                    getch();
                    clear();
                    break;
                }

                // Step 2: Display Patron Info and Confirm Lending
                Patron* selectedPatron = patrons[patronIndex];
                clear();
                selectedPatron->display();

                mvprintw(19, 1, "Do you want to lend books to this person? Y/n");
                if (!confirmChoice()) {
                    clear();
                    break;
                }

                // Step 3: Initialize vector to store selected books
                vector<Book> TakenBooks;
                bool lendMoreBooks = true;

                while (lendMoreBooks) {
                    // Step 4: Search for the Book
                    clear();
                    mvprintw(3, 1, "Enter the title of the book to find: ");
                    string bookTitle = getInput(256);
                    vector<Book> books = findBooksByTitle(db, bookTitle);

                    int bookIndex = drawBookTable(1, 1, books);
                    if (books.size() == 0 ) {continue;}
                    if (bookIndex == -1 ) {
                        mvprintw(20, 1, "No book selected.");
                        getch();
                        clear();
                        break;
                    }

                    // Step 5: Display Book Info and Confirm Lending
                    Book selectedBook = books[bookIndex];
                    clear();
                    selectedBook.getInfo();

                    mvprintw(19, 1, "Do you want to lend this book? Y/n");
                    if (!confirmChoice()) {
                        clear();
                        continue;
                    }

                    // Step 6: Determine Number of Copies to Lend
                    int maxCopies = min(selectedBook.getCopy(),(int)(selectedPatron->getAvailableFee() / selectedBook.getPrice()));
                    mvprintw(20, 1, "Enter number of copies (max of %d, 0 for none): ", maxCopies);

                    while (true) {
                        string copyInput = getInput(256);

                        if (copyInput.empty()) {
                            mvprintw(21, 1, "Input cannot be empty. Try again.");
                            getch();
                            mvprintw(21, 1, "                                 ");
                            mvprintw(20, 1, "                                                                       ");
                            mvprintw(20, 1, "Enter number of copies (max of %d, 0 for none): ", maxCopies);
                            continue;
                        }

                        int copyCount = 0;
                        try {
                            copyCount = stoi(copyInput);
                        } catch (const invalid_argument& e) {
                            mvprintw(21, 1, "Invalid input. Please enter a number.");
                            getch();
                            mvprintw(21, 1, "                                     ");
                            mvprintw(20, 1, "                                                                       ");
                            mvprintw(20, 1, "Enter number of copies (max of %d, 0 for none): ", maxCopies);
                            continue;
                        }

                        if (copyCount == 0) {
                            mvprintw(21, 1, "No copies selected for this book.");
                            getch();
                            break;
                        }

                        if (copyCount > 0 && copyCount <= maxCopies) {
                            Book Takenbook;
                            Takenbook = selectedBook;
                            float totalFee = copyCount*selectedBook.getPrice();
                            selectedPatron->updateAvailable(true,totalFee);
                            Takenbook.setCopy(copyCount);
                            TakenBooks.push_back(Takenbook);
                            mvprintw(21, 1, "Book added to lending list!");
                            getch();
                            break; 
                        } else {
                            mvprintw(21, 1, "Invalid number of copies. Must be between 0 and %d.", maxCopies);
                            getch();
                            mvprintw(21, 1, "                                                         ");
                            mvprintw(20, 1, "                                                                       ");
                            mvprintw(20, 1, "Enter number of copies (max of %d, 0 for none): ", maxCopies);
                        }
                    }


                    // Ask if the user wants to lend more books
                    clear();
                    mvprintw(19, 1, "Do you want to lend another book? Y/n");
                    lendMoreBooks = confirmChoice();
                }

                // Step 7: Finalize Lending
                clear();
                mvprintw(3, 1, "Summary of books to lend:");
                int row = 4;
                for (const auto& book : TakenBooks) {
                    mvprintw(row++, 1, "Title: %s, Copies: %d", book.getTitle().c_str(), book.getCopy());
                }

                // Confirm before finalizing
                mvprintw(row + 1, 1, "Do you want to proceed with lending these books? Y/n");
                if (confirmChoice()) {
                    for (const auto& book : TakenBooks) {
                        lendBookToPatron(db, selectedPatron->getID(), ID, book.getISBN(), book.getCopy());
                    }
                    mvprintw(row + 3, 1, "All books successfully checked out!");
                } else {
                    mvprintw(row + 3, 1, "Operation canceled. No books were checked out.");
                }

                getch();
                clear();
                break;
            }
            case 2:
            {
                clear();
                mvprintw(3, 1, "Enter the name of borrowers (or part of it): ");
                string name = getInput(256);
                vector<Patron*> patrons = findPatronsByName(db, name);
                int index = drawPatronTable(1, 1, patrons);
                if (index != -1) {
                    Patron* patron = patrons[index];
                    clear();
                    vector<vector<string>> Items = loadCheckoutByPatronID(db, patron->getID() );
                    drawCheckoutTable(1, 1, Items);
                    
                } else {
                    mvprintw(20, 1, "No patron selected.");
                }
                break;
            }
            case 3:
                {
                clear();
                mvprintw(3, 1, "Enter the name of borrowers (or part of it): ");
                string name = getInput(256);
                vector<Patron*> patrons = findPatronsByName(db, name);
                int index = drawPatronTable(1, 1, patrons);
                if (index == -1) {
                    mvprintw(20, 1, "No patron selected.");
                    getch();
                    clear();
                    break;
                }
                Patron* selectedPatron = patrons[index];
                clear();
                selectedPatron->display();
                mvprintw(19, 1, "Do you want to return books from this person Y/n");
                if (!confirmChoice()) {
                    clear();
                    break;
                }
                vector<Book> ReturnBooks;
                bool returnMoreBooks = true;

                while (returnMoreBooks) {
                    clear();
                    vector<vector<string>> Items = loadCheckoutByPatronID(db, selectedPatron->getID() );
                    int itemIndex = drawCheckoutTable(1, 1, Items);
                    if (itemIndex == -1) {
                        mvprintw(20, 1, "No book selected.");
                        getch();
                        clear();
                        break;
                    } 

                    // Step 5: Display Book Info and Confirm Lending
                    vector<string> selectedItem = Items[itemIndex];
                    Book selectedBook;
                    selectedBook.loadFromDatabase(db,selectedItem[0]);
                    mvprintw(19, 1, "Do you want to return this book? Y/n");
                    if (!confirmChoice()) {
                        clear();
                        continue;
                    }

                    // Step 6: Determine Number of Copies to Lend
                    int maxCopies = stoi(selectedItem[2]);
                    mvprintw(20, 1, "Enter number of copies (max of %d, 0 for none): ", maxCopies);
                    while (true) {
                        string copyInput = getInput(256);
                        if (copyInput.empty()) {
                            mvprintw(21, 1, "Input cannot be empty. Try again.");
                            getch();
                            mvprintw(21, 1, "                                 ");
                            mvprintw(20, 1, "                                                                       ");
                            mvprintw(20, 1, "Enter number of copies (max of %d, 0 for none): ", maxCopies);
                            continue;
                        }

                        int copyCount = 0;
                        try {
                            copyCount = stoi(copyInput);
                        } catch (const invalid_argument& e) {
                            mvprintw(21, 1, "Invalid input. Please enter a number.");
                            getch();
                            mvprintw(21, 1, "                                     ");
                            mvprintw(20, 1, "                                                                       ");
                            mvprintw(20, 1, "Enter number of copies (max of %d, 0 for none): ", maxCopies);
                            continue;
                        }

                        if (copyCount == 0) {
                            mvprintw(21, 1, "No copies selected for this book.");
                            getch();
                            break;
                        }

                        if (copyCount > 0 && copyCount <= maxCopies) {
                            Book Returnbook;
                            Returnbook = selectedBook;
                            float totalFee = copyCount*selectedBook.getPrice();
                            selectedPatron->updateAvailable(false,totalFee);
                            Returnbook.setCopy(copyCount);
                            ReturnBooks.push_back(Returnbook);
                            mvprintw(21, 1, "Book added to returning list!");
                            getch();
                            break;
                        } else {
                            mvprintw(21, 1, "Invalid number of copies. Must be between 0 and %d.", maxCopies);
                            getch();
                            mvprintw(21, 1, "                                                         ");
                            mvprintw(20, 1, "                                                                       ");
                            mvprintw(20, 1, "Enter number of copies (max of %d, 0 for none): ", maxCopies);
                        }
                    }


                    // Ask if the user wants to lend more books
                    clear();
                    mvprintw(19, 1, "Do you want to return another book? Y/n");
                    returnMoreBooks = confirmChoice();
                }

                // Step 7: Finalize Lending
                clear();
                mvprintw(3, 1, "Summary of books to be return:");
                int row = 4;
                for (const auto& book : ReturnBooks) {
                    mvprintw(row++, 1, "Title: %s, Copies: %d", book.getTitle().c_str(), book.getCopy());
                }

                // Confirm before finalizing
                mvprintw(row + 1, 1, "Do you want to proceed with returing these books? Y/n");
                if (confirmChoice()) {
                    for (const auto& book : ReturnBooks) {
                        returnBook(db, selectedPatron->getID(), book.getISBN(), book.getPrice(), book.getCopy());
                    }
                    mvprintw(row + 3, 1, "All books successfully return!");
                } else {
                    mvprintw(row + 3, 1, "Operation canceled. No books were return.");
                }
                getch();
                clear();
                break;
            }
            case 4:
            {
                clear();
                mvprintw(3, 1, "Enter the name of borrowers (or part of it): ");
                string name = getInput(256);
                vector<Patron*> patrons = findPatronsByName(db, name);
                int index = drawPatronTable(1, 1, patrons);
                if (index == -1) {
                    mvprintw(20, 1, "No patron selected.");
                    getch();
                    clear();
                    break;
                }
                Patron* selectedPatron = patrons[index];
                clear();
                selectedPatron->display();
                mvprintw(19, 1, "Do you want to expand time for this person Y/n");
                if (!confirmChoice()) {
                    clear();
                    break;
                }
                clear();
                vector<vector<string>> Items = loadCheckoutByPatronID(db, selectedPatron->getID() );
                int check = drawCheckoutTable(1, 1, Items);
                if (check == -1){
                    clear();
                    break;
                }
                DateTime start;
                start = Items[0][3];
                DateTime end;
                end = Items[0][4];
                bool able_expand = true;
                if (end-start > 26*3600*24) able_expand =false;
                if( !able_expand){
                    mvprintw(19, 1, "This person has reach the limit of expanding");
                    getch();
                    clear();
                    break;
                }
                mvprintw(19, 1, "Do you want to expand time for all of above items Y/n");
                if (!confirmChoice()) {
                    clear();
                    break;
                }
                for (int i=0;i<Items.size();i++){
                    expandCheckout(db, selectedPatron->getID(), Items[i][0], stoi(Items[i][2]));
                }
                mvprintw(20, 1, "Expand lending time successfully");
                getch();
                clear();
                break;
            }
            case 5:
            {
                // Add patron
                clear();
                char firstName[50], lastName[50], email[50], phoneNumber[15], address[255];
                int roleChoice, yearOfEnrolment;
                char faculty[255], degree[50], department[255];
                double penaltyFee=0, availableFee;
                
                mvprintw(3, 1, "Enter first name: ");
                echo();
                getnstr(firstName, sizeof(firstName) - 1);
                mvprintw(4, 1, "Enter last name: ");
                getnstr(lastName, sizeof(lastName) - 1);
                mvprintw(5, 1, "Enter email: ");
                getnstr(email, sizeof(email) - 1);
                mvprintw(6, 1, "Enter phone number: ");
                getnstr(phoneNumber, sizeof(phoneNumber) - 1);
                mvprintw(7, 1, "Enter address: ");
                getnstr(address, sizeof(address) - 1);
                mvprintw(8, 1, "Choose role (1-Student, 2-Lecturer, 3-Staff): ");
                scanw("%d", &roleChoice);
                noecho();

                string role;
                switch (roleChoice) {
                    case 1:
                        role = "Student";
                        availableFee=500;
                        mvprintw(9, 1, "Enter faculty: ");
                        echo();
                        getnstr(faculty, sizeof(faculty) - 1);
                        mvprintw(10, 1, "Enter year of enrolment: ");
                        scanw("%d", &yearOfEnrolment);
                        noecho();
                        break;
                    case 2:
                        role = "Lecturer";
                        availableFee=1000;
                        mvprintw(9, 1, "Enter faculty: ");
                        echo();
                        getnstr(faculty, sizeof(faculty) - 1);
                        mvprintw(10, 1, "Enter degree: ");
                        getnstr(degree, sizeof(degree) - 1);
                        noecho();
                        break;
                    case 3:
                        role = "Staff";
                        availableFee=2000;
                        mvprintw(9, 1, "Enter department: ");
                        echo();
                        getnstr(department, sizeof(department) - 1);
                        noecho();
                        break;
                    default:
                        mvprintw(12, 1, "Invalid role choice.");
                        getch();
                        clear();
                        break;
                }

                if (!role.empty()) {
                    if(role == "Student"){
                        Student student(0,firstName, lastName, email,"Active",phoneNumber, address, penaltyFee, availableFee, faculty, yearOfEnrolment);
                        student.saveToDatabase(db);
                    } else if (role == "Lecturer") {
                        Lecturer lecturer(0,firstName, lastName, email, "Active", phoneNumber, address, penaltyFee, availableFee, faculty, degree);
                        lecturer.saveToDatabase(db);
                    } else if (role == "Staff") {
                        Staff staff(0,firstName, lastName, email, "Active", phoneNumber, address, penaltyFee, availableFee, department);
                        staff.saveToDatabase(db);
                    }
                    mvprintw(13, 1, "Patron added successfully.");
                }
                else {
                    mvprintw(13, 1, "Failed to add patron.");
                }
                getch();
                clear();
                break;
            }
            case 6:
            {
                clear();
                mvprintw(2, 1, "Patron Name ( or part of name):");
                char input[50];
                echo();
                getnstr(input, sizeof(input) - 1);
                noecho();
                string searchName(input);
                vector<Patron*> finded_patrons = findPatronsByName(db, searchName);
                int selected = drawPatronTable(2, 5, finded_patrons);
                if (selected == -1) {
                    clear();
                    break;
                }
                Patron* patron = finded_patrons[selected];
                clear();
                patron->display();
                mvprintw(19, 1, "Do you want to edit this patron Y/n");
                if (!confirmChoice()) {
                    clear();
                    break;
                }
                bool editing = true;
                while (editing) {
                    clear();
                    mvprintw(3, 1, "Editing patron: %s %s", patron->getFirstName().c_str(), patron->getLastName().c_str());
                    mvprintw(5, 1, "1. Change first name");
                    mvprintw(6, 1, "2. Change last name");
                    mvprintw(7, 1, "3. Change email");
                    mvprintw(8, 1, "4. Change phone number");
                    mvprintw(9, 1, "5. Change address");
                    mvprintw(10, 1, "6. Delete patron");
                    mvprintw(11, 1, "7. Save and exit");
                    mvprintw(12, 1, "8. Cancel and exit");
                    mvprintw(14, 1, "Choose an option: ");
                    refresh();

                    int choice = getch() - '0';
                    switch (choice) {
                        case 1:
                            mvprintw(16, 1, "Enter new first name: ");
                            echo();
                            getnstr(input, sizeof(input) - 1);
                            noecho();
                            patron->updateFirstName(input);
                            break;
                        case 2:
                            mvprintw(16, 1, "Enter new last name: ");
                            echo();
                            getnstr(input, sizeof(input) - 1);
                            noecho();
                            patron->updateLastName(input);
                            break;
                        case 3:
                            mvprintw(16, 1, "Enter new email: ");
                            echo();
                            getnstr(input, sizeof(input) - 1);
                            noecho();
                            patron->updateEmail(input);
                            break;
                        case 4:
                            mvprintw(16, 1, "Enter new phone number: ");
                            echo();
                            getnstr(input, sizeof(input) - 1);
                            noecho();
                            patron->updatePhone(input);
                            break;
                        case 5:
                            mvprintw(16, 1, "Enter new address: ");
                            echo();
                            getnstr(input, sizeof(input) - 1);
                            noecho();
                            patron->updateAddress(input);
                            break;
                        case 6:
                        {
                            mvprintw(16, 1, "Are you sure you want to delete this patron? (Y/n): ");
                            if (getch() == 'Y' || getch() == 'y') {
                                patron->updateStatus("Inactive");
                                patron->updateToDatabase(db);
                                mvprintw(18, 1, "Patron deleted.");
                                getch();
                                editing = false;
                            }
                            break;
                        }
                        case 7:
                        {
                            patron->updateToDatabase(db);
                            mvprintw(18, 1, "Changes saved.");
                            getch();
                            editing = false;
                            break;
                        }
                        case 8:
                        {
                            editing = false;
                            break;
                        }
                        default:
                            mvprintw(16, 1, "Invalid choice. Try again.");
                            getch();
                            break;
                    }
                }
                clear();
                break;
            }
            case 7:
            {
                running = false;
                break;
            }
            default:
                break;
        }
    }
}


void updatePenalties(sqlite3* db) {
    try {

        string sql = "SELECT COUNT(*) FROM overdue_check WHERE check_date = DATE('now');";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }
        
        int count = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);

        if (count > 0) {
            cout << "Penalties have already been updated today." << endl;
            return;
        }

        // Insert today's date into the overdue_check table
        sql = "INSERT INTO overdue_check (check_date) VALUES (DATE('now'));";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare insert statement: " + string(sqlite3_errmsg(db)));
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw runtime_error("Failed to insert check date: " + string(sqlite3_errmsg(db)));
        }
        sqlite3_finalize(stmt);
        sql = 
            "SELECT Patron_ID, SUM(b.Price * 0.1) AS Penalty "
            "FROM Check_out c "
            "JOIN Copy co ON c.Copy_ID = co.ID "
            "JOIN Book b ON co.ISBN = b.ISBN "
            "WHERE c.Is_return = FALSE AND c.End_time < DATETIME('now') "
            "GROUP BY c.Patron_ID;";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int patronId = sqlite3_column_int(stmt, 0);
            double penalty = sqlite3_column_double(stmt, 1);

            string updateSql = "UPDATE Patron SET Penalty_Fee = Penalty_Fee + ? WHERE ID = ?;";
            sqlite3_stmt* updateStmt;
            if (sqlite3_prepare_v2(db, updateSql.c_str(), -1, &updateStmt, nullptr) != SQLITE_OK) {
                throw runtime_error("Failed to prepare update statement: " + string(sqlite3_errmsg(db)));
            }

            sqlite3_bind_double(updateStmt, 1, penalty);
            sqlite3_bind_int(updateStmt, 2, patronId);

            if (sqlite3_step(updateStmt) != SQLITE_DONE) {
                throw runtime_error("Failed to update penalty fee: " + string(sqlite3_errmsg(db)));
            }

            sqlite3_finalize(updateStmt);
        }

        sqlite3_finalize(stmt);
        cout << "Penalties updated successfully." << endl;

    } catch (const exception& e) {
        cerr << "Error updating penalties: " << e.what() << endl;
    }
}
int main(){
    sqlite3* db;
    if (sqlite3_open("Data/Database.db", &db) != SQLITE_OK) {
    std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
    return -1;
    }
    updatePenalties(db);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    string role;
    int ID;
    while (true) {
        if (!login(db, role, ID)) {
            endwin();
            sqlite3_close(db);
            return 1;
        }

        if (role == "Admin") {
            admin_menu(db);
        } else if (role == "Assistant") {
            assistant_menu(db);
        } else if (role == "Librarian") {
            librarian_menu(db, ID);
        } else {
            mvprintw(20, 1, "Unknown role. Exiting...");
            getch();
        }
        mvprintw(20, 1, "Press ESC to exit or any other key to login again...");
        int ch = getch();
        if (ch == 27) { // ESC key
            break;
        }
    }
    endwin();
    sqlite3_close(db);
    return 0;
}