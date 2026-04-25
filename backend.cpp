/*
 * ════════════════════════════════════════════════════════
 *   NAGARSEVAK CONNECT — C++ OOP Web Server Engine
 *   Pune Municipal Corporation Civic Issue Portal
 * ════════════════════════════════════════════════════════
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
#include <climits>
#include <ctime>
#include <iomanip>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    typedef int socklen_t;
#else
    #include <netinet/in.h>
    #include <unistd.h>
    #include <sys/socket.h>
#endif

using namespace std;

// ════════════════════════════════════════════════════════
// 9. EXCEPTION HANDLING — Custom Exception Hierarchy
// ════════════════════════════════════════════════════════

// Base custom exception (inherits from std::exception)
class ServerException : public exception {
    string message;
    string timestamp;
public:
    ServerException(string msg) : message(msg) {
        time_t now = time(nullptr);
        struct tm timeinfo;
#ifdef _WIN32
        localtime_s(&timeinfo, &now);
#else
        localtime_r(&now, &timeinfo);
#endif
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
        timestamp = string(buf);
    }
    
    const char* what() const noexcept override {
        return message.c_str();
    }
    
    inline string getTimestamp() const { return timestamp; }
};

class FileException : public ServerException {
public:
    FileException(string filename) 
        : ServerException("File I/O Error: Unable to access '" + filename + "'") {}
};

class NetworkException : public ServerException {
public:
    NetworkException(string detail) 
        : ServerException("Network Error: " + detail) {}
};

// ════════════════════════════════════════════════════════
// 1. CORE OOP — Abstract Interface (Abstraction)
// ════════════════════════════════════════════════════════

class ISerializable {
public:
    virtual string serialize() const = 0;
    virtual string toJSON() const = 0;
    virtual ~ISerializable() {}
};

class ILoggable {
public:
    virtual void logCreation() const = 0;
    virtual ~ILoggable() {}
};

// ════════════════════════════════════════════════════════
// 3. CLASSES, OBJECTS & STATIC MEMBERS
// ════════════════════════════════════════════════════════

class Report : public ISerializable, public ILoggable {
protected:
    string id;
    string title;
    string description;
    string createdAt;
    string location;   // [Encapsulation] Location data
    static int totalReports;
    static int lifetimeReports;

public:
    Report() : id("0"), title("None"), description("None"), location("None") {
        totalReports++;
        lifetimeReports++;
        setTimestamp();
    }
    
    Report(string i, string t, string d) : id(i), title(t), description(d), location("None") {
        totalReports++;
        lifetimeReports++;
        setTimestamp();
    }
    
    Report(const Report& other) 
        : id(other.id), title(other.title), description(other.description), 
          createdAt(other.createdAt), location(other.location) {
        totalReports++;
        lifetimeReports++;
    }
    
    virtual ~Report() { 
        totalReports--; 
    }

    inline static int getTotalReports() { return totalReports; }
    inline static int getLifetimeReports() { return lifetimeReports; }
    
    inline string getId() const { return id; }
    inline string getTitle() const { return title; }

    // [Encapsulation] Location Setters/Getters
    inline void setLocation(string loc) {
        location = loc;
    }
    inline string getLocation() const { return location; }

    virtual void display() const = 0;
    virtual string toJSON() const = 0;
    
    void logCreation() const override {
        cout << "   [LOG] Report " << id << " created at " << createdAt << "\n";
    }

private:
    void setTimestamp() {
        time_t now = time(nullptr);
        struct tm timeinfo;
#ifdef _WIN32
        localtime_s(&timeinfo, &now);
#else
        localtime_r(&now, &timeinfo);
#endif
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
        createdAt = string(buf);
    }
};

int Report::totalReports = 0;
int Report::lifetimeReports = 0;

class CivicIssue;

// ════════════════════════════════════════════════════════
// 6. INHERITANCE — CivicIssue
// ════════════════════════════════════════════════════════

class CivicIssue : public Report {
protected:
    string ward;
    string category;
    int upvotes;

public:
    CivicIssue() : Report(), ward("Unknown"), category("Unknown"), upvotes(0) {}
    
    CivicIssue(string i, string t, string d, string w, string c, string loc = "None")
        : Report(i, t, d), ward(w), category(c), upvotes(0) {
            this->setLocation(loc);
        }

    void display() const override {
        cout << " [" << id << "] " << title << "\n"
             << "   Ward: " << ward << " | Category: " << category << "\n"
             << "   Location: " << location << "\n"
             << "   Desc: " << description << "\n"
             << "   Priority Votes: " << upvotes << "\n";
    }

    string serialize() const override {
        return id + "|" + title + "|" + description + "|" + ward + "|" + category + "|" + to_string(upvotes) + "|" + location;
    }

    string escapeJson(string s) const {
        string res;
        res.reserve(s.size() + 10);
        for (char c : s) {
            switch (c) {
                case '"':  res += "\\\""; break;
                case '\\': res += "\\\\"; break;
                case '\n': res += "\\n";  break;
                case '\r': break;
                case '\t': res += "\\t";  break;
                default:   res += c;      break;
            }
        }
        return res;
    }

    string toJSON() const override {
        return "{\"id\":\"" + id 
             + "\",\"title\":\"" + escapeJson(title) 
             + "\",\"ward\":\"" + escapeJson(ward) 
             + "\",\"category\":\"" + escapeJson(category) 
             + "\",\"description\":\"" + escapeJson(description) 
             + "\",\"location\":\"" + escapeJson(location) + "\""
             + ",\"upvotes\":" + to_string(upvotes) + "}";
    }

    void operator++() {
        this->upvotes += 1;
    }
    
    void operator++(int) {
        this->upvotes += 1;
    }

    bool operator<(const CivicIssue& other) const {
        return this->upvotes < other.upvotes;
    }
    
    bool operator==(const CivicIssue& other) const {
        return this->id == other.id;
    }

    friend ostream& operator<<(ostream& os, const CivicIssue& issue);
    friend void printIssueUrgency(const CivicIssue& issue);
};

ostream& operator<<(ostream& os, const CivicIssue& issue) {
    os << "[" << issue.id << "] " << issue.title 
       << " (" << issue.category << ", " << issue.ward << ") "
       << "▲" << issue.upvotes;
    return os;
}

void printIssueUrgency(const CivicIssue& issue) {
    if (issue.upvotes > 10)
        cout << "   🔴 [CRITICAL URGENCY — IMMEDIATE ACTION NEEDED]\n";
    else if (issue.upvotes > 5)
        cout << "   🟡 [High Priority — Escalated]\n";
    else if (issue.upvotes > 2)
        cout << "   🟢 [Medium Priority]\n";
    else
        cout << "   ⚪ [Normal Priority]\n";
}

// ════════════════════════════════════════════════════════
// 6. MULTILEVEL INHERITANCE
// ════════════════════════════════════════════════════════

class UrgentIssue : public CivicIssue {
    string urgencyLevel;
public:
    UrgentIssue(string i, string t, string d, string w, string c, string loc, string urg)
        : CivicIssue(i, t, d, w, c, loc), urgencyLevel(urg) {}

    void display() const override {
        cout << " 🚨 [EMERGENCY — " << urgencyLevel << "] " << title << "\n"
             << "   Ward: " << ward << " | Immediate action required!\n"
             << "   Location: " << location << "\n"
             << "   " << description << "\n";
    }

    string toJSON() const override {
        return "{\"id\":\"" + id 
             + "\",\"title\":\"" + escapeJson(title) 
             + "\",\"ward\":\"" + escapeJson(ward) 
             + "\",\"category\":\"" + escapeJson(category) 
             + "\",\"description\":\"" + escapeJson(description) 
             + "\",\"location\":\"" + escapeJson(location) + "\""
             + ",\"upvotes\":" + to_string(upvotes) 
             + ",\"urgency\":\"" + urgencyLevel + "\"}";
    }
};

// ════════════════════════════════════════════════════════
// 10. TEMPLATES & STL
// ════════════════════════════════════════════════════════

template <typename T>
class IssueManager {
private:
    vector<T*> issues;

public:
    ~IssueManager() {
        clearMemory();
    }

    void clearMemory() {
        typename vector<T*>::iterator it = issues.begin();
        while (it != issues.end()) {
            delete *it;
            it++;
        }
        issues.clear();
    }

    void addIssue(T* issue) {
        issues.push_back(issue);
    }

    void displayAll() {
        displayAll(false);
    }

    void displayAll(bool escalatePriorities) {
        if (issues.empty()) {
            cout << "   [No civic issues found in system]\n";
            return;
        }

        int count = 1;
        for (typename vector<T*>::iterator it = issues.begin(); it != issues.end(); ++it) {
            cout << " " << count++ << ". ";
            (*it)->display();
            
            if (escalatePriorities) {
                if (CivicIssue* c = dynamic_cast<CivicIssue*>(*it)) {
                    printIssueUrgency(*c);
                }
            }
        }
    }

    string getAllIssuesJSON() {
        string json = "[";
        for (size_t i = 0; i < issues.size(); ++i) {
            json += issues[i]->toJSON();
            if (i < issues.size() - 1) json += ",";
        }
        json += "]";
        return json;
    }

    size_t getCount() const {
        return issues.size();
    }
    
    void saveToFile(const string& filename) {
        ofstream out(filename);
        if (!out) {
            throw FileException(filename);
        }
        for (auto item : issues) {
            out << item->serialize() << "\n";
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
                T* newIssue = new CivicIssue(i, t, d, w, c, loc_str);
                
                int upvotes = u_str.empty() ? 0 : stoi(u_str);
                for (int k = 0; k < upvotes; k++) {
                    ++(*newIssue);
                }

                addIssue(newIssue);
            }
        }
        in.close();
    }
    
    T* findById(const string& targetId) {
        for (auto item : issues) {
            if (item->getId() == targetId) {
                return item;
            }
        }
        return nullptr;
    }
};

template <typename T>
void printHeader(T titleText) {
    cout << "\n=========================================\n";
    cout << "   " << titleText << "\n";
    cout << "=========================================\n";
}

// ════════════════════════════════════════════════════════
// BACKEND WEB SERVER — NagarsevakServer Class
// ════════════════════════════════════════════════════════

class NagarsevakServer {
private:
    int port;
    IssueManager<CivicIssue>& manager;

    string urlDecode(string str) {
        string ret;
        ret.reserve(str.size());
        char ch;
        int ii;
        for (size_t i = 0; i < str.length(); i++) {
            if (str[i] == '%') {
                if (i + 2 < str.length()) {
                    sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
                    ch = static_cast<char>(ii);
                    ret += ch;
                    i += 2;
                }
            } else if (str[i] == '+') {
                ret += ' ';
            } else {
                ret += str[i];
            }
        }
        return ret;
    }

    string getValue(const string& data, const string& key) {
        size_t start = data.find(key + "=");
        if (start == string::npos) return "";
        start += key.length() + 1;
        size_t end = data.find("&", start);
        if (end == string::npos) end = data.length();
        return urlDecode(data.substr(start, end - start));
    }

    string readFile(const string& filepath) {
        ifstream file(filepath);
        if (!file.is_open()) return "";
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    string getMimeType(const string& path) {
        if (path.find(".html") != string::npos) return "text/html";
        if (path.find(".css") != string::npos)  return "text/css";
        if (path.find(".js") != string::npos)   return "application/javascript";
        if (path.find(".json") != string::npos)  return "application/json";
        if (path.find(".png") != string::npos)   return "image/png";
        if (path.find(".jpg") != string::npos || path.find(".jpeg") != string::npos) return "image/jpeg";
        return "text/plain";
    }
    
    string buildResponse(int statusCode, const string& contentType, const string& body, 
                         const string& extraHeaders = "") {
        string status;
        switch (statusCode) {
            case 200: status = "200 OK"; break;
            case 404: status = "404 Not Found"; break;
            case 400: status = "400 Bad Request"; break;
            case 500: status = "500 Internal Server Error"; break;
            default:  status = to_string(statusCode); break;
        }
        
        return "HTTP/1.1 " + status + "\r\n"
             + "Content-Type: " + contentType + "\r\n"
             + "Connection: close\r\n"
             + "Content-Length: " + to_string(body.length()) + "\r\n"
             + "Access-Control-Allow-Origin: *\r\n"
             + extraHeaders
             + "\r\n"
             + body;
    }

    void logRequest(const string& method, const string& path) {
        time_t now = time(nullptr);
        struct tm timeinfo;
#ifdef _WIN32
        localtime_s(&timeinfo, &now);
#else
        localtime_r(&now, &timeinfo);
#endif
        char timeBuf[32];
        strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &timeinfo);
        cout << " [" << timeBuf << "] " << method << " " << path << "\n";
    }

public:
    NagarsevakServer(int p, IssueManager<CivicIssue>& m) : port(p), manager(m) {}

    void start() {
        try {
#ifdef _WIN32
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                throw NetworkException("WSA/Socket initialization failed.");
            }
            SOCKET server_fd, new_socket;
#else
            int server_fd, new_socket;
            int opt = 1;
#endif

            struct sockaddr_in address;
            int addrlen = sizeof(address);

            server_fd = socket(AF_INET, SOCK_STREAM, 0);

#ifndef _WIN32
            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
                cerr << "setsockopt Error\n";
            }
#endif

            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(port);

            if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
                throw NetworkException("Bind failed. Port " + to_string(port) + " might be in use.");
            }

            listen(server_fd, 10);

            printHeader("NAGARSEVAK CONNECT — SERVER ENGINE");
            cout << "\n";
            cout << " ┌─────────────────────────────────────────┐\n";
            cout << " │  🏛️  Nagarsevak Connect Server v2.0      │\n";
            cout << " │  📡  Port: " << port << "                           │\n";
            cout << " │  📋  Issues Loaded: " << setw(4) << left << manager.getCount() << "                 │\n";
            cout << " │  🔗  http://localhost:" << port << "/              │\n";
            cout << " │  ⏹️   Press Ctrl+C to stop               │\n";
            cout << " └─────────────────────────────────────────┘\n\n";
            cout << " OOP Concepts Active:\n";
            cout << "   ✅ Inheritance (Multi-level)  ✅ Polymorphism (Virtual)\n";
            cout << "   ✅ Encapsulation (Access)     ✅ Abstraction (Interface)\n";
            cout << "   ✅ Templates & STL            ✅ Exception Handling\n";
            cout << "   ✅ Operator Overloading        ✅ Friend Functions\n";
            cout << "   ✅ Dynamic Memory (new/del)   ✅ Static Members\n\n";
            cout << " Listening for requests...\n\n";

            while (true) {
#ifdef _WIN32
                new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
#else
                new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
#endif
                if (new_socket < 0) continue;

                char buffer[30000] = {0};
                int valread;
#ifdef _WIN32
                valread = recv(new_socket, buffer, 30000, 0);
#else
                valread = read(new_socket, buffer, 30000);
#endif

                if (valread <= 0) {
#ifdef _WIN32
                    closesocket(new_socket);
#else
                    close(new_socket);
#endif
                    continue;
                }

                string request(buffer);
                string response;

                if (request.find("GET / ") != string::npos || request.find("GET /index.html") != string::npos) {
                    logRequest("GET", "/");
                    string content = readFile("index.html");
                    response = content.empty()
                        ? buildResponse(404, "text/plain", "index.html not found.")
                        : buildResponse(200, "text/html", content);
                }
                else if (request.find("GET /style.css") != string::npos) {
                    logRequest("GET", "/style.css");
                    string content = readFile("style.css");
                    response = content.empty()
                        ? buildResponse(404, "text/plain", "style.css not found.")
                        : buildResponse(200, "text/css", content);
                }
                else if (request.find("GET /api/issues") != string::npos) {
                    logRequest("GET", "/api/issues");
                    string jsonPayload = manager.getAllIssuesJSON();
                    response = buildResponse(200, "application/json", jsonPayload);
                }
                else if (request.find("POST /api/upvote") != string::npos) {
                    size_t idStart = request.find("id=");
                    string issueId = "";
                    if (idStart != string::npos) {
                        idStart += 3;
                        size_t idEnd = request.find_first_of(" &\r\n", idStart);
                        if (idEnd == string::npos) idEnd = request.length();
                        issueId = request.substr(idStart, idEnd - idStart);
                    }
                    
                    logRequest("POST", "/api/upvote?id=" + issueId);
                    
                    CivicIssue* found = manager.findById(issueId);
                    if (found) {
                        ++(*found);
                        manager.saveToFile("issues.txt");
                        cout << "   ▲ Upvoted: " << *found << "\n";
                        response = buildResponse(200, "application/json", 
                            "{\"status\":\"success\",\"message\":\"Upvoted " + issueId + "\"}");
                    } else {
                        response = buildResponse(404, "application/json", 
                            "{\"status\":\"error\",\"message\":\"Issue not found\"}");
                    }
                }
                else if (request.find("POST /submit") != string::npos) {
                    logRequest("POST", "/submit");
                    size_t body_pos = request.find("\r\n\r\n");
                    if (body_pos != string::npos) {
                        string body = request.substr(body_pos + 4);
                        string title = getValue(body, "title");
                        string ward = getValue(body, "ward");
                        string category = getValue(body, "category");
                        string description = getValue(body, "description");
                        string locationStr = getValue(body, "location");

                        if (!title.empty() && !ward.empty()) {
                            string newId = "ISSUE-" + to_string(Report::getTotalReports() + 1);

                            CivicIssue* newIssue = new CivicIssue(newId, title, description, ward, category, locationStr);
                            newIssue->logCreation();
                            manager.addIssue(newIssue);
                            manager.saveToFile("issues.txt");

                            cout << "   ✅ New Issue: " << *newIssue << "\n";
                        }

                        response = buildResponse(200, "application/json",
                            "{\"status\":\"success\",\"message\":\"Issue Reported Successfully\"}");
                    }
                }
                else {
                    logRequest("???", "Unknown Route");
                    response = buildResponse(404, "text/plain", "Not Found.");
                }

                send(new_socket, response.c_str(), response.length(), 0);

#ifdef _WIN32
                closesocket(new_socket);
#else
                close(new_socket);
#endif
            }
#ifdef _WIN32
            WSACleanup();
#endif
        } catch (const NetworkException& e) {
            cerr << " 🔴 NETWORK ERROR [" << e.getTimestamp() << "]: " << e.what() << "\n";
        } catch (const FileException& e) {
            cerr << " 🔴 FILE ERROR [" << e.getTimestamp() << "]: " << e.what() << "\n";
        } catch (const ServerException& e) {
            cerr << " 🔴 SERVER ERROR [" << e.getTimestamp() << "]: " << e.what() << "\n";
        } catch (const exception& e) {
            cerr << " 🔴 SYSTEM ERROR: " << e.what() << "\n";
        }
    }
};

// ════════════════════════════════════════════════════════
// MAIN — Menu-Driven Admin Console
// ════════════════════════════════════════════════════════

void showMenu() {
    printHeader("NAGARSEVAK CONNECT — ADMIN CONSOLE");
    cout << "\n";
    cout << " ┌──────────────────────────────────┐\n";
    cout << " │  1. 🌐  Start Web Server         │\n";
    cout << " │  2. 📋  View All Issues           │\n";
    cout << " │  3. 🔥  View with Priorities      │\n";
    cout << " │  4. 🗑️   Clear All Issues          │\n";
    cout << " │  5. 🧪  Demo Polymorphism         │\n";
    cout << " │  6. 📊  Show OOP Stats            │\n";
    cout << " │  7. ❌  Exit                      │\n";
    cout << " └──────────────────────────────────┘\n";
    cout << "\n Enter your choice: ";
}

int main(int argc, char* argv[]) {
    IssueManager<CivicIssue> manager;
    manager.loadFromFile("issues.txt");

    // Automatically start server on port 3000 if --server flag is passed
    if (argc > 1 && string(argv[1]) == "--server") {
        NagarsevakServer server(3000, manager);
        server.start();
        return 0;
    }

    NagarsevakServer server(8080, manager);

    printHeader("WELCOME TO NAGARSEVAK CONNECT");
    cout << "\n   Pune Municipal Corporation — Smart Civic Platform\n";
    cout << "   Built with C++ OOP Architecture\n";
    cout << "   Issues loaded from disk: " << manager.getCount() << "\n";
    cout << "   Total Report objects alive: " << Report::getTotalReports() << "\n";

    int choice;
    do {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            choice = 0;
        }

        switch (choice) {
            case 1:
                server.start();
                break;
                
            case 2:
                printHeader("ALL REPORTED ISSUES");
                manager.displayAll();
                break;
                
            case 3:
                printHeader("ISSUES WITH PRIORITY ANALYSIS");
                manager.displayAll(true);
                break;
                
            case 4:
                manager.clearMemory();
                try {
                    manager.saveToFile("issues.txt");
                    cout << "   ✅ All issues cleared from system!\n";
                } catch (const FileException& e) {
                    cout << "   ❌ " << e.what() << "\n";
                }
                break;
                
            case 5:
                printHeader("POLYMORPHISM DEMONSTRATION");
                cout << "\n   Creating objects via polymorphic base pointer array...\n\n";
                {
                    Report* mockArray[3];
                    mockArray[0] = new CivicIssue("DEMO-1", "Pothole on FC Road", "Large crater near signal", "29 Deccan Gymkhana", "Pothole", "FC Road");
                    mockArray[1] = new UrgentIssue("URG-1", "Bridge Collapse Risk", "Cracks observed on flyover", "16 Hadapsar", "Emergency", "Hadapsar Flyover", "CRITICAL");
                    mockArray[2] = new CivicIssue("DEMO-2", "Garbage Overflow", "Bins not cleared for 3 days", "31 Kothrud", "Garbage", "Kothrud Depot");

                    CivicIssue* demo1 = dynamic_cast<CivicIssue*>(mockArray[0]);
                    if (demo1) {
                        for (int v = 0; v < 7; v++) ++(*demo1);
                        cout << "   [Operator++] Applied 7 upvotes to DEMO-1\n";
                    }

                    cout << "\n   ── Polymorphic Display (late binding) ──\n\n";
                    for (int i = 0; i < 3; i++) {
                        mockArray[i]->display();
                        mockArray[i]->logCreation();
                        cout << "\n";
                    }

                    CivicIssue* demo2 = dynamic_cast<CivicIssue*>(mockArray[2]);
                    if (demo1 && demo2) {
                        cout << "   [Operator<] DEMO-1 > DEMO-2? " 
                             << ((*demo2 < *demo1) ? "Yes" : "No") << "\n";
                        cout << "   [Stream<<] " << *demo1 << "\n";
                        cout << "   [Stream<<] " << *demo2 << "\n";
                    }

                    if (demo1) {
                        cout << "   [Friend Fn] ";
                        printIssueUrgency(*demo1);
                    }

                    cout << "\n   Cleaning up dynamic memory...\n";
                    for (int i = 0; i < 3; i++) {
                        delete mockArray[i];
                    }
                    cout << "   ✅ Memory freed. Reports alive: " << Report::getTotalReports() << "\n";
                }
                break;
                
            case 6:
                printHeader("OOP SYSTEM STATISTICS");
                cout << "\n   ┌────────────────────────────────────┐\n";
                cout << "   │ Reports Currently Alive : " << setw(6) << Report::getTotalReports() << "   │\n";
                cout << "   │ Lifetime Reports Created: " << setw(6) << Report::getLifetimeReports() << "   │\n";
                cout << "   │ Issues in Manager       : " << setw(6) << manager.getCount() << "   │\n";
                cout << "   └────────────────────────────────────┘\n";
                break;
                
            case 7:
                cout << "\n   Exiting Nagarsevak Connect. Jai Hind! 🇮🇳\n\n";
                break;
                
            default:
                cout << "   ⚠️  Invalid option. Use 1-7.\n";
        }
    } while (choice != 7);

    return 0;
}
