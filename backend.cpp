/*
 * NAGARSEVAK CONNECT - Pure Terminal Application
 * Pune Municipal Corporation Civic Issue Portal
 * Demonstrates all core C++ OOP concepts from syllabus
 *
 * Topics Covered:
 * 1. Classes, Objects, Access Specifiers, Static Members
 * 2. Constructors (Default, Parameterized, Copy), Destructors
 * 3. Operator Overloading (Unary, Binary), Friend Functions
 * 4. Inheritance (Single, Multilevel, Multiple/Hierarchical)
 * 5. Polymorphism (Overloading, Overriding, Virtual Functions)
 * 6. Pointers, this pointer, Dynamic Memory (new/delete)
 * 7. Exception Handling (try/catch/throw, Custom Exceptions)
 * 8. Templates (Function & Class Templates)
 * 9. Abstraction, Encapsulation
 * 10. Inline functions, Default arguments, Scope Resolution
 */

// HEADER FILES — These give us ready-made tools to use
#include <iostream>   // For input (cin) and output (cout)
#include <fstream>    // For reading/writing files (ifstream, ofstream)
#include <string>     // For using string data type
#include <sstream>    // For stringstream (helps parse strings)
#include <exception>  // For exception handling (try/catch/throw)
#include <climits>    // For INT_MAX constant
#include <ctime>      // For getting current date and time
#include <iomanip>    // For output formatting (setw, left, etc.)

// 'using namespace std' lets us write cout instead of std::cout
using namespace std;

// ======================================================================
// EXCEPTION HANDLING (try, catch, throw)
// ======================================================================
// What: Exceptions handle errors gracefully instead of crashing.
// How:  We create custom exception classes that inherit from std::exception.
//       When error occurs, we 'throw' it. Calling code 'catches' it.
// Why:  ServerException is BASE. FileException & InputException are DERIVED.
//       This shows INHERITANCE in exception classes too!

// Base exception class — inherits from C++ built-in 'exception' class
// 'public exception' = INHERITANCE (ServerException IS-A exception)
class ServerException : public exception {
    string message;      // private: stores the error message
    string timestamp;    // private: stores when the error happened
public:
    // PARAMETERIZED CONSTRUCTOR — takes error message as input
    ServerException(string msg) : message(msg) {  // initializer list
        time_t now = time(nullptr);       // get current time
        struct tm* ti = localtime(&now);  // convert to local time
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ti);
        timestamp = string(buf);
    }
    // FUNCTION OVERRIDING — overrides what() from base 'exception' class
    // 'const noexcept override' = won't modify object, won't throw, overrides parent
    const char* what() const noexcept override { return message.c_str(); }
    // INLINE FUNCTION — compiler replaces call with actual code (faster)
    inline string getTimestamp() const { return timestamp; }
};

// SINGLE INHERITANCE — FileException inherits from ServerException
// Constructor calls PARENT constructor using : ServerException(...)
class FileException : public ServerException {
public:
    FileException(string f) : ServerException("File I/O Error: '" + f + "'") {}
};

// Another child of ServerException — shows HIERARCHICAL INHERITANCE
// (Multiple classes inheriting from same parent)
class InputException : public ServerException {
public:
    InputException(string d) : ServerException("Input Error: " + d) {}
};

// ======================================================================
// ABSTRACTION — Pure Virtual Functions / Abstract Classes
// ======================================================================
// What: Abstract class = a class with at least one PURE VIRTUAL function
// How:  '= 0' makes a function pure virtual (no body, MUST be overridden)
// Why:  Forces all child classes to provide their own implementation
//       You CANNOT create objects of abstract classes directly!

class ISerializable {
public:
    virtual string serialize() const = 0;  // Pure virtual — child MUST implement
    virtual ~ISerializable() {}            // Virtual destructor for safe cleanup
};

class ILoggable {
public:
    virtual void logCreation() const = 0;  // Pure virtual — child MUST implement
    virtual ~ILoggable() {}                // Virtual destructor
};

