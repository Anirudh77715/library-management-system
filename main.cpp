#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include "library.hpp"

// Enhanced ANSI color codes for gradient effects
const std::string ORANGE = "\033[38;2;255;165;0m";
const std::string PINK = "\033[38;2;255;192;203m";
const std::string PURPLE = "\033[38;2;147;112;219m";
const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string DIM = "\033[2m";

// Box-drawing characters as strings to avoid multi-character constant warnings
const std::string HORIZONTAL = "═";
const std::string VERTICAL = "║";
const std::string TOP_LEFT = "╔";
const std::string TOP_RIGHT = "╗";
const std::string BOTTOM_LEFT = "╚";
const std::string BOTTOM_RIGHT = "╝";
const std::string HORIZONTAL_LINE = "─";

void clearScreen() {
    // Clear screen once
    std::cout << "\033[H\033[2J\033[3J";
}

std::string createButton(const std::string& text, const std::string& color) {
    std::string padding(16 - text.length(), ' ');
    std::string border(10, HORIZONTAL_LINE[0]); 
    std::string shadow = DIM + "  " + BOTTOM_LEFT + border + BOTTOM_RIGHT + RESET;

    return color + BOLD +
           "  " + TOP_LEFT + border + TOP_RIGHT + "\n" +
           "  " + VERTICAL + " " + text + padding.substr(0, padding.length() - 2) + VERTICAL + "\n" +
           "  " + BOTTOM_LEFT + border + BOTTOM_RIGHT + RESET + "\n" +
           shadow + "\n";
}

std::string createHeader(const std::string& text) {
    std::string border(40, HORIZONTAL[0]); 
    return PURPLE + BOLD +
           TOP_LEFT + border + TOP_RIGHT + "\n" +
           VERTICAL + std::string(15, ' ') + text + std::string(15, ' ') + VERTICAL + "\n" +
           BOTTOM_LEFT + border + BOTTOM_RIGHT + "\n" + RESET;
}

void displayMainMenu() {
    clearScreen();
    std::cout << "\n" << createHeader("Library Management System") << "\n";

    std::cout << ORANGE << createButton("1. Login", ORANGE)
              << PINK << createButton("2. Exit", PINK) << RESET;
}

void displayUserMenu(UserRole role) {
    clearScreen();
    std::cout << "\n" << createHeader("User Menu") << "\n";

    switch(role) {
        case UserRole::STUDENT:
        case UserRole::FACULTY:
            std::cout << ORANGE << createButton("1. View Books", ORANGE)
                     << PINK << createButton("2. Search Books", PINK)
                     << PURPLE << createButton("3. Borrow Book", PURPLE)
                     << ORANGE << createButton("4. Return Book", ORANGE)
                     << PINK << createButton("5. My Books", PINK)
                     << PURPLE << createButton("6. View Fines", PURPLE)
                     << ORANGE << createButton("7. Pay Fine", ORANGE)
                     << PINK << createButton("8. Logout", PINK);
            break;

        case UserRole::LIBRARIAN:
            std::cout << ORANGE << createButton("1. Add Book", ORANGE)
                     << PINK << createButton("2. Remove Book", PINK)
                     << PURPLE << createButton("3. Add User", PURPLE)
                     << ORANGE << createButton("4. Remove User", ORANGE)
                     << PINK << createButton("5. View Books", PINK)
                     << PURPLE << createButton("6. View Users", PURPLE)
                     << ORANGE << createButton("7. View Fines", ORANGE)
                     << PINK << createButton("8. Logout", PINK);
            break;
    }
    std::cout << PURPLE << "\nChoice: " << RESET;
}

