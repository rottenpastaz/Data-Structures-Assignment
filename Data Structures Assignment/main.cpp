#include "LibrarySystem.h"
#include <iostream>
#include <limits>

static void clearInputLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    LibrarySystem sys;

    std::cout << "Loading games from CSV...\n";
    sys.loadGamesFromCSV("data//games.csv");

    while (true) {
        std::cout << "Admin Menu\n";
        std::cout << "1) Add new board game\n";
        std::cout << "2) Remove a board game\n";
        std::cout << "3) Add a new member\n";
        std::cout << "4) Display summary of games borrowed/returned\n";
        std::cout << "5) Display all games (first 10)\n";
        std::cout << "0) Exit\n";
        std::cout << "Choice: ";

        int c;
        if (!(std::cin >> c)) {
            clearInputLine();
            std::cout << "Invalid input.\n";
            continue;
        }

        switch (c) {
        case 1: sys.addGame(); break;
        case 2: sys.removeGame(); break;
        case 3: sys.addMember(); break;
        case 4: sys.showSummary(); break;
        case 5: sys.displayAllGames(); break;
        case 0: return 0;
        default: std::cout << "Invalid choice.\n"; break;
        }
    }
}
