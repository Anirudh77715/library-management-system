#include "user.hpp"
#include "utils.hpp"
#include <sstream>
#include <iostream>

// Constructor implementation
User::User(const std::string& name, const std::string& email, 
           const std::string& password, UserRole role)
    : id(Utils::generateUniqueId())
    , name(name)
    , email(email)
    , password(password)
    , role(role) {
    std::cout << "Creating user: " << name << " (Role: " << 
        (role == UserRole::STUDENT ? "Student" : 
         role == UserRole::FACULTY ? "Faculty" : "Librarian") << ")\n";
}

std::string User::serialize() const {
    std::stringstream ss;
    ss << static_cast<int>(role) << "|" << id << "|" << name << "|" 
       << email << "|" << password << "|" << account.serialize();
    return ss.str();
}

User* User::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string roleStr, id, name, email, password, accountData;

    std::getline(ss, roleStr, '|');
    std::getline(ss, id, '|');
    std::getline(ss, name, '|');
    std::getline(ss, email, '|');
    std::getline(ss, password, '|');
    std::getline(ss, accountData);

    UserRole role = static_cast<UserRole>(std::stoi(roleStr));
    User* user = nullptr;

    std::cout << "Deserializing user: " << name << " (Role: " << roleStr << ")\n";

    switch(role) {
        case UserRole::STUDENT:
            user = new Student(name, email, password);
            break;
        case UserRole::FACULTY:
            user = new Faculty(name, email, password);
            break;
        case UserRole::LIBRARIAN:
            user = new Librarian(name, email, password);
            break;
    }

    if (user) {
        user->id = id;
        user->account = Account::deserialize(accountData);
    }

    return user;
}

// Student implementation
Student::Student(const std::string& name, const std::string& email, 
                const std::string& password)
    : User(name, email, password, UserRole::STUDENT) {}

int Student::getMaxBooks() const { return 3; }
int Student::getMaxDays() const { return 15; }
double Student::calculateFine(int daysOverdue) const {
    return daysOverdue > 0 ? daysOverdue * 10.0 : 0.0;
}

bool Student::borrowBook(const std::string& bookId) {
    if (getAccount().getCurrentlyBorrowedBooks().size() >= getMaxBooks()) {
        std::cout << "Student has reached maximum books limit.\n";
        return false;
    }
    if (getAccount().hasFine()) {
        std::cout << "Student has unpaid fines.\n";
        return false;
    }
    return getAccount().addBorrowedBook(bookId);
}

bool Student::returnBook(const std::string& bookId) {
    return getAccount().returnBook(bookId);
}

// Faculty implementation
Faculty::Faculty(const std::string& name, const std::string& email, 
                const std::string& password)
    : User(name, email, password, UserRole::FACULTY) {}

int Faculty::getMaxBooks() const { return 5; }
int Faculty::getMaxDays() const { return 30; }
double Faculty::calculateFine(int) const { return 0.0; }  // No fines for faculty

bool Faculty::borrowBook(const std::string& bookId) {
    if (getAccount().getCurrentlyBorrowedBooks().size() >= getMaxBooks()) {
        std::cout << "Faculty has reached maximum books limit.\n";
        return false;
    }
    return getAccount().addBorrowedBook(bookId);
}

bool Faculty::returnBook(const std::string& bookId) {
    return getAccount().returnBook(bookId);
}

// Librarian implementation
Librarian::Librarian(const std::string& name, const std::string& email, 
                     const std::string& password)
    : User(name, email, password, UserRole::LIBRARIAN) {}

int Librarian::getMaxBooks() const { return 0; }  // Cannot borrow books
int Librarian::getMaxDays() const { return 0; }   // Cannot borrow books
double Librarian::calculateFine(int) const { return 0.0; }

bool Librarian::borrowBook(const std::string&) {
    std::cout << "Librarians cannot borrow books.\n";
    return false;
}

bool Librarian::returnBook(const std::string&) {
    std::cout << "Librarians cannot return books.\n";
    return false;
}