void displayBooks(const std::vector<Book*>& books) {
    std::cout << "\n" << createHeader("Books Catalog") << "\n";
    std::cout << BOLD << PURPLE
              << std::setw(10) << "ID" 
              << std::setw(30) << "Title"
              << std::setw(20) << "Author"
              << std::setw(15) << "Status" << RESET << "\n";
    std::cout << DIM << std::string(75, HORIZONTAL_LINE[0]) << RESET << "\n";

    for (const auto& book : books) {
        std::string status;
        switch(book->getStatus()) {
            case BookStatus::AVAILABLE: 
                status = ORANGE + "Available" + RESET; 
                break;
            case BookStatus::BORROWED: 
                status = PINK + "Borrowed" + RESET; 
                break;
            case BookStatus::RESERVED: 
                status = PURPLE + "Reserved" + RESET; 
                break;
        }

        std::cout << std::setw(10) << book->getId()
                  << std::setw(30) << book->getTitle()
                  << std::setw(20) << book->getAuthor()
                  << std::setw(15) << status << "\n";
    }
    std::cout << DIM << std::string(75, HORIZONTAL_LINE[0]) << RESET << "\n";
}

User* login(Library& library) {
    std::string email, password;
    std::cout << "Email: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, email);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    // Trim whitespace from input
    email.erase(0, email.find_first_not_of(" \t\n\r"));
    email.erase(email.find_last_not_of(" \t\n\r") + 1);
    password.erase(0, password.find_first_not_of(" \t\n\r"));
    password.erase(password.find_last_not_of(" \t\n\r") + 1);

    for (const auto& user : library.getAllUsers()) {
        if (user->getEmail() == email && user->getPassword() == password) {
            return user.get();
        }
    }
    return nullptr;
}