// ======================================================================
// CLASSES & OBJECTS, ENCAPSULATION, STATIC MEMBERS
// ======================================================================
// CLASS  = Blueprint/template for creating objects (like a form design)
// OBJECT = Actual instance of a class (like a filled form)
// ENCAPSULATION = Hiding data using access specifiers:
//   private:   Only this class can access
//   protected: This class + child classes can access
//   public:    Anyone can access
//
// MULTIPLE INHERITANCE — Report inherits from BOTH ISerializable AND ILoggable

class Report : public ISerializable, public ILoggable {
protected:  // ACCESS SPECIFIER — accessible by this class and its children
    string id;
    string title;
    string description;
    string createdAt;
    string location;
    // STATIC DATA MEMBERS — shared by ALL objects of this class (not per-object)
    // Only ONE copy exists no matter how many objects are created
    static int totalReports;
    static int lifetimeReports;

public:
    // Default Constructor
    Report() : id("0"), title("None"), description("None"), location("None") {
        totalReports++;
        lifetimeReports++;
        setTimestamp();
    }

    // Parameterized Constructor
    Report(string i, string t, string d) : id(i), title(t), description(d), location("None") {
        totalReports++;
        lifetimeReports++;
        setTimestamp();
    }

    // Copy Constructor
    Report(const Report& other)
        : id(other.id), title(other.title), description(other.description),
          createdAt(other.createdAt), location(other.location) {
        totalReports++;
        lifetimeReports++;
    }

    // Virtual Destructor (for proper cleanup in inheritance)
    virtual ~Report() { totalReports--; }

    // Static member functions accessed via scope resolution operator (Report::)
    inline static int getTotalReports() { return totalReports; }
    inline static int getLifetimeReports() { return lifetimeReports; }

    // Encapsulation: getters and setters
    inline string getId() const { return id; }
    inline string getTitle() const { return title; }
    inline void setLocation(string loc) { location = loc; }
    inline string getLocation() const { return location; }

    // Pure virtual functions (Abstraction — forces derived classes to implement)
    virtual void display() const = 0;

    // ILoggable interface implementation
    void logCreation() const override {
        cout << "   [LOG] Report " << id << " created at " << createdAt << "\n";
    }

private:
    // Private member function
    void setTimestamp() {
        time_t now = time(nullptr);
        struct tm* ti = localtime(&now);
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ti);
        createdAt = string(buf);
    }
};

// STATIC MEMBER INITIALIZATION — must be done OUTSIDE the class
// Uses SCOPE RESOLUTION OPERATOR (::) to say "this belongs to Report class"
// Report::totalReports means "totalReports that belongs to Report"
int Report::totalReports = 0;
int Report::lifetimeReports = 0;

// ======================================================================
// SINGLE INHERITANCE — CivicIssue IS-A Report
// ======================================================================
// What: CivicIssue inherits ALL members of Report (id, title, etc.)
// How:  'class CivicIssue : public Report' = public inheritance
// Also demonstrates: Operator Overloading, Friend Functions

class CivicIssue : public Report {
protected:
    string ward;       // Extra data not in parent Report
    string category;
    int upvotes;

public:
    // DEFAULT CONSTRUCTOR — no arguments, sets default values
    // Report() calls the PARENT's default constructor first (constructor chaining)
    CivicIssue() : Report(), ward("Unknown"), category("Unknown"), upvotes(0) {}

    // PARAMETERIZED CONSTRUCTOR with DEFAULT ARGUMENT (loc = "None")
    // If caller doesn't pass 'loc', it automatically becomes "None"
    // Report(i, t, d) = calling parent's parameterized constructor
    CivicIssue(string i, string t, string d, string w, string c, string loc = "None")
        : Report(i, t, d), ward(w), category(c), upvotes(0) {
        // 'this' POINTER — points to the current object calling this function
        // this->setLocation(loc) = "call setLocation on THIS object"
        this->setLocation(loc);
    }

