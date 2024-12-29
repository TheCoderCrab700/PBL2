DROP TABLE IF EXISTS User_account;
DROP TABLE IF EXISTS Publisher;
DROP TABLE IF EXISTS Book;
DROP TABLE IF EXISTS Magazine;
DROP TABLE IF EXISTS Extend_magazine;
DROP TABLE IF EXISTS Author;
DROP TABLE IF EXISTS Category;
DROP TABLE IF EXISTS Copy;
DROP TABLE IF EXISTS Patron;
DROP TABLE IF EXISTS Student;
DROP TABLE IF EXISTS Lecturer;
DROP TABLE IF EXISTS Staff;
DROP TABLE IF EXISTS Book_author;
DROP TABLE IF EXISTS Book_category;
DROP TABLE IF EXISTS Magazine_category;
DROP TABLE IF EXISTS Check_out;
DROP TABLE IF EXISTS Overdue_check;

-- --------------------------------------------------------------------------------
-- Create Table struct of User_account
CREATE TABLE User_account (
    ID          INTEGER         PRIMARY KEY     AUTOINCREMENT,
    First_name  VARCHAR(255)    NOT NULL,
    Last_name   VARCHAR(255)    NOT NULL,
    Role        VARCHAR(50),
    User_name   VARCHAR(255)    NOT NULL        UNIQUE,
    Password    VARCHAR(255)    NOT NULL,
    Email       VARCHAR(255)    UNIQUE
);
INSERT INTO User_account (First_name, Last_name, Role, User_name, Password, Email) VALUES
('Huynh Phuoc', 'Loc', 'Admin', 'phuocloc', 'password123', 'loc.huynh@example.com'),
('Nguyen Trong', 'Hieu', 'Admin', 'tronghieu', 'password123', 'hieu.nguyen@example.com'),
('Vo Quoc', 'Minh', 'Admin', 'quocminh', 'password123', 'minh.vo@example.com'),
('Nguyen Van', 'Tri', 'Librarian', 'vantri', 'password123', 'tri.nguyen@example.com'),
('Nguyen Van', 'Tuan', 'Assistant', 'vantuan', 'password123', 'tuan.nguyen@example.com'),
('Le Thi', 'Oanh', 'Librarian', 'thioanh', 'password123', 'oanh.le@example.com'),
('Tran Van', 'An', 'Assistant', 'vanan', 'password123', 'an.tran@example.com'),
('Pham Thi', 'Hoa', 'Librarian', 'thihoa', 'password123', 'hoa.pham@example.com'),
('Nguyen Hoang', 'Khang', 'Librarian', 'hoangkhang', 'password123', 'khang.nguyen@example.com'),
('Dang Kim', 'Phuong', 'Assistant', 'kimphuong', 'password123', 'phuong.dang@example.com');

-- --------------------------------------------------------------------------------
-- Create Table struct of Publisher
CREATE TABLE Publisher (
    ID          INTEGER         PRIMARY KEY     AUTOINCREMENT,
    Name        VARCHAR(255)    NOT NULL        UNIQUE
);
INSERT INTO Publisher (Name) VALUES
('NXB Kim Dong'),
('NXB Tre'),
('NXB Giao duc Viet Nam'),
('NXB Van hoa Van nghe'),
('NXB Lao Dong'),
('NXB Phu Nu Viet Nam'),
('NXB Chinh Tri Quoc Gia Su That'),
('NXB Tong Hop TP.HCM'),
('NXB Khoa Hoc va Ky Thuat'),
('NXB Hoi Nha Van');