void handleStudentFacultyMenu(Library& library, User* user) {
    int choice;
    std::string bookId;

    while (true) {
        displayUserMenu(user->getRole());
        std::cin >> choice;

        switch(choice) {
            case 1: 
                {
                    auto books = library.searchBooks("");
                    displayBooks(books);
                }
                break;

            case 2: 
                {
                    std::string query;
                    std::cout << "Enter search term: ";
                    std::cin >> query;
                    auto books = library.searchBooks(query);
                    displayBooks(books);
                }
                break;

            case 3: 
                {
                    std::cout << "Enter Book ID: ";
                    std::cin >> bookId;
                    if (library.borrowBook(user->getId(), bookId)) {
                        std::cout << "Book borrowed successfully!\n";
                    } else {
                        std::cout << "Failed to borrow book.\n";
                    }
                }
                break;

            case 4: 
                {
                    std::cout << "Enter Book ID: ";
                    std::cin >> bookId;
                    if (library.returnBook(user->getId(), bookId)) {
                        std::cout << "Book returned successfully!\n";
                    } else {
                        std::cout << "Failed to return book.\n";
                    }
                }
                break;

            case 5: 
                {
                    const auto& borrowed = user->getAccount().getCurrentlyBorrowedBooks();
                    std::cout << "\nCurrently Borrowed Books:\n";
                    for (const auto& bookId : borrowed) {
                        if (auto book = library.findBook(bookId)) {
                            std::cout << book->getTitle() << "\n";
                        }
                    }
                }
                break;

            case 6: 
                std::cout << "Outstanding fine: ₹" << user->getAccount().getFine() << "\n";
                break;

            case 7: 
                if (user->getAccount().hasFine()) {
                    library.clearFine(user->getId());
                    std::cout << "Fine paid successfully!\n";
                } else {
                    std::cout << "No outstanding fines.\n";
                }
                break;

            case 8: 
                return;

            default:
                std::cout << "Invalid choice!\n";
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

void handleLibrarianMenu(Library& library, User* user) {
    int choice;
    std::string input;

    while (true) {
        displayUserMenu(user->getRole());
        std::cin >> choice;

        switch(choice) {
            case 1: 
                {
                    std::string title, author, publisher, isbn;
                    int year;

                    std::cin.ignore();
                    std::cout << "Title: ";
                    std::getline(std::cin, title);
                    std::cout << "Author: ";
                    std::getline(std::cin, author);
                    std::cout << "Publisher: ";
                    std::getline(std::cin, publisher);
                    std::cout << "Year: ";
                    std::cin >> year;
                    std::cout << "ISBN: ";
                    std::cin >> isbn;

                    library.addBook(std::make_unique<Book>(title, author, publisher, year, isbn));
                    std::cout << "Book added successfully!\n";
                }
                break;

            case 2: 
                {
                    std::cout << "Enter Book ID: ";
                    std::cin >> input;
                    if (library.removeBook(input)) {
                        std::cout << "Book removed successfully!\n";
                    } else {
                        std::cout << "Failed to remove book.\n";
                    }
                }
                break;

            case 3: 
                {
                    std::string name, email, password;
                    int roleChoice;

                    std::cin.ignore();
                    std::cout << "Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Email: ";
                    std::cin >> email;
                    std::cout << "Password: ";
                    std::cin >> password;
                    std::cout << "Role (1: Student, 2: Faculty): ";
                    std::cin >> roleChoice;

                    std::unique_ptr<User> newUser;
                    if (roleChoice == 1) {
                        newUser = std::make_unique<Student>(name, email, password);
                    } else {
                        newUser = std::make_unique<Faculty>(name, email, password);
                    }

                    library.addUser(std::move(newUser));
                    std::cout << "User added successfully!\n";
                }
                break;

            case 4: 
                {
                    std::cout << "Enter User ID: ";
                    std::cin >> input;
                    if (library.removeUser(input)) {
                        std::cout << "User removed successfully!\n";
                    } else {
                        std::cout << "Failed to remove user.\n";
                    }
                }
                break;

            case 5: 
                {
                    auto books = library.searchBooks("");
                    displayBooks(books);
                }
                break;

            case 6: 
                {
                    std::cout << "\n=== Users ===\n";
                    for (const auto& u : library.getAllUsers()) {
                        std::cout << "ID: " << u->getId() 
                                 << ", Name: " << u->getName()
                                 << ", Role: " << static_cast<int>(u->getRole()) << "\n";
                    }
                }
                break;

            case 7: 
                {
                    std::cout << "\n=== Outstanding Fines ===\n";
                    for (const auto& u : library.getAllUsers()) {
                        if (u->getAccount().hasFine()) {
                            std::cout << "User: " << u->getName() 
                                     << ", Fine: ₹" << u->getAccount().getFine() << "\n";
                        }
                    }
                }
                break;

            case 8: 
                return;

            default:
                std::cout << "Invalid choice!\n";
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

void handleUserMenu(Library& library, User* user) {
    if (user->getRole() == UserRole::LIBRARIAN) {
        handleLibrarianMenu(library, user);
    } else {
        handleStudentFacultyMenu(library, user);
    }
}

void runTests(Library& library) {
    // Test mode for verification
    std::cout << "\nRunning OOP implementation verification...\n";

    // Test Faculty login and functionality
    std::cout << "\n1. Testing Faculty Class (Inheritance & Polymorphism):\n";
    User* faculty = nullptr;
    for (const auto& user : library.getAllUsers()) {
        if (user->getEmail() == "wilson@example.com" && user->getPassword() == "pass987") {
            faculty = user.get();
            std::cout << "Faculty login successful: " << faculty->getName() << std::endl;
            break;
        }
    }

    if (faculty) {
        auto* facultyUser = dynamic_cast<Faculty*>(faculty);

        // Test polymorphic behavior
        std::cout << "\na) Verifying Faculty-specific implementations:\n";
        std::cout << "Role verification: " << (facultyUser->getRole() == UserRole::FACULTY ? "Passed" : "Failed") << std::endl;
        std::cout << "Max books allowed: " << facultyUser->getMaxBooks() << " (expected: 5)" << std::endl;
        std::cout << "Max days allowed: " << facultyUser->getMaxDays() << " (expected: 30)" << std::endl;
        std::cout << "Fine calculation: " << facultyUser->calculateFine(5) << " (expected: 0.0)" << std::endl;

        // Reset faculty account before testing
        library.resetUserAccount(faculty->getId());

        // Test borrowing limits with actual book IDs
        std::cout << "\nb) Testing Faculty borrowing rules:\n";
        std::vector<std::string> facultyBookIds;

        // Get first 6 available books
        for (const auto& book : library.searchBooks("")) {
            if (book->getStatus() == BookStatus::AVAILABLE) {
                facultyBookIds.push_back(book->getId());
                if (facultyBookIds.size() >= 6) break;
            }
        }

        for (size_t i = 0; i < facultyBookIds.size(); i++) {
            Book* book = library.findBook(facultyBookIds[i]);
            if (book) {
                bool result = library.borrowBook(faculty->getId(), facultyBookIds[i]);
                std::cout << "Attempt to borrow '" << book->getTitle() 
                         << "': " << (result ? "Succeeded" : "Failed") 
                         << " (Book " << (i+1) << "/" << facultyBookIds.size() << ")" << std::endl;
            }
        }
    }

    // Test Student functionality
    std::cout << "\n2. Testing Student Class (Inheritance & Polymorphism):\n";
    User* student = nullptr;
    for (const auto& user : library.getAllUsers()) {
        if (user->getEmail() == "john@example.com" && user->getPassword() == "pass123") {
            student = user.get();
            std::cout << "Student login successful: " << student->getName() << std::endl;
            break;
        }
    }

    if (student) {
        auto* studentUser = dynamic_cast<Student*>(student);

        // Test polymorphic behavior
        std::cout << "\na) Verifying Student-specific implementations:\n";
        std::cout << "Role verification: " << (studentUser->getRole() == UserRole::STUDENT ? "Passed" : "Failed") << std::endl;
        std::cout << "Max books allowed: " << studentUser->getMaxBooks() << " (expected: 3)" << std::endl;
        std::cout << "Max days allowed: " << studentUser->getMaxDays() << " (expected: 15)" << std::endl;
        std::cout << "Fine calculation: " << studentUser->calculateFine(5) << " (expected: 50.0)" << std::endl;

        // Reset student account before testing
        library.resetUserAccount(student->getId());

        // Test borrowing and fine system with different book IDs
        std::cout << "\nb) Testing Student borrowing rules:\n";
        std::vector<std::string> studentBookIds;

        // Get next 4 available books
        for (const auto& book : library.searchBooks("")) {
            if (book->getStatus() == BookStatus::AVAILABLE) {
                studentBookIds.push_back(book->getId());
                if (studentBookIds.size() >= 4) break;
            }
        }

        for (size_t i = 0; i < studentBookIds.size(); i++) {
            Book* book = library.findBook(studentBookIds[i]);
            if (book) {
                std::cout << "\nTesting with book: " << book->getTitle() << std::endl;
                bool result = library.borrowBook(student->getId(), studentBookIds[i]);
                std::cout << "Attempt to borrow: " << (result ? "Succeeded" : "Failed") 
                         << " (Book " << (i+1) << "/" << studentBookIds.size() << ")" << std::endl;

                if (i == 2) { // Add fine after third successful borrow
                    std::cout << "Adding fine to test borrowing restriction..." << std::endl;
                    student->getAccount().addFine(50.0);
                }
            }
        }
    }

    std::cout << "\nOOP Implementation Verification completed.\n";
}

int main(int argc, char* argv[]) {
    Library library;
    library.initialize();

    if (argc > 1 && std::string(argv[1]) == "--test") {
        runTests(library);
        return 0;
    }

    // Normal interactive mode
    while (true) {
        clearScreen();
        displayMainMenu();
        std::cout << PURPLE << "\nChoice: " << RESET;

        int choice;
        std::string input;
        std::getline(std::cin >> std::ws, input);

        try {
            choice = std::stoi(input);
        } catch (...) {
            choice = 0;  // Invalid input
        }

        switch (choice) {
            case 1: {
                std::cout << "Email: ";
                std::string email;
                std::getline(std::cin >> std::ws, email);

                std::cout << "Password: ";
                std::string password;
                std::getline(std::cin >> std::ws, password);

                User* user = nullptr;
                for (const auto& u : library.getAllUsers()) {
                    if (u->getEmail() == email && u->authenticate(password)) {
                        user = u.get();
                        break;
                    }
                }

                if (user) {
                    std::cout << "Login successful: " << user->getName() << "\n";
                    handleUserMenu(library, user);
                } else {
                    std::cout << "Invalid credentials. Please try again.\n";
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                break;
            }
            case 2:
                std::cout << "Thank you for using the Library Management System!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::cout << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return 0;
}