    // Function Overriding (Runtime Polymorphism via virtual function)
    void display() const override {
        cout << "   [" << id << "] " << title << "\n"
             << "      Ward: " << ward << " | Category: " << category << "\n"
             << "      Location: " << location << "\n"
             << "      Description: " << description << "\n"
             << "      Priority Votes: " << upvotes << "\n";
    }

    // ISerializable implementation
    string serialize() const override {
        return id + "|" + title + "|" + description + "|" + ward + "|" + category + "|" + to_string(upvotes) + "|" + location;
    }

    inline string getWard() const { return ward; }
    inline string getCategory() const { return category; }
    inline int getUpvotes() const { return upvotes; }

    // ---- OPERATOR OVERLOADING ----
    // What: Giving new meaning to operators (+, ++, <, ==, <<) for our class

    // UNARY OPERATOR OVERLOADING — prefix ++ (e.g., ++issue)
    // Unary = works on ONE object. Increases upvotes by 1.
    void operator++() { this->upvotes += 1; }

    // UNARY OPERATOR OVERLOADING — postfix ++ (e.g., issue++)
    // 'int' parameter is a dummy just to differentiate from prefix
    void operator++(int) { this->upvotes += 1; }

    // BINARY OPERATOR OVERLOADING — < (less than)
    // Binary = works on TWO objects. Compares upvotes of two issues.
    // 'const' = won't modify either object
    bool operator<(const CivicIssue& other) const {
        return this->upvotes < other.upvotes;
    }

    // BINARY OPERATOR OVERLOADING — == (equality)
    bool operator==(const CivicIssue& other) const {
        return this->id == other.id;
    }

    // FRIEND DECLARATIONS — these outside functions can access private members
    // 'friend' breaks encapsulation intentionally when needed
    friend ostream& operator<<(ostream& os, const CivicIssue& issue);
    friend void printIssueUrgency(const CivicIssue& issue);
};

// FRIEND OPERATOR<< — allows us to do: cout << issueObject
// This function is NOT a member of CivicIssue, but it can access
// private/protected members (id, title, etc.) because it's declared as 'friend'
ostream& operator<<(ostream& os, const CivicIssue& issue) {
    os << "[" << issue.id << "] " << issue.title
       << " (" << issue.category << ", " << issue.ward << ") "
       << "Votes:" << issue.upvotes;
    return os;  // return stream so we can chain: cout << a << b
}

// FRIEND FUNCTION — not a member, but can access protected 'upvotes'
// Uses if-else (CONTROL STRUCTURE) to check priority levels
void printIssueUrgency(const CivicIssue& issue) {
    if (issue.upvotes > 10)
        cout << "      >> CRITICAL URGENCY - IMMEDIATE ACTION NEEDED\n";
    else if (issue.upvotes > 5)
        cout << "      >> High Priority - Escalated\n";
    else if (issue.upvotes > 2)
        cout << "      >> Medium Priority\n";
    else
        cout << "      >> Normal Priority\n";
}

// ======================================================================
// MULTILEVEL INHERITANCE: UrgentIssue -> CivicIssue -> Report
// ======================================================================
// What: UrgentIssue inherits CivicIssue, which inherits Report
//       So UrgentIssue gets ALL members from BOTH parent classes!
// Constructor Chain: UrgentIssue() -> CivicIssue() -> Report()

class UrgentIssue : public CivicIssue {
    string urgencyLevel;  // private by default in class
public:
    // Constructor calls PARENT CivicIssue constructor, which calls Report constructor
    // This is CONSTRUCTOR BEHAVIOR IN INHERITANCE
    UrgentIssue(string i, string t, string d, string w, string c, string loc, string urg)
        : CivicIssue(i, t, d, w, c, loc), urgencyLevel(urg) {}

    // FUNCTION OVERRIDING — same function name as parent but different behavior
    // 'override' keyword ensures we're actually overriding a virtual function
    void display() const override {
        cout << "   !! [EMERGENCY - " << urgencyLevel << "] " << title << "\n"
             << "      Ward: " << ward << " | Immediate action required!\n"
             << "      Location: " << location << "\n"
             << "      " << description << "\n";
    }
};

