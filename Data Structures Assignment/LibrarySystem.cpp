#include "LibrarySystem.h"
#include <iostream>
#include <limits>

static void clearInputLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

LibrarySystem::LibrarySystem()
    : games(new GameCopy[MAX]),
    gameCount(0),
    nextCopyId(1),
    members(new Member[MAX]),
    memberCount(0),
    loans(new LoanRecord[MAX]),
    loanCount(0) {
}

LibrarySystem::~LibrarySystem() {
    delete[] games;
    delete[] members;
    delete[] loans;
}

int LibrarySystem::findGameByCopyId(int copyId) const {
    for (int i = 0; i < gameCount; i++) {
        if (games[i].copyId == copyId) return i;
    }
    return -1;
}

int LibrarySystem::findMemberById(int memberId) const {
    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberId == memberId) return i;
    }
    return -1;
}

bool LibrarySystem::removeGameAtIndex(int idx) {
    if (idx < 0 || idx >= gameCount) return false;
    if (games[idx].isBorrowed) return false;

    games[idx] = games[gameCount - 1]; // swap-with-last
    gameCount--;
    return true;
}

void LibrarySystem::addGame() {
    std::string title, category;
    int pmin = 0, pmax = 0, year = 0, copies = 0;

    std::cout << "Title: ";
    std::getline(std::cin >> std::ws, title);

    std::cout << "Category: ";
    std::getline(std::cin, category);

    std::cout << "Players min: ";
    if (!(std::cin >> pmin)) { clearInputLine(); std::cout << "Invalid.\n"; return; }

    std::cout << "Players max: ";
    if (!(std::cin >> pmax)) { clearInputLine(); std::cout << "Invalid.\n"; return; }

    std::cout << "Year: ";
    if (!(std::cin >> year)) { clearInputLine(); std::cout << "Invalid.\n"; return; }

    std::cout << "Number of copies to add: ";
    if (!(std::cin >> copies)) { clearInputLine(); std::cout << "Invalid.\n"; return; }

    if (title.empty() || copies <= 0) {
        std::cout << "Invalid input.\n";
        return;
    }

    for (int i = 0; i < copies; i++) {
        if (gameCount >= MAX) {
            std::cout << "Inventory full. Added " << i << " copies.\n";
            return;
        }

        GameCopy g;
        g.copyId = nextCopyId++;
        g.title = title;
        g.category = category;
        g.playersMin = pmin;
        g.playersMax = pmax;
        g.year = year;
        g.isBorrowed = false;

        games[gameCount++] = g;
    }

    std::cout << "Added " << copies << " copies.\n";
}

void LibrarySystem::removeGame() {
    int copyId = 0;
    std::cout << "Enter copyId to remove: ";
    if (!(std::cin >> copyId)) { clearInputLine(); std::cout << "Invalid.\n"; return; }

    int idx = findGameByCopyId(copyId);
    if (idx == -1) {
        std::cout << "copyId not found.\n";
        return;
    }

    if (games[idx].isBorrowed) {
        std::cout << "Cannot remove. This copy is currently borrowed.\n";
        return;
    }

    bool ok = removeGameAtIndex(idx);
    std::cout << (ok ? "Removed.\n" : "Remove failed.\n");
}

void LibrarySystem::addMember() {
    int id = 0;
    std::string name, contact;

    std::cout << "Member ID (number): ";
    if (!(std::cin >> id)) { clearInputLine(); std::cout << "Invalid.\n"; return; }

    std::cout << "Name: ";
    std::getline(std::cin >> std::ws, name);

    std::cout << "Contact: ";
    std::getline(std::cin, contact);

    if (id <= 0 || name.empty()) {
        std::cout << "Invalid input.\n";
        return;
    }

    if (findMemberById(id) != -1) {
        std::cout << "Member ID already exists.\n";
        return;
    }

    if (memberCount >= MAX) {
        std::cout << "Member list full.\n";
        return;
    }

    Member m;
    m.memberId = id;
    m.name = name;
    m.contact = contact;

    members[memberCount++] = m;
    std::cout << "Member added.\n";
}

void LibrarySystem::showSummary() const {
    int returnedCount = 0;
    int activeCount = 0;

    for (int i = 0; i < loanCount; i++) {
        if (loans[i].returned) returnedCount++;
        else activeCount++;
    }

    std::cout << "\n=== Summary ===\n";
    std::cout << "Total borrow transactions: " << loanCount << "\n";
    std::cout << "Total returned: " << returnedCount << "\n";
    std::cout << "Currently borrowed: " << activeCount << "\n";

    if (activeCount > 0) {
        std::cout << "\nCurrently borrowed list:\n";
        for (int i = 0; i < loanCount; i++) {
            if (!loans[i].returned) {
                std::cout << "copyId " << loans[i].copyId
                    << ", memberId " << loans[i].memberId
                    << ", borrowDate " << loans[i].borrowDate << "\n";
            }
        }
    }

    std::cout << "==============\n\n";
}
