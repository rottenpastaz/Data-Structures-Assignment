#pragma once
#include <string>

const int MAX = 5000;

struct GameCopy {
    int copyId;
    std::string title;
    std::string category;
    int playersMin;
    int playersMax;
    int year;
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

public:
    LibrarySystem();
    ~LibrarySystem();

    // prevent copying (raw pointers)
    LibrarySystem(const LibrarySystem&) = delete;
    LibrarySystem& operator=(const LibrarySystem&) = delete;

    // admin actions
    void addGame();        // add N copies
    void removeGame();     // remove by copyId (if not borrowed)
    void addMember();
    void showSummary() const;
};