// ======================================================================
// CLASS TEMPLATE + DYNAMIC MEMORY (new/delete) + POINTERS
// ======================================================================
// TEMPLATE: Write ONE class that works with ANY data type
//   'template <typename T>' means T is a placeholder for any type
//   When we write IssueManager<CivicIssue>, T becomes CivicIssue
//
// DYNAMIC MEMORY: Using new/delete instead of fixed arrays
//   new   = allocate memory at runtime (on heap)
//   delete = free that memory when done (prevents memory leak)
//
// POINTERS: Variables that store memory addresses
//   T*  = pointer to one T object
//   T** = pointer to an array of T pointers (array of objects using pointers)

template <typename T>
class IssueManager {
private:
    T** issues;    // POINTER TO POINTER — dynamic array of pointers to objects
    int count;     // how many issues currently stored
    int capacity;  // max size before we need to grow the array

    // Private helper: doubles array size when full
    // Demonstrates: new[], delete[], pointer manipulation
    void resize() {
        int newCap = capacity * 2;
        T** newArr = new T*[newCap];  // allocate bigger array with 'new'
        for (int i = 0; i < count; i++) {
            newArr[i] = issues[i];     // copy old pointers to new array
        }
        delete[] issues;   // free old array with 'delete[]'
        issues = newArr;   // point to new array
        capacity = newCap;
    }

public:
    // CONSTRUCTOR — allocates initial dynamic array
    IssueManager() : count(0), capacity(10) {
        issues = new T*[capacity];  // 'new' allocates array on heap
    }

    // Destructor: clean up all dynamic memory
    ~IssueManager() {
        clearMemory();
        delete[] issues;
    }

    void clearMemory() {
        for (int i = 0; i < count; i++) {
            delete issues[i];
        }
        count = 0;
    }

    void addIssue(T* issue) {
        if (count >= capacity) resize();
        issues[count] = issue;
        count++;
    }

    // FUNCTION OVERLOADING — same function name, different parameters
    // Compiler picks the right version based on arguments passed
    void displayAll() { displayAll(false); }  // version 1: no arguments

    void displayAll(bool showPriority) {       // version 2: with bool argument
        if (count == 0) {
            cout << "   [No civic issues found in system]\n";
            return;
        }
        // LOOP through array using index and pointer arrow operator (->)
        for (int i = 0; i < count; i++) {
            cout << "\n   " << (i + 1) << ". ";
            // issues[i]->display() = RUNTIME POLYMORPHISM
            // Calls the correct display() based on actual object type
            issues[i]->display();
            if (showPriority) {
                // dynamic_cast = safely convert base pointer to derived pointer
                CivicIssue* c = dynamic_cast<CivicIssue*>(issues[i]);
                if (c) printIssueUrgency(*c);  // call friend function
            }
        }
    }

    int getCount() const { return count; }

    // File I/O with exception handling
    void saveToFile(const string& filename) {
        ofstream out(filename);
        if (!out) throw FileException(filename);
        for (int i = 0; i < count; i++) {
            out << issues[i]->serialize() << "\n";
        }
        out.close();
    }

    void loadFromFile(const string& filename) {
        ifstream in(filename);
        if (!in) return;
        clearMemory();
        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string i, t, d, w, c, u_str, loc_str;
            getline(ss, i, '|');
            getline(ss, t, '|');
            getline(ss, d, '|');
            getline(ss, w, '|');
            getline(ss, c, '|');
            getline(ss, u_str, '|');
            getline(ss, loc_str, '|');
            if (!i.empty() && !t.empty()) {
                T* ni = new CivicIssue(i, t, d, w, c, loc_str);
                int uv = u_str.empty() ? 0 : stoi(u_str);
                for (int k = 0; k < uv; k++) ++(*ni);
                addIssue(ni);
            }
        }
        in.close();
    }

    T* findById(const string& targetId) {
        for (int i = 0; i < count; i++) {
            if (issues[i]->getId() == targetId) return issues[i];
        }
        return nullptr;
    }

    // Get issue by index (pointer to object)
    T* getAt(int index) {
        if (index >= 0 && index < count) return issues[index];
        return nullptr;
    }
};

