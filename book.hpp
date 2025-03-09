#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>
#include <iostream>

// Encapsulation: Status as enum class for type safety
enum class BookStatus {
    AVAILABLE,
    BORROWED,
    RESERVED
};

class Book {
private:
    // Encapsulation: Private data members
    std::string id;
    std::string title;
    std::string author;
    std::string publisher;
    int year;
    std::string isbn;
    BookStatus status;

public:
    // Constructor
    Book(const std::string& title, const std::string& author, 
         const std::string& publisher, int year, const std::string& isbn);

    // Encapsulation: Getters
    std::string getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getPublisher() const { return publisher; }
    int getYear() const { return year; }
    std::string getIsbn() const { return isbn; }
    BookStatus getStatus() const { return status; }

    // Print methods for each attribute
    void printId() const { std::cout << "Book ID: " << id << std::endl; }
    void printTitle() const { std::cout << "Title: " << title << std::endl; }
    void printAuthor() const { std::cout << "Author: " << author << std::endl; }
    void printPublisher() const { std::cout << "Publisher: " << publisher << std::endl; }
    void printYear() const { std::cout << "Year: " << year << std::endl; }
    void printIsbn() const { std::cout << "ISBN: " << isbn << std::endl; }
    void printStatus() const { 
        std::cout << "Status: " << 
            (status == BookStatus::AVAILABLE ? "Available" : 
             status == BookStatus::BORROWED ? "Borrowed" : "Reserved") << std::endl; 
    }

    // Update methods for attributes
    void updateTitle(const std::string& newTitle) { title = newTitle; }
    void updateAuthor(const std::string& newAuthor) { author = newAuthor; }
    void updatePublisher(const std::string& newPublisher) { publisher = newPublisher; }
    void updateYear(int newYear) { year = newYear; }
    void updateIsbn(const std::string& newIsbn) { isbn = newIsbn; }
    void setStatus(BookStatus newStatus) { status = newStatus; }

    // Print all details
    void printDetails() const {
        printId();
        printTitle();
        printAuthor();
        printPublisher();
        printYear();
        printIsbn();
        printStatus();
    }

    // Serialization
    std::string serialize() const;
    static Book deserialize(const std::string& data);
};

#endif