-- --------------------------------------------------------------------------------
-- Create Table struct of Book
CREATE TABLE Book (
    ISBN                VARCHAR(50)     PRIMARY KEY,
    Title               VARCHAR(255)    NOT NULL,
    Price               DECIMAL(10, 2)  NOT NULL,
    Publisher_ID        INTEGER         NOT NULL,
    Publication_date    DATE            NOT NULL, 
    FOREIGN KEY (Publisher_ID) REFERENCES Publisher(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
INSERT INTO Book (ISBN, Title, Price, Publisher_ID, Publication_date) VALUES
('9781234567890', 'Lap trinh Python co ban', 150.00, 1, '2022-05-15'),
('9780987654321', 'Giai tich 1', 200.00, 2, '2021-03-10'),
('9784567891234', 'Nhung nguyen ly co ban cua Vat Ly', 250.00, 3, '2020-09-05'),
('9783456789123', 'Giao trinh Tieng Anh co ban', 180.00, 4, '2023-01-20'),
('9785678912345', 'Van hoc Viet Nam hien dai', 220.00, 5, '2019-11-30'),
('9786789123456', 'Kinh te hoc vi mo', 300.00, 6, '2023-07-25'),
('9787891234567', 'Cuon theo chieu gio', 350.00, 7, '2022-02-18'),
('9788912345678', 'Phat trien ung dung web voi JavaScript', 280.00, 8, '2021-06-12'),
('9789123456789', 'Ky thuat so', 240.00, 9, '2020-12-01'),
('9781239876543', 'Giao duc hoc', 210.00, 10, '2023-10-15');

-- --------------------------------------------------------------------------------
-- Create Table struct of Magazine
CREATE TABLE Magazine (
    ISSN                VARCHAR(50)     PRIMARY KEY,
    Title               VARCHAR(255)    NOT NULL,
    Price               DECIMAL(10, 2)  NOT NULL,
    Publisher_ID        INTEGER         NOT NULL,
    Established_date    DATE            NOT NULL,
    FOREIGN KEY (Publisher_ID) REFERENCES Publisher(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
INSERT INTO Magazine (ISSN, Title, Price, Publisher_ID, Established_date) VALUES
('9771234567890', 'Tap chi Khoa hoc va Cong nghe', 50.00, 1, '2000-06-01'),
('9770987654321', 'Tap chi Van hoc va Nghe thuat', 45.00, 2, '1995-04-10'),
('9774567891234', 'Tap chi Giao duc Viet Nam', 55.00, 3, '2010-09-15'),
('9773456789123', 'Tap chi The thao va Suc khoe', 40.00, 4, '2015-03-20'),
('9775678912345', 'Tap chi Kinh te va Phat trien', 60.00, 5, '2005-12-01'),
('9776789123456', 'Tap chi Cong nghe thong tin', 75.00, 6, '2018-07-25'),
('9777891234567', 'Tap chi Moi truong va Xa hoi', 65.00, 7, '2012-02-18'),
('9778912345678', 'Tap chi Van hoa the gioi', 70.00, 8, '2008-11-10'),
('9779123456789', 'Tap chi Y hoc hien dai', 85.00, 9, '2016-08-05'),
('9771239876543', 'Tap chi Nha khoa va Khoa hoc tre', 72.00, 10, '2021-01-30');

-- --------------------------------------------------------------------------------
-- Create Table struct of Extend_magazine
CREATE TABLE Extend_magazine (
    EISSN               VARCHAR(50)     PRIMARY KEY, 
    ISSN                VARCHAR(50)     NOT NULL,    
    Volume              INTEGER         NOT NULL,    
    Issue               INTEGER         NOT NULL,    
    Publication_date    DATE            NOT NULL,    
    Special_edition     VARCHAR(255),
    FOREIGN KEY (ISSN) REFERENCES Magazine(ISSN)
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
INSERT INTO Extend_magazine (EISSN, ISSN, Volume, Issue, Publication_date, Special_edition) VALUES
('9771234567891', '9771234567890', 1, 1, '2023-01-01', 'Special Science Issue'),
('9770987654322', '9770987654321', 1, 2, '2023-02-15', NULL),
('9774567891235', '9774567891234', 2, 1, '2023-03-10', 'Education Trends 2023'),
('9773456789124', '9773456789123', 3, 1, '2023-04-20', 'Sports Highlights'),
('9775678912346', '9775678912345', 2, 3, '2023-05-05', NULL),
('9776789123457', '9776789123456', 4, 1, '2023-06-15', 'Tech Innovations'),
('9777891234568', '9777891234567', 5, 2, '2023-07-25', NULL),
('9778912345679', '9778912345678', 3, 4, '2023-08-10', 'Cultural Exchange Edition'),
('9779123456790', '9779123456789', 2, 5, '2023-09-05', NULL),
('9771239876544', '9771239876543', 1, 6, '2023-10-20', 'Young Scientists Edition');

-- --------------------------------------------------------------------------------
-- Create Table struct of Author
CREATE TABLE Author (
    ID          INTEGER         PRIMARY KEY     AUTOINCREMENT,
    First_name  VARCHAR(255)    NOT NULL,
    Last_name   VARCHAR(255)    NOT NULL        UNIQUE
);
INSERT INTO Author (First_name, Last_name) VALUES
('Nguyen Van', 'A'),
('Tran Thi', 'B'),
('Le Van', 'C'),
('Pham Quang', 'D'),
('Hoang Minh', 'E'),
('Vo Thi', 'F'),
('Nguyen Huu', 'G'),
('Do Van', 'H'),
('Bui Quoc', 'I'),
('Dang Thi', 'J');

-- --------------------------------------------------------------------------------
-- Create Table struct of Category
CREATE TABLE Category (
    ID           INTEGER        PRIMARY KEY     AUTOINCREMENT,
    Name         VARCHAR(255)   NOT NULL        UNIQUE
);
INSERT INTO Category (Name) VALUES
('Khoa hoc'),
('Van hoc'),
('Giao duc'),
('Ky thuat'),
('Kinh te'),
('Y hoc'),
('Cong nghe'),
('Moi truong'),
('Van hoa'),
('The thao');

-- --------------------------------------------------------------------------------
-- Create Table struct of Copy
CREATE TABLE Copy (
    ID          INTEGER         PRIMARY KEY     AUTOINCREMENT,
    ISBN        VARCHAR(50),   -- Book reference
    EISSN       VARCHAR(50),   -- Magazine reference
    Status      VARCHAR(50)     NOT NULL,
    FOREIGN KEY (ISBN) REFERENCES Book(ISBN)
        ON UPDATE CASCADE 
        ON DELETE CASCADE,
    FOREIGN KEY (EISSN) REFERENCES Extend_magazine(EISSN)
        ON UPDATE CASCADE 
        ON DELETE CASCADE,
    CHECK ((ISBN IS NOT NULL AND EISSN IS NULL) OR (EISSN IS NOT NULL AND ISBN IS NULL))
);
INSERT INTO Copy (ISBN, EISSN, Status) VALUES
('9781234567890', NULL, 'Available'), 
('9780987654321', NULL, 'Checked Out'),
(NULL, '9771234567891', 'Available'),
('9784567891234', NULL, 'Damaged'),
(NULL, '9770987654322', 'Available'),
('9783456789123', NULL, 'Checked Out'),
('9783456789123', NULL, 'Checked Out'),
('9783456789123', NULL, 'Checked Out'),
('9783456789123', NULL, 'Checked Out'),
('9783456789123', NULL, 'Checked Out'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9783456789123', NULL, 'Available'),
('9785678912345', NULL, 'Lost'),
(NULL, '9774567891235', 'Checked Out'),
(NULL, '9776789123457', 'Damaged'),
('9787891234567', NULL, 'Checked Out'),
('9787891234567', NULL, 'Checked Out'),
('9787891234567', NULL, 'Checked Out'),
('9787891234567', NULL, 'Checked Out'),
('9787891234567', NULL, 'Checked Out'),
('9787891234567', NULL, 'Checked Out'),
('9787891234567', NULL, 'Available'),
('9787891234567', NULL, 'Available'),
('9787891234567', NULL, 'Available'),
('9787891234567', NULL, 'Available'),
('9787891234567', NULL, 'Available'),
('9787891234567', NULL, 'Available'),
('9787891234567', NULL, 'Available'),
('9787891234567', NULL, 'Available'),
('9787891234567', NULL, 'Available');

-- --------------------------------------------------------------------------------
-- Create Table struct of Patron
CREATE TABLE Patron (
    ID                  INTEGER         PRIMARY KEY AUTOINCREMENT,
    First_name          VARCHAR(255)    NOT NULL,
    Last_name           VARCHAR(255)    NOT NULL,
    Email               VARCHAR(255)    UNIQUE,
    Status              VARCHAR(50)     NOT NULL,
    Phone_number        VARCHAR(15),
    Address             VARCHAR(255),
    Penalty_Fee         DECIMAL(10, 2)  NOT NULL,
    Available_Fee       DECIMAL(10, 2)  NOT NULL,
    Role                VARCHAR(50)
);
INSERT INTO Patron (First_name, Last_name, Email, Status, Phone_number, Address, Penalty_Fee, Available_Fee, Role) VALUES
('Nguyen Van', 'A', 'nguyenvana@example.com', 'Active'  , '0123456789', 'Ha Noi'     ,  0.00,  500.00, 'Student'),
('Tran Thi'  , 'B', 'tranthib@example.com'  , 'Active'  , '0987654321', 'Ho Chi Minh', 20.00, 1000.00, 'Lecturer'),
('Le Van'    , 'C', 'levanc@example.com'    , 'Inactive', '0334567890', 'Da Nang'    ,  0.00, 2000.00, 'Staff'),
('Pham Thi'  , 'D', 'phamthid@example.com'  , 'Active'  , '0912345678', 'Hue'        , 50.00,  500.00, 'Student'),
('Hoang Van' , 'E', 'hoangvane@example.com' , 'Active'  , '0912895678', 'Can Tho'    , 10.00, 1000.00, 'Lecturer'),
('Vo Thi'    , 'F', 'vothif@example.com'    , 'Active'  , '0701234567', 'Hai Phong'  ,  0.00, 2000.00, 'Staff'),
('Do Van'    , 'G', 'dovang@example.com'    , 'Inactive', '0910985678', 'Ha Giang'   , 30.00,  500.00, 'Student'),
('Dang Thi'  , 'H', 'dangthih@example.com'  , 'Active'  , '0789012345', 'Da Lat'     ,  0.00, 1000.00, 'Lecturer'),
('Ngo Van'   , 'I', 'ngovani@example.com'   , 'Active'  , '0908765432', 'Quang Ninh' , 20.00, 2000.00, 'Staff'),
('Bui Thi'  , 'K', 'buithik@example.com'   , 'Inactive', '0356789123', 'Ninh Binh'  , 10.00,  500.00, 'Student');

-- --------------------------------------------------------------------------------
-- Create Table struct of Student
CREATE TABLE Student (
    Patron_ID           INTEGER         PRIMARY KEY,
    Faculty             VARCHAR(255)    NOT NULL,
    Year_of_Enrolment   INTEGER         NOT NULL,
    FOREIGN KEY (Patron_ID) REFERENCES Patron(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
INSERT INTO Student (Patron_ID, Faculty, Year_of_Enrolment) VALUES
(1, 'Cong nghe thong tin', 2020),
(4, 'Kinh te', 2019),
(7, 'Ky thuat', 2021),
(10, 'Khoa hoc xa hoi', 2018);

-- --------------------------------------------------------------------------------
-- Create Table struct of Lecturer
CREATE TABLE Lecturer (
    Patron_ID           INTEGER         PRIMARY KEY,
    Faculty             VARCHAR(255)    NOT NULL,
    Degree              VARCHAR(50)     NOT NULL, 
    FOREIGN KEY (Patron_ID) REFERENCES Patron(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
INSERT INTO Lecturer (Patron_ID, Faculty, Degree) VALUES
(2, 'Cong nghe thong tin', 'TS'),
(5, 'Kinh te', 'PGS TS'),
(8, 'Y duoc', 'GS TS');

-- --------------------------------------------------------------------------------
-- Create Table struct of Staff
CREATE TABLE Staff (
    Patron_ID           INTEGER         PRIMARY KEY,
    Department          VARCHAR(255)    NOT NULL,
    FOREIGN KEY (Patron_ID) REFERENCES Patron(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
INSERT INTO Staff (Patron_ID, Department) VALUES
(3, 'Phong Quan tri'),
(6, 'Phong Dao tao'),
(9, 'Phong Ke toan');
-- --------------------------------------------------------------------------------
-- Create Table struct of Book_author
CREATE TABLE Book_author (
    ISBN                VARCHAR(50)     NOT NULL,
    Author_ID           INTEGER         NOT NULL,
    PRIMARY KEY (ISBN, Author_ID),
    FOREIGN KEY (ISBN) REFERENCES Book(ISBN)
        ON UPDATE CASCADE 
        ON DELETE CASCADE,
    FOREIGN KEY (Author_ID) REFERENCES Author(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
INSERT INTO Book_author (ISBN, Author_ID) VALUES
('9781234567890', 1),
('9780987654321', 2),
('9784567891234', 3),
('9783456789123', 4),
('9785678912345', 5),
('9786789123456', 6),
('9787891234567', 7),
('9788912345678', 8),
('9789123456789', 9),
('9781239876543', 10),
('9781234567890', 3), 
('9784567891234', 4),
('9785678912345', 2);

-- --------------------------------------------------------------------------------
-- Create Table struct of Book_category
CREATE TABLE Book_category (
    Category_id         INTEGER         NOT NULL,
    ISBN                VARCHAR(50)     NOT NULL,
    PRIMARY KEY (Category_id, ISBN),
    FOREIGN KEY (Category_id) REFERENCES Category(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE,
    FOREIGN KEY (ISBN) REFERENCES Book(ISBN)
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
INSERT INTO Book_category (Category_id, ISBN) VALUES
(1, '9781234567890'),
(2, '9780987654321'),
(3, '9784567891234'),
(4, '9783456789123'),
(5, '9785678912345'),
(6, '9786789123456'),
(7, '9787891234567'),
(8, '9788912345678'),
(9, '9789123456789'),
(10, '9781239876543'),
(4, '9781234567890'),
(1, '9784567891234'),
(9, '9785678912345');

-- --------------------------------------------------------------------------------
-- Create Table struct of Magazine_category
CREATE TABLE Magazine_category (
    Category_ID         INTEGER         NOT NULL,
    ISSN                VARCHAR(50)     NOT NULL,
    PRIMARY KEY (Category_ID, ISSN),
    FOREIGN KEY (Category_ID) REFERENCES Category(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE,
    FOREIGN KEY (ISSN) REFERENCES Magazine(ISSN)
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
INSERT INTO Magazine_category (Category_ID, ISSN) VALUES
(1, '9771234567890'), 
(2, '9770987654321'), 
(3, '9774567891234'), 
(4, '9773456789123'), 
(5, '9775678912345'),
(6, '9776789123456'), 
(7, '9777891234567'),
(8, '9778912345678'), 
(9, '9779123456789'), 
(10, '9771239876543'),
(4, '9771234567890'), 
(1, '9775678912345');

-- --------------------------------------------------------------------------------
-- Create Table struct of Check_out
CREATE TABLE Check_out (
    ID                  INTEGER         PRIMARY KEY    AUTOINCREMENT,
    Start_time          DATETIME        NOT NULL,
    End_time            DATETIME        NOT NULL,
    Return_time         DATETIME,
    Patron_ID           INTEGER         NOT NULL,
    Copy_ID             INTEGER         NOT NULL,
    By_ID               INTEGER         NOT NULL,
    Is_return           BOOLEAN         NOT NULL,
    FOREIGN KEY (Patron_ID) REFERENCES Patron(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE,
    FOREIGN KEY (Copy_ID) REFERENCES Copy(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE,
    FOREIGN KEY (By_ID) REFERENCES User_account(ID)
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
INSERT INTO Check_out (Start_time, End_time, Return_time, Patron_ID, Copy_ID, By_ID, Is_return) VALUES
('2024-11-01 10:00:00', '2024-11-15 10:00:00', '2024-11-14 09:00:00', 1, 1, 4, 1),
('2024-11-03 14:30:00', '2024-11-17 14:30:00', NULL,                  2, 2, 9, 0),
('2024-11-05 09:00:00', '2024-11-19 09:00:00', '2024-11-18 16:00:00', 4, 3, 6, 1),
('2024-11-06 16:45:00', '2024-11-20 16:45:00', NULL,                  5, 4, 6, 0),
('2024-11-07 11:15:00', '2024-11-21 11:15:00', '2024-11-20 10:00:00', 6, 5, 8, 1),
('2024-11-08 13:00:00', '2024-11-22 13:00:00', NULL,                  8, 6, 9, 0),
('2024-11-09 15:30:00', '2024-11-23 15:30:00', '2024-11-22 14:45:00', 9, 7, 8, 1),
('2024-11-10 08:15:00', '2024-11-24 08:15:00', NULL,                  10, 8, 6, 0),
('2024-11-11 09:30:00', '2024-11-25 09:30:00', '2024-11-24 16:00:00', 3, 9, 9, 1),
('2024-11-12 10:00:00', '2024-11-26 10:00:00', NULL,                  7, 10, 4, 0);




-- --------------------------------------------------------------------------------
-- Create Table struct of Overdue_check
CREATE TABLE Overdue_check (
    ID                  INTEGER         PRIMARY KEY AUTOINCREMENT ,
    Check_date          DATE            NOT NULL
);
INSERT INTO Overdue_check (Check_date) VALUES
('2024-11-18'),
('2024-11-21'),
('2024-11-23'),
('2024-11-25'),
('2024-11-27');