// FUNCTION TEMPLATE — works with any data type (string, int, char*, etc.)
// 'template <typename T>' means T can be any type
// Example: printHeader("Hello") → T is const char*
//          printHeader(42)      → T is int
template <typename T>
void printHeader(T titleText) {
    cout << "\n=========================================\n";
    cout << "   " << titleText << "\n";
    cout << "=========================================\n";
}

// ====== MENU-DRIVEN TERMINAL APPLICATION ======

void showMenu() {
    cout << "\n-----------------------------------------\n";
    cout << "   NAGARSEVAK CONNECT - MAIN MENU\n";
    cout << "-----------------------------------------\n";
    cout << "  1. Report New Issue\n";
    cout << "  2. View All Issues\n";
    cout << "  3. View Issues with Priority\n";
    cout << "  4. Upvote an Issue\n";
    cout << "  5. Search Issue by ID\n";
    cout << "  6. Clear All Issues\n";
    cout << "  7. Demo: Polymorphism\n";
    cout << "  8. Demo: Copy Constructor\n";
    cout << "  9. Show OOP Stats\n";
    cout << "  0. Exit\n";
    cout << "-----------------------------------------\n";
    cout << "  Enter your choice: ";
}

int main() {
    IssueManager<CivicIssue> manager;

    // Load existing issues from file (Exception handling)
    try {
        manager.loadFromFile("issues.txt");
    } catch (const FileException& e) {
        cout << "   Warning: " << e.what() << "\n";
    }

    printHeader("WELCOME TO NAGARSEVAK CONNECT");
    cout << "\n   Pune Municipal Corporation - Smart Civic Platform\n";
    cout << "   Built with C++ OOP Architecture\n";
    cout << "   Issues loaded from disk: " << manager.getCount() << "\n";
    cout << "   Total Report objects alive: " << Report::getTotalReports() << "\n";

    // Display Ward Directory at startup
    printHeader("PUNE MUNICIPAL CORPORATION - WARD DIRECTORY");
    cout << "\n   Find your ward number below:\n\n";
    cout << "   Ward 1  - Vishrambaug          Ward 2  - Kasba Peth\n";
    cout << "   Ward 3  - Raviwar Peth         Ward 4  - Somwar Peth\n";
    cout << "   Ward 5  - Mangalwar Peth       Ward 6  - Guruwar Peth\n";
    cout << "   Ward 7  - Shukrawar Peth       Ward 8  - Sadashiv Peth\n";
    cout << "   Ward 9  - Narayan Peth         Ward 10 - Nana Peth\n";
    cout << "   Ward 11 - Ganj Peth            Ward 12 - Bhavani Peth\n";
    cout << "   Ward 13 - Dhole Patil Nagar    Ward 14 - Yerawada\n";
    cout << "   Ward 15 - Shivajinagar         Ward 16 - Hadapsar\n";
    cout << "   Ward 17 - Mundhwa              Ward 18 - Kondhwa\n";
    cout << "   Ward 19 - Bibwewadi            Ward 20 - Dhankawadi\n";
    cout << "   Ward 21 - Sahakarnagar         Ward 22 - Parvati\n";
    cout << "   Ward 23 - Sinhagad Road        Ward 24 - Warje\n";
    cout << "   Ward 25 - Karve Nagar          Ward 26 - Erandwane\n";
    cout << "   Ward 27 - Model Colony         Ward 28 - Aundh\n";
    cout << "   Ward 29 - Deccan Gymkhana      Ward 30 - Baner\n";
    cout << "   Ward 31 - Kothrud              Ward 32 - Bavdhan\n";
    cout << "   Ward 33 - Pashan               Ward 34 - Sus\n";
    cout << "   Ward 35 - Hinjewadi            Ward 36 - Wakad\n";
    cout << "   Ward 37 - Pimpri               Ward 38 - Chinchwad\n";
    cout << "   Ward 39 - Nigdi                Ward 40 - Akurdi\n";
    cout << "   Ward 41 - Sangvi\n";
    cout << "\n   Use your ward number while reporting issues.\n";

    int choice;
    int issueCounter = manager.getCount();

    do {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            choice = -1;
        }

        switch (choice) {

            // ---- REPORT NEW ISSUE ----
            case 1: {
                printHeader("REPORT NEW CIVIC ISSUE");
                cin.ignore(INT_MAX, '\n');

                string title, desc, ward, category, loc;

                cout << "   Title: ";
                getline(cin, title);
                cout << "   Description: ";
                getline(cin, desc);
                cout << "   Ward (e.g. 29 Deccan Gymkhana): ";
                getline(cin, ward);
                cout << "   Category (Pothole/Garbage/Water/Electricity/Other): ";
                getline(cin, category);
                cout << "   Location: ";
                getline(cin, loc);

                if (title.empty() || ward.empty()) {
                    cout << "\n   Error: Title and Ward are required!\n";
                    break;
                }

                try {
                    issueCounter++;
                    string newId = "ISSUE-" + to_string(issueCounter);

                    // Dynamic memory allocation (new)
                    CivicIssue* newIssue = new CivicIssue(newId, title, desc, ward, category, loc);
                    newIssue->logCreation();
                    manager.addIssue(newIssue);
                    manager.saveToFile("issues.txt");

                    cout << "\n   Issue reported successfully!\n";
                    cout << "   " << *newIssue << "\n";  // Friend operator<<
                } catch (const FileException& e) {
                    cerr << "   Save Error: " << e.what() << "\n";
                } catch (const exception& e) {
                    cerr << "   Error: " << e.what() << "\n";
                }
                break;
            }

            // ---- VIEW ALL ISSUES ----
            case 2:
                printHeader("ALL REPORTED ISSUES");
                manager.displayAll();  // Overloaded version (no args)
                break;

            // ---- VIEW WITH PRIORITIES ----
            case 3:
                printHeader("ISSUES WITH PRIORITY ANALYSIS");
                manager.displayAll(true);  // Overloaded version (with bool)
                break;

            // ---- UPVOTE AN ISSUE ----
            case 4: {
                printHeader("UPVOTE AN ISSUE");
                if (manager.getCount() == 0) {
                    cout << "   No issues to upvote.\n";
                    break;
                }
                manager.displayAll();
                cin.ignore(INT_MAX, '\n');
                cout << "\n   Enter Issue ID to upvote (e.g. ISSUE-1): ";
                string targetId;
                getline(cin, targetId);

                CivicIssue* found = manager.findById(targetId);
                if (found) {
                    ++(*found);  // Unary operator++ overloading
                    try {
                        manager.saveToFile("issues.txt");
                    } catch (const FileException& e) {
                        cerr << "   " << e.what() << "\n";
                    }
                    cout << "   Upvoted! " << *found << "\n";
                    printIssueUrgency(*found);  // Friend function
                } else {
                    cout << "   Issue '" << targetId << "' not found.\n";
                }
                break;
            }

            // ---- SEARCH BY ID ----
            case 5: {
                printHeader("SEARCH ISSUE BY ID");
                cin.ignore(INT_MAX, '\n');
                cout << "   Enter Issue ID: ";
                string searchId;
                getline(cin, searchId);

                CivicIssue* result = manager.findById(searchId);
                if (result) {
                    cout << "\n   Found:\n";
                    result->display();       // Virtual function call
                    printIssueUrgency(*result); // Friend function
                } else {
                    cout << "   Issue '" << searchId << "' not found.\n";
                }
                break;
            }

            // ---- CLEAR ALL ----
            case 6:
                printHeader("CLEAR ALL ISSUES");
                manager.clearMemory();
                try {
                    manager.saveToFile("issues.txt");
                    cout << "   All issues cleared from system!\n";
                    cout << "   Reports alive: " << Report::getTotalReports() << "\n";
                } catch (const FileException& e) {
                    cout << "   Error: " << e.what() << "\n";
                }
                break;

            // ---- POLYMORPHISM DEMO ----
            case 7: {
                printHeader("POLYMORPHISM DEMONSTRATION");
                cout << "\n   Creating objects via base class pointer array...\n";

                // Array of pointers to base class (Runtime Polymorphism)
                Report* arr[3];
                arr[0] = new CivicIssue("DEMO-1", "Pothole on FC Road",
                    "Large crater near signal", "29 Deccan Gymkhana", "Pothole", "FC Road");
                arr[1] = new UrgentIssue("URG-1", "Bridge Collapse Risk",
                    "Cracks on flyover", "16 Hadapsar", "Emergency", "Hadapsar Flyover", "CRITICAL");
                arr[2] = new CivicIssue("DEMO-2", "Garbage Overflow",
                    "Bins not cleared 3 days", "31 Kothrud", "Garbage", "Kothrud Depot");

                // dynamic_cast + operator++ demo
                CivicIssue* d1 = dynamic_cast<CivicIssue*>(arr[0]);
                if (d1) {
                    for (int v = 0; v < 7; v++) ++(*d1);
                    cout << "   [Operator++] Applied 7 upvotes to DEMO-1\n";
                }

                // Virtual function calls (late binding)
                cout << "\n   -- Polymorphic Display (late binding) --\n";
                for (int i = 0; i < 3; i++) {
                    cout << "\n";
                    arr[i]->display();     // Calls correct overridden version
                    arr[i]->logCreation(); // Interface method
                }

                // Binary operator< demo
                CivicIssue* d2 = dynamic_cast<CivicIssue*>(arr[2]);
                if (d1 && d2) {
                    cout << "\n   [Operator<] DEMO-1 > DEMO-2? "
                         << ((*d2 < *d1) ? "Yes" : "No") << "\n";
                    cout << "   [Stream<<] " << *d1 << "\n";
                    cout << "   [Stream<<] " << *d2 << "\n";
                }

                // Friend function
                if (d1) {
                    cout << "   [Friend Fn] ";
                    printIssueUrgency(*d1);
                }

                // Clean up dynamic memory (delete)
                cout << "\n   Cleaning up dynamic memory (delete)...\n";
                for (int i = 0; i < 3; i++) delete arr[i];
                cout << "   Memory freed. Reports alive: " << Report::getTotalReports() << "\n";
                break;
            }

            // ---- COPY CONSTRUCTOR DEMO ----
            case 8: {
                printHeader("COPY CONSTRUCTOR DEMONSTRATION");
                cout << "\n   Creating original CivicIssue...\n";
                CivicIssue original("COPY-1", "Water Leak", "Pipe burst on MG Road",
                                    "15 Shivajinagar", "Water", "MG Road");
                original.display();
                cout << "   Reports alive after original: " << Report::getTotalReports() << "\n";

                cout << "\n   Creating copy using Copy Constructor...\n";
                CivicIssue copied(original);  // Copy constructor invoked
                copied.display();
                cout << "   Reports alive after copy: " << Report::getTotalReports() << "\n";

                cout << "\n   Both objects will be destroyed at end of this block.\n";
                break;
            }

            // ---- OOP STATS ----
            case 9:
                printHeader("OOP SYSTEM STATISTICS");
                cout << "\n   Reports Currently Alive : " << Report::getTotalReports() << "\n";
                cout << "   Lifetime Reports Created: " << Report::getLifetimeReports() << "\n";
                cout << "   Issues in Manager       : " << manager.getCount() << "\n";
                break;

            // ---- EXIT ----
            case 0:
                cout << "\n   Exiting Nagarsevak Connect. Jai Hind!\n\n";
                break;

            default:
                cout << "   Invalid option. Use 0-9.\n";
        }
    } while (choice != 0);

    return 0;
}
