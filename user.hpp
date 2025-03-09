#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <iostream>
#include "account.hpp"

// Encapsulation: Role types in enum class
enum class UserRole {
    STUDENT,
    FACULTY,
    LIBRARIAN
};

// Abstract base class for all users
class User {
private:  // Encapsulation: Private data members
    std::string id;
    std::string name;
    std::string email;
    std::string password;
    UserRole role;
    Account account;

protected:  // Protected constructor for abstract class
    User(const std::string& name, const std::string& email, 
         const std::string& password, UserRole role);

public:
    virtual ~User() = default;  // Virtual destructor for polymorphic behavior

    // Encapsulation: Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPassword() const { return password; }
    UserRole getRole() const { return role; }
    Account& getAccount() { return account; }

    // Authentication method
    bool authenticate(const std::string& inputPassword) const {
        return password == inputPassword;
    }

    // Print methods for each attribute
    void printId() const { std::cout << "ID: " << id << std::endl; }
    void printName() const { std::cout << "Name: " << name << std::endl; }
    void printEmail() const { std::cout << "Email: " << email << std::endl; }
    void printRole() const { 
        std::cout << "Role: " << 
            (role == UserRole::STUDENT ? "Student" : 
             role == UserRole::FACULTY ? "Faculty" : "Librarian") << std::endl; 
    }

    // Update methods for attributes
    void updateName(const std::string& newName) { name = newName; }
    void updateEmail(const std::string& newEmail) { email = newEmail; }
    void updatePassword(const std::string& newPassword) { password = newPassword; }

    // Pure virtual methods define the interface
    virtual int getMaxBooks() const = 0;
    virtual int getMaxDays() const = 0;
    virtual double calculateFine(int daysOverdue) const = 0;
    virtual bool borrowBook(const std::string& bookId) = 0;
    virtual bool returnBook(const std::string& bookId) = 0;

    // Serialization
    std::string serialize() const;
    static User* deserialize(const std::string& data);

    friend class Library; // Allow Library to access id
};

// Inheritance: Student inherits from User
class Student : public User {
public:
    Student(const std::string& name, const std::string& email, 
            const std::string& password);

    // Polymorphism: Override virtual methods
    int getMaxBooks() const override;
    int getMaxDays() const override;
    double calculateFine(int daysOverdue) const override;
    bool borrowBook(const std::string& bookId) override;
    bool returnBook(const std::string& bookId) override;
};

// Inheritance: Faculty inherits from User
class Faculty : public User {
public:
    Faculty(const std::string& name, const std::string& email, 
            const std::string& password);

    // Polymorphism: Different implementation for faculty
    int getMaxBooks() const override;
    int getMaxDays() const override;
    double calculateFine(int) const override;
    bool borrowBook(const std::string& bookId) override;
    bool returnBook(const std::string& bookId) override;
};

// Inheritance: Librarian inherits from User
class Librarian : public User {
public:
    Librarian(const std::string& name, const std::string& email, 
              const std::string& password);

    // Polymorphism: Librarian-specific implementation
    int getMaxBooks() const override;
    int getMaxDays() const override;
    double calculateFine(int) const override;
    bool borrowBook(const std::string&) override;
    bool returnBook(const std::string&) override;
};

#endif