#include "LibrarySystem.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <cctype>

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

std::string LibrarySystem::trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) start++;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;
    return s.substr(start, end - start);
}

int LibrarySystem::toIntSafe(const std::string& s) {
    std::string t = trim(s);
    if (t.empty()) return 0;
    try {
        return std::stoi(t);
    }
    catch (...) {
        return 0;
    }
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

void LibrarySystem::loadGamesFromCSV(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::string line;

    // Skip the header line
    std::getline(file, line);

    int loadedCount = 0;

    while (std::getline(file, line)) {
        if (trim(line).empty()) continue;

        std::stringstream ss(line);
        std::string token;

        GameCopy game;
        game.isBorrowed = false;
        game.copyId = nextCopyId++;

        // Defaults (in case fields are missing)
        game.title = "";
        game.category = "";
        game.playersMin = 0;
        game.playersMax = 0;
        game.year = 0;

        int field = 0;

        // Parse CSV line by comma
        while (std::getline(ss, token, ',')) {
            token = trim(token);

            // Remove surrounding quotes if present
            if (token.size() >= 2 && token.front() == '"' && token.back() == '"') {
                token = token.substr(1, token.length() - 2);
            }

            switch (field) {
            case 0: game.title = token; break;                  // Title
            case 1: game.category = token; break;               // Category
            case 2: game.playersMin = toIntSafe(token); break;  // Players Min
            case 3: game.playersMax = toIntSafe(token); break;  // Players Max
            case 4: game.year = toIntSafe(token); break;        // Year
            default: break;
            }
            field++;
        }

        if (game.title.empty()) {
            // skip bad/empty rows
            continue;
        }

        if (gameCount < MAX) {
            games[gameCount++] = game;
            loadedCount++;
        }
        else {
            std::cout << "Warning: Inventory full. Loaded " << loadedCount << " games.\n";
            break;
        }
    }

    file.close();
    std::cout << "Successfully loaded " << loadedCount << " games from CSV.\n";
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

    if (trim(title).empty() || copies <= 0) {
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

    if (id <= 0 || trim(name).empty()) {
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
