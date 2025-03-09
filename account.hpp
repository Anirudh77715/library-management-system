#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>
#include <vector>
#include <ctime>
#include <iostream>

// Encapsulation: Struct for record keeping
struct BorrowRecord {
    std::string bookId;
    std::time_t borrowDate;
    std::time_t returnDate;

    void print() const {
        std::cout << "Book ID: " << bookId << std::endl;
        std::cout << "Borrow Date: " << std::ctime(&borrowDate);
        if (returnDate > 0) {
            std::cout << "Return Date: " << std::ctime(&returnDate);
        } else {
            std::cout << "Return Date: Not returned yet" << std::endl;
        }
    }
};

class Account {
private:
    // Encapsulation: Private data members
    std::vector<std::string> currentlyBorrowedBooks;
    std::vector<BorrowRecord> borrowHistory;
    double outstandingFine;

public:
    Account();

    // Encapsulation: Public methods for controlled access
    bool addBorrowedBook(const std::string& bookId);
    bool returnBook(const std::string& bookId);
    bool hasFine() const { return outstandingFine > 0; }
    void addFine(double amount) { outstandingFine += amount; }
    void clearFine() { outstandingFine = 0; }
    double getFine() const { return outstandingFine; }

    // Print methods for each attribute
    void printCurrentlyBorrowedBooks() const {
        std::cout << "Currently Borrowed Books:" << std::endl;
        for (const auto& bookId : currentlyBorrowedBooks) {
            std::cout << "- " << bookId << std::endl;
        }
    }

    void printBorrowHistory() const {
        std::cout << "Borrowing History:" << std::endl;
        for (const auto& record : borrowHistory) {
            record.print();
            std::cout << "------------------------" << std::endl;
        }
    }

    void printFine() const {
        std::cout << "Outstanding Fine: ₹" << outstandingFine << std::endl;
    }

    // Print all details
    void printDetails() const {
        printCurrentlyBorrowedBooks();
        printBorrowHistory();
        printFine();
    }

    // Getters for borrowing information
    const std::vector<std::string>& getCurrentlyBorrowedBooks() const { 
        return currentlyBorrowedBooks; 
    }
    const std::vector<BorrowRecord>& getBorrowHistory() const { 
        return borrowHistory; 
    }

    // Update methods
    void updateFine(double newFine) { outstandingFine = newFine; }
    void removeBorrowedBook(const std::string& bookId);
    void clearBorrowHistory() { borrowHistory.clear(); }

    // Serialization
    std::string serialize() const;
    static Account deserialize(const std::string& data);
};

#endif