#include "library.hpp"
#include "utils.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

Library::Library() : isInitialized(false) {
    std::cout << "Initializing Library System...\n";
}

Library::~Library() {
    std::cout << "Saving Library System state...\n";
    saveData();
}

void Library::loadData() {
    std::cout << "Loading data from files...\n";
    books.clear();
    users.clear();

    std::string bookData = Utils::readFromFile("data/books.txt");
    std::istringstream bookSS(bookData);
    std::string line;
    std::set<std::string> loadedBookIds;

    while (std::getline(bookSS, line)) {
        if (!line.empty()) {
            auto book = Book::deserialize(line);
            if (loadedBookIds.find(book.getId()) == loadedBookIds.end()) {
                books.push_back(std::make_unique<Book>(book));
                loadedBookIds.insert(book.getId());
                std::cout << "Loaded book: " << books.back()->getTitle() << "\n";
            }
        }
    }

    // Load users
    std::string userData = Utils::readFromFile("data/users.txt");
    std::istringstream userSS(userData);
    std::set<std::string> loadedUserIds;

    while (std::getline(userSS, line)) {
        if (!line.empty()) {
            User* user = User::deserialize(line);
            if (user && loadedUserIds.find(user->getId()) == loadedUserIds.end()) {
                users.push_back(std::unique_ptr<User>(user));
                loadedUserIds.insert(user->getId());
                std::cout << "Loaded user: " << users.back()->getName() << "\n";
            }
        }
    }

    // Reset all book statuses to AVAILABLE for test mode
    for (const auto& book : books) {
        book->setStatus(BookStatus::AVAILABLE);
    }
}

void Library::saveData() const {
    if (!isInitialized) return;  // Don't save if not initialized

    std::cout << "Saving data to files...\n";
    std::stringstream bookSS;
    for (const auto& book : books) {
        bookSS << book->serialize() << "\n";
    }
    Utils::saveToFile("data/books.txt", bookSS.str());

    std::stringstream userSS;
    for (const auto& user : users) {
        userSS << user->serialize() << "\n";
    }
    Utils::saveToFile("data/users.txt", userSS.str());
    std::cout << "Data saved successfully.\n";
}

void Library::initialize() {
    if (isInitialized) return;  // Prevent multiple initializations

    loadData();

    // Only add default books if none exist
    if (books.empty()) {
        std::cout << "Adding default books...\n";
        addBook(std::make_unique<Book>("The Great Gatsby", "F. Scott Fitzgerald", "Scribner", 1925, "978-0743273565"));
        addBook(std::make_unique<Book>("To Kill a Mockingbird", "Harper Lee", "Grand Central", 1960, "978-0446310789"));
        addBook(std::make_unique<Book>("1984", "George Orwell", "Penguin", 1949, "978-0451524935"));
        addBook(std::make_unique<Book>("Pride and Prejudice", "Jane Austen", "Penguin", 1813, "978-0141439518"));
        addBook(std::make_unique<Book>("The Catcher in the Rye", "J.D. Salinger", "Little, Brown", 1951, "978-0316769488"));
        addBook(std::make_unique<Book>("Lord of the Rings", "J.R.R. Tolkien", "Mariner Books", 1954, "978-0544003415"));
        addBook(std::make_unique<Book>("The Hobbit", "J.R.R. Tolkien", "Mariner Books", 1937, "978-0547928227"));
        addBook(std::make_unique<Book>("The Da Vinci Code", "Dan Brown", "Anchor", 2003, "978-0307474278"));
        addBook(std::make_unique<Book>("Harry Potter", "J.K. Rowling", "Scholastic", 1997, "978-0590353427"));
        addBook(std::make_unique<Book>("The Alchemist", "Paulo Coelho", "HarperOne", 1988, "978-0062315007"));
    }

    // Only add default users if none exist
    if (users.empty()) {
        std::cout << "Adding default users...\n";
        // Students
        addUser(std::make_unique<Student>("John Doe", "john@example.com", "pass123"));
        addUser(std::make_unique<Student>("Jane Smith", "jane@example.com", "pass456"));
        addUser(std::make_unique<Student>("Bob Wilson", "bob@example.com", "pass789"));
        addUser(std::make_unique<Student>("Alice Brown", "alice@example.com", "pass321"));
        addUser(std::make_unique<Student>("Charlie Davis", "charlie@example.com", "pass654"));

        // Faculty
        addUser(std::make_unique<Faculty>("Dr. Wilson", "wilson@example.com", "pass987"));
        addUser(std::make_unique<Faculty>("Prof. Johnson", "johnson@example.com", "pass654"));
        addUser(std::make_unique<Faculty>("Dr. Martinez", "martinez@example.com", "pass321"));

        // Librarian
        addUser(std::make_unique<Librarian>("Admin", "admin@library.com", "admin123"));
    }

    isInitialized = true;  // Mark as initialized after successful setup
}

bool Library::addBook(std::unique_ptr<Book> book) {
    books.push_back(std::move(book));
    return true;
}

bool Library::removeBook(const std::string& bookId) {
    auto it = std::find_if(books.begin(), books.end(),
        [&bookId](const auto& book) { return book->getId() == bookId; });

    if (it != books.end()) {
        books.erase(it);
        return true;
    }
    return false;
}

Book* Library::findBook(const std::string& bookId) {
    auto it = std::find_if(books.begin(), books.end(),
        [&bookId](const auto& book) { return book->getId() == bookId; });

    return it != books.end() ? it->get() : nullptr;
}

std::vector<Book*> Library::searchBooks(const std::string& query) {
    std::vector<Book*> results;
    for (const auto& book : books) {
        if (book->getTitle().find(query) != std::string::npos ||
            book->getAuthor().find(query) != std::string::npos ||
            book->getIsbn().find(query) != std::string::npos) {
            results.push_back(book.get());
        }
    }
    return results;
}

bool Library::addUser(std::unique_ptr<User> user) {
    users.push_back(std::move(user));
    return true;
}

bool Library::removeUser(const std::string& userId) {
    auto it = std::find_if(users.begin(), users.end(),
        [&userId](const auto& user) { return user->getId() == userId; });

    if (it != users.end()) {
        users.erase(it);
        return true;
    }
    return false;
}

User* Library::findUser(const std::string& userId) {
    auto it = std::find_if(users.begin(), users.end(),
        [&userId](const auto& user) { return user->getId() == userId; });

    return it != users.end() ? it->get() : nullptr;
}

bool Library::borrowBook(const std::string& userId, const std::string& bookId) {
    User* user = findUser(userId);
    Book* book = findBook(bookId);

    if (!user || !book) {
        std::cout << "Error: User or book not found.\n";
        return false;
    }
    if (book->getStatus() != BookStatus::AVAILABLE) {
        std::cout << "Error: Book is not available.\n";
        return false;
    }
    if (user->getAccount().getCurrentlyBorrowedBooks().size() >= user->getMaxBooks()) {
        std::cout << "Error: User has reached maximum books limit.\n";
        return false;
    }
    if (user->getAccount().hasFine()) {
        std::cout << "Error: User has outstanding fines.\n";
        return false;
    }

    // Faculty specific check for overdue books > 60 days
    if (user->getRole() == UserRole::FACULTY) {
        for (const auto& record : user->getAccount().getBorrowHistory()) {
            if (record.returnDate == 0) {  // Book not returned yet
                int daysOverdue = Utils::calculateDaysDifference(
                    record.borrowDate + user->getMaxDays() * 24 * 60 * 60,
                    Utils::getCurrentTime());
                if (daysOverdue > 60) {
                    std::cout << "Error: Faculty member has book(s) overdue for more than 60 days.\n";
                    return false;
                }
            }
        }
    }

    book->setStatus(BookStatus::BORROWED);
    user->getAccount().addBorrowedBook(bookId);

    // Log transaction
    std::ofstream transactionFile("data/transactions.txt", std::ios::app);
    if (transactionFile) {
        transactionFile << bookId << "|" << userId << "|" 
                       << Utils::getCurrentTime() << "|0|0.0\n";
    }

    std::cout << "Book '" << book->getTitle() << "' borrowed by " << user->getName() << "\n";
    return true;
}

bool Library::returnBook(const std::string& userId, const std::string& bookId) {
    User* user = findUser(userId);
    Book* book = findBook(bookId);

    if (!user || !book) {
        std::cout << "Error: User or book not found.\n";
        return false;
    }

    book->setStatus(BookStatus::AVAILABLE);
    user->getAccount().returnBook(bookId);

    // Calculate fine
    double fine = calculateFine(userId, bookId);
    if (fine > 0) {
        user->getAccount().addFine(fine);
        std::cout << "Fine of ₹" << fine << " added for overdue book.\n";
    }

    // Log transaction with return date and fine
    std::ofstream transactionFile("data/transactions.txt", std::ios::app);
    if (transactionFile) {
        transactionFile << bookId << "|" << userId << "|" 
                       << Utils::getCurrentTime() << "|" 
                       << Utils::getCurrentTime() << "|" << fine << "\n";
    }

    std::cout << "Book '" << book->getTitle() << "' returned by " << user->getName() << "\n";
    return true;
}

double Library::calculateFine(const std::string& userId, const std::string& bookId) {
    User* user = findUser(userId);
    if (!user) return 0.0;

    for (const auto& record : user->getAccount().getBorrowHistory()) {
        if (record.bookId == bookId && record.returnDate > 0) {
            int daysOverdue = Utils::calculateDaysDifference(
                record.borrowDate + user->getMaxDays() * 24 * 60 * 60,
                record.returnDate);
            return user->calculateFine(daysOverdue);
        }
    }
    return 0.0;
}

void Library::clearFine(const std::string& userId) {
    User* user = findUser(userId);
    if (user) {
        user->getAccount().clearFine();
    }
}

const std::vector<std::unique_ptr<User>>& Library::getAllUsers() const {
    return users;
}

bool Library::resetUserAccount(const std::string& userId) {
    User* user = findUser(userId);
    if (user) {
        user->getAccount() = Account(); // Reset to fresh account
        return true;
    }
    return false;
}