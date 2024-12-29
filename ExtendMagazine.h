#include <string>
#include <iostream>
#include <sqlite\sqlite3.h>

class ExtendMagazine {
private:
    std::string EISSN;
    std::string ISSN;
    int volume;
    int issue;
    std::string publication_date;
    std::string special_edition;

public:
    // Constructor
    ExtendMagazine(const std::string& EISSN = "", const std::string& ISSN = "", int volume = 0, int issue = 0,
                   const std::string& publication_date = "", const std::string& special_edition = "")
        : EISSN(EISSN), ISSN(ISSN), volume(volume), issue(issue), publication_date(publication_date), special_edition(special_edition) {}

    // Getters
    std::string getEISSN() const { return EISSN; }
    std::string getISSN() const { return ISSN; }
    int getVolume() const { return volume; }
    int getIssue() const { return issue; }
    std::string getPublicationDate() const { return publication_date; }
    std::string getSpecialEdition() const { return special_edition; }

    // Setters
    void setEISSN(const std::string& eissn) { EISSN = eissn; }
    void setISSN(const std::string& issn) { ISSN = issn; }
    void setVolume(int vol) { volume = vol; }
    void setIssue(int iss) { issue = iss; }
    void setPublicationDate(const std::string& date) { publication_date = date; }
    void setSpecialEdition(const std::string& edition) { special_edition = edition; }

    // Load from DB
    bool loadFromDB(sqlite3* db);

    // Save to DB
    bool saveToDB(sqlite3* db);

    // Display magazine details
    void display() const;
};
