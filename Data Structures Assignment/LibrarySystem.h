#pragma once
#include <string>

const int MAX = 5000;

struct GameCopy {
    int copyId;
    std::string name;        // CSV: name
    int minPlayers;          // CSV: minplayers
    int maxPlayers;          // CSV: maxplayers
    int maxPlayTime;         // CSV: maxplaytime
    int minPlayTime;         // CSV: minplaytime
    int yearPublished;       // CSV: yearpublished
    bool isBorrowed;
};

struct Member {
    int memberId;
    std::string name;
    std::string contact;
};

struct LoanRecord {
    int copyId;
    int memberId;
    std::string borrowDate;  // YYYY-MM-DD
    std::string returnDate;  // "" if not returned
    bool returned;
};

class LibrarySystem {
private:
    GameCopy* games;
    int gameCount;
    int nextCopyId;

    Member* members;
    int memberCount;

    LoanRecord* loans;
    int loanCount;

    // helpers
    int findGameByCopyId(int copyId) const;
    int findMemberById(int memberId) const;
    bool removeGameAtIndex(int idx);

    static std::string trim(const std::string& s);
    static int toIntSafe(const std::string& s);

public:
    LibrarySystem();
    ~LibrarySystem();

    LibrarySystem(const LibrarySystem&) = delete;
    LibrarySystem& operator=(const LibrarySystem&) = delete;

    // file loading
    void loadGamesFromCSV(const std::string& filename);

    // admin actions
    void addGame();        // add N copies (manual input)
    void removeGame();     // remove by copyId (if not borrowed)
    void addMember();
    void showSummary() const;

    // testing / display
    void displayAllGames() const; // shows first 10
};
