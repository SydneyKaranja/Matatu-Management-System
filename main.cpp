/*
 ============================================================
  NAIROBI MATATU MANAGEMENT SYSTEM  —  THIKA ROAD (Route 45)
  Data Structures: Queue  |  Stack  |  Tree  |  Graph
  Compiler : C++11 or later
  IDE      : Code::Blocks  (set C++11 in Project > Build Options)
 ============================================================
*/

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <climits>

using namespace std;


// ═══════════════════════════════════════════════════════════
//  GLOBAL ROUTE DATA  —  Thika Road stages, fares, distances
// ═══════════════════════════════════════════════════════════

const vector<string> VALID_STAGES = {
    "CBD", "Ngara", "Allsops", "Garden City", "Roysambu",
    "Githurai", "Kahawa Wendani", "Kahawa Sukari", "Bypass",
    "Ruiru", "Kimbo", "Toll", "Kenyatta Road", "Juja",
    "Witeithe", "Thika", "Makongeni"
};

// Fare from CBD (KES)
const map<string,int> STAGE_FARES = {
    {"CBD",0},{"Ngara",20},{"Allsops",40},{"Garden City",50},
    {"Roysambu",50},{"Githurai",50},{"Kahawa Wendani",50},
    {"Kahawa Sukari",50},{"Bypass",50},{"Ruiru",80},
    {"Kimbo",80},{"Toll",80},{"Kenyatta Road",80},
    {"Juja",80},{"Witeithe",100},{"Thika",100},{"Makongeni",130}
};

struct EdgeDef { string from, to; int km; };
const vector<EdgeDef> ROAD_EDGES = {
    {"CBD",           "Ngara",          2},
    {"Ngara",         "Allsops",        3},
    {"Allsops",       "Garden City",    4},
    {"Garden City",   "Roysambu",       3},
    {"Roysambu",      "Githurai",       4},
    {"Githurai",      "Kahawa Wendani", 3},
    {"Kahawa Wendani","Kahawa Sukari",  2},
    {"Kahawa Sukari", "Bypass",         3},
    {"Bypass",        "Ruiru",          5},
    {"Ruiru",         "Kimbo",          4},
    {"Kimbo",         "Toll",           3},
    {"Toll",          "Kenyatta Road",  3},
    {"Kenyatta Road", "Juja",           4},
    {"Juja",          "Witeithe",       5},
    {"Witeithe",      "Thika",          6},
    {"Thika",         "Makongeni",      4}
};


// ═══════════════════════════════════════════════════════════
//  UTILITY HELPERS
// ═══════════════════════════════════════════════════════════

// Title-case a string  (e.g. "garden city" -> "Garden City")
string toTitleCase(const string& s)
{
    string r = s;
    bool cap = true;
    for (char& c : r) {
        if (c == ' ') { cap = true; }
        else { c = cap ? (char)toupper(c) : (char)tolower(c); cap = false; }
    }
    return r;
}

// Case-insensitive match against valid stages; returns matched name or ""
string normaliseStage(const string& input)
{
    string lo = input;
    transform(lo.begin(), lo.end(), lo.begin(), ::tolower);

    for (const string& s : VALID_STAGES) {
        string sl = s;
        transform(sl.begin(), sl.end(), sl.begin(), ::tolower);
        if (sl == lo) return s;
    }
    return "";   // not found
}

void printLine(char c = '-', int n = 58) { cout << string(n, c) << "\n"; }

void printHeader(const string& t)
{
    cout << "\n";
    printLine('=');
    cout << "  " << t << "\n";
    printLine('=');
}

void printSection(const string& t)
{
    cout << "\n";
    printLine('-');
    cout << "  " << t << "\n";
    printLine('-');
}

// Read a trimmed line from stdin
string readLine(const string& prompt = "")
{
    if (!prompt.empty()) cout << prompt;
    string line;
    getline(cin, line);
    size_t a = line.find_first_not_of(" \t\r\n");
    size_t b = line.find_last_not_of(" \t\r\n");
    return (a == string::npos) ? "" : line.substr(a, b - a + 1);
}

// Read an integer in [lo, hi] safely — no crash on bad input
int readInt(const string& prompt, int lo, int hi)
{
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        istringstream ss(line);
        int val;
        if (ss >> val && val >= lo && val <= hi) return val;
        cout << "  [!] Please enter a number between " << lo << " and " << hi << ".\n";
    }
}

void pressEnter()
{
    cout << "\n  Press Enter to continue...";
    // consume any leftover newline then wait
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


// ═══════════════════════════════════════════════════════════
//  DATA STRUCTURE 1 — QUEUE   (STL queue, FIFO boarding)
// ═══════════════════════════════════════════════════════════
class PassengerQueue
{
    string stageName;
    queue<string> q;
    int totalBoarded;
public:
    PassengerQueue() : stageName("(none)"), totalBoarded(0) {}
    explicit PassengerQueue(const string& stage)
        : stageName(stage), totalBoarded(0) {}

    void setStage(const string& s) { stageName = s; }
    string getStage()  const { return stageName; }
    bool   isEmpty()   const { return q.empty(); }
    int    waiting()   const { return (int)q.size(); }

    bool addPassenger(const string& name)
    {
        if (name.empty()) {
            cout << "  [!] Error: passenger name cannot be empty.\n";
            return false;
        }
        q.push(name);
        cout << "  [OK] " << name << " joined the queue at " << stageName << ".\n";
        return true;
    }

    bool boardNext()
    {
        if (q.empty()) {
            cout << "  [!] Error: no passengers waiting at " << stageName << ".\n";
            return false;
        }
        string p = q.front(); q.pop();
        totalBoarded++;
        cout << "  [OK] " << p << " has boarded the matatu.\n";
        cout << "       " << q.size() << " passenger(s) still waiting.\n";
        return true;
    }

    void display() const
    {
        cout << "  Stage        : " << stageName << "\n";
        cout << "  Waiting      : " << q.size()
             << "  |  Total boarded so far: " << totalBoarded << "\n";
        if (q.empty()) { cout << "  Queue        : [empty]\n"; return; }

        queue<string> tmp = q;          // copy — queue has no iterator
        cout << "  Queue (FIFO) : ";
        bool first = true;
        while (!tmp.empty()) {
            if (!first) cout << " -> ";
            cout << tmp.front();
            tmp.pop();
            first = false;
        }
        cout << "\n";
    }

    void clear() { while (!q.empty()) q.pop(); }
};


// ═══════════════════════════════════════════════════════════
//  DATA STRUCTURE 2 — STACK   (STL stack, route history)
// ═══════════════════════════════════════════════════════════
class RouteStack
{
    string vehicleId;
    stack<string> s;
public:
    explicit RouteStack(const string& vid = "KBZ-045G") : vehicleId(vid) {}

    bool isEmpty()  const { return s.empty(); }
    string current() const { return s.empty() ? "" : s.top(); }

    bool visitStage(const string& stage)
    {
        if (stage.empty()) {
            cout << "  [!] Error: stage name cannot be empty.\n";
            return false;
        }
        s.push(stage);
        cout << "  [OK] " << vehicleId << " arrived at: " << stage << "\n";
        return true;
    }

    bool backtrack()
    {
        if (s.empty()) {
            cout << "  [!] Error: no route history — nothing to backtrack from.\n";
            return false;
        }
        string last = s.top(); s.pop();
        cout << "  [<-] Backtracked from: " << last << "\n";
        cout << "       Now at : " << (s.empty() ? "Terminus (no further history)" : s.top()) << "\n";
        return true;
    }

    void display() const
    {
        cout << "  Vehicle : " << vehicleId << "\n";
        if (s.empty()) { cout << "  History : [none yet]\n"; return; }

        stack<string> tmp = s;
        vector<string> v;
        while (!tmp.empty()) { v.push_back(tmp.top()); tmp.pop(); }
        reverse(v.begin(), v.end());

        cout << "  History : ";
        for (int i = 0; i < (int)v.size(); i++) {
            if (i) cout << " -> ";
            cout << v[i];
        }
        cout << "\n  Current : " << v.back() << "\n";
    }
};


// ═══════════════════════════════════════════════════════════
//  DATA STRUCTURE 3 — TREE   (fare hierarchy, manual tree)
// ═══════════════════════════════════════════════════════════
struct TreeNode
{
    string name;
    int    fare;                       // -1 = not a leaf (hub/route node)
    vector<TreeNode*> children;

    TreeNode(const string& n, int f = -1) : name(n), fare(f) {}
    ~TreeNode() { for (TreeNode* c : children) delete c; }

    TreeNode* addChild(const string& n, int f = -1)
    {
        TreeNode* c = new TreeNode(n, f);
        children.push_back(c);
        return c;
    }

    // DFS search by name
    TreeNode* find(const string& target) const
    {
        if (name == target) return const_cast<TreeNode*>(this);
        for (TreeNode* c : children) {
            TreeNode* r = c->find(target);
            if (r) return r;
        }
        return nullptr;
    }

    void print(int depth = 0) const
    {
        string indent(depth * 4, ' ');
        string prefix = (depth > 0) ? "└── " : "";
        cout << indent << prefix << name;
        if (fare >= 0) cout << "   [KES " << fare << "]";
        cout << "\n";
        for (const TreeNode* c : children) c->print(depth + 1);
    }
};

class FareTree
{
    TreeNode* root;
public:
    FareTree()
    {
        root = new TreeNode("Nairobi County");
        TreeNode* hub   = root->addChild("CBD Hub");
        TreeNode* route = hub->addChild("Thika Road  (Route 45)");

        for (const string& s : VALID_STAGES) {
            int f = STAGE_FARES.count(s) ? STAGE_FARES.at(s) : -1;
            route->addChild(s, f);
        }
    }
    ~FareTree() { delete root; }

    // Returns fare >= 0, or -1 (hub node, no fare), or -2 (not found)
    int getFare(const string& stage) const
    {
        TreeNode* n = root->find(stage);
        if (!n) return -2;
        return n->fare;
    }

    void display() const { root->print(); }
};


// ═══════════════════════════════════════════════════════════
//  DATA STRUCTURE 4 — GRAPH  (adjacency list + Dijkstra)
// ═══════════════════════════════════════════════════════════
class MatatuGraph
{
    map<string, vector<pair<string,int>>> adj;
public:
    bool addEdge(const string& a, const string& b, int km)
    {
        if (km <= 0) {
            cout << "  [!] Error: distance must be a positive number.\n";
            return false;
        }
        adj[a].push_back({b, km});
        adj[b].push_back({a, km});
        return true;
    }

    bool hasStage(const string& s) const { return adj.count(s) > 0; }

    // Returns {total_distance, path}. distance = -1 if no route found.
    pair<int, vector<string>> shortestPath(const string& start,
                                           const string& end) const
    {
        if (!hasStage(start) || !hasStage(end)) return {-1, {}};
        if (start == end) return {0, {start}};

        map<string,int>    dist;
        map<string,string> prev;
        for (auto& kv : adj) { dist[kv.first] = INT_MAX; prev[kv.first] = ""; }
        dist[start] = 0;

        // Min-heap: (distance, node_name)
        priority_queue<pair<int,string>,
                       vector<pair<int,string>>,
                       greater<pair<int,string>>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            auto top = pq.top(); pq.pop();
            int  d   = top.first;
            string u = top.second;
            if (d > dist[u]) continue;
            for (size_t i = 0; i < adj.at(u).size(); i++) {
                string v = adj.at(u)[i].first;
                int    w = adj.at(u)[i].second;
                int   nd = d + w;
                if (nd < dist[v]) {
                    dist[v] = nd;
                    prev[v] = u;
                    pq.push({nd, v});
                }
            }
        }

        if (dist[end] == INT_MAX) return {-1, {}};

        // Reconstruct path
        vector<string> path;
        for (string cur = end; !cur.empty(); cur = prev[cur])
            path.push_back(cur);
        reverse(path.begin(), path.end());
        return {dist[end], path};
    }

    void display() const
    {
        for (auto& kv : adj) {
            cout << "  " << left << setw(18) << kv.first << " -> ";
            for (size_t i = 0; i < kv.second.size(); i++) {
                if (i) cout << ", ";
                cout << kv.second[i].first << " (" << kv.second[i].second << "km)";
            }
            cout << "\n";
        }
    }
};

MatatuGraph buildNetwork()
{
    MatatuGraph g;
    for (const EdgeDef& e : ROAD_EDGES)
        g.addEdge(e.from, e.to, e.km);
    return g;
}


// ═══════════════════════════════════════════════════════════
//  BOARDING STAGE SELECTION  (required before main menu)
// ═══════════════════════════════════════════════════════════
string selectBoardingStage()
{
    printSection("SELECT YOUR BOARDING STAGE");
    cout << "  The following stages are available on Thika Road:\n\n  ";
    for (int i = 0; i < (int)VALID_STAGES.size(); i++) {
        cout << left << setw(18) << VALID_STAGES[i];
        if ((i + 1) % 3 == 0) cout << "\n  ";
    }
    cout << "\n\n";

    while (true) {
        string input = readLine("  Enter your boarding stage: ");
        if (input.empty()) {
            cout << "  [!] Stage name cannot be empty. Please try again.\n";
            continue;
        }
        string matched = normaliseStage(input);
        if (matched.empty()) {
            cout << "  [!] '" << input << "' is not a recognised Thika Road stage.\n";
            cout << "      Only stages along this route are supported at this time.\n";
            cout << "      Please enter a valid stage from the list above.\n\n";
        } else {
            cout << "  [OK] Boarding stage set to: " << matched << "\n";
            return matched;
        }
    }
}


// ═══════════════════════════════════════════════════════════
//  MENU HANDLERS
// ═══════════════════════════════════════════════════════════

// ── Queue menu ──────────────────────────────────────────
void menuQueue(PassengerQueue& pq)
{
    printSection("PASSENGER QUEUE  [Stage: " + pq.getStage() + "]");
    cout << "  1. Add passenger to queue\n";
    cout << "  2. Board next passenger (FIFO)\n";
    cout << "  3. Display current queue\n";
    cout << "  4. Clear queue\n";
    cout << "  0. Back to main menu\n";

    int ch = readInt("\n  Choice: ", 0, 4);
    switch (ch) {
        case 1: { string n = readLine("  Passenger name: "); pq.addPassenger(n); break; }
        case 2:   pq.boardNext();   break;
        case 3:   pq.display();     break;
        case 4:   pq.clear();
                  cout << "  [OK] Queue cleared.\n"; break;
        case 0:   break;
    }
}

// ── Stack menu ───────────────────────────────────────────
void menuStack(RouteStack& rs, const string& boarding)
{
    printSection("ROUTE HISTORY — STACK  [Vehicle: KBZ-045G]");
    cout << "  Your boarding stage : " << boarding << "\n\n";
    cout << "  1. Mark arrival at a stage (push)\n";
    cout << "  2. Backtrack — undo last stage (pop)\n";
    cout << "  3. Display full route history\n";
    cout << "  0. Back to main menu\n";

    int ch = readInt("\n  Choice: ", 0, 3);
    switch (ch) {
        case 1: {
            string input = readLine("  Stage name: ");
            if (input.empty()) { cout << "  [!] Stage name cannot be empty.\n"; break; }
            string matched = normaliseStage(input);
            if (matched.empty()) {
                cout << "  [!] '" << input << "' is not a valid Thika Road stage.\n";
                cout << "      Only stages along this route are recognised.\n";
            } else {
                rs.visitStage(matched);
            }
            break;
        }
        case 2: rs.backtrack(); break;
        case 3: rs.display();   break;
        case 0: break;
    }
}

// ── Fare tree menu ───────────────────────────────────────
void menuFare(const FareTree& ft, const string& boarding)
{
    printSection("FARE LOOKUP — TREE  [Boarding: " + boarding + "]");
    cout << "  1. Look up fare to a destination\n";
    cout << "  2. Display full fare hierarchy\n";
    cout << "  0. Back to main menu\n";

    int ch = readInt("\n  Choice: ", 0, 2);
    switch (ch) {
        case 1: {
            string input = readLine("  Destination stage: ");
            if (input.empty()) { cout << "  [!] Stage name cannot be empty.\n"; break; }

            string dest = normaliseStage(input);
            if (dest.empty()) {
                cout << "  [!] '" << input << "' is not a valid Thika Road stage.\n";
                cout << "      Only stages along this route are currently supported.\n";
                break;
            }
            int destFare    = ft.getFare(dest);
            int boardFare   = ft.getFare(boarding);

            if (destFare == -2) {
                cout << "  [!] Stage not found in fare tree.\n"; break;
            }
            int tripFare = abs(destFare - boardFare);

            cout << "\n";
            cout << "  Fare CBD -> " << left << setw(18) << boarding
                 << ": KES " << boardFare << "\n";
            cout << "  Fare CBD -> " << left << setw(18) << dest
                 << ": KES " << destFare << "\n";
            printLine('-', 44);
            cout << "  Your trip fare  (" << boarding << " -> " << dest
                 << ") : KES " << tripFare << "\n";
            break;
        }
        case 2:
            printSection("FARE HIERARCHY TREE");
            ft.display();
            break;
        case 0: break;
    }
}

// ── Graph / Dijkstra menu ────────────────────────────────
void menuGraph(const MatatuGraph& g, const string& boarding)
{
    printSection("SHORTEST PATH — GRAPH (DIJKSTRA)  [From: " + boarding + "]");
    cout << "  1. Find shortest path to a destination\n";
    cout << "  2. Display full stage network\n";
    cout << "  0. Back to main menu\n";

    int ch = readInt("\n  Choice: ", 0, 2);
    switch (ch) {
        case 1: {
            string input = readLine("  Destination stage: ");
            if (input.empty()) { cout << "  [!] Stage name cannot be empty.\n"; break; }

            string dest = normaliseStage(input);
            if (dest.empty()) {
                cout << "  [!] '" << input << "' is not a valid Thika Road stage.\n";
                cout << "      Only stages along this route are currently supported.\n";
                break;
            }
            if (dest == boarding) {
                cout << "  [i] You are already at " << boarding << ". No travel needed.\n";
                break;
            }

            pair<int, vector<string>> result = g.shortestPath(boarding, dest);
            int dist = result.first;
            vector<string>& path = result.second;

            if (dist < 0) {
                cout << "  [!] No route found between " << boarding << " and " << dest << ".\n";
            } else {
                cout << "\n  Shortest route:\n  ";
                for (int i = 0; i < (int)path.size(); i++) {
                    if (i) cout << " -> ";
                    cout << path[i];
                }
                cout << "\n\n";
                cout << "  Total distance : " << dist << " km\n";
                cout << "  Stages crossed : " << (int)path.size() - 1 << "\n";
            }
            break;
        }
        case 2:
            printSection("STAGE NETWORK (ADJACENCY LIST)");
            g.display();
            break;
        case 0: break;
    }
}

// ── All stages table ─────────────────────────────────────
void displayAllStages()
{
    printSection("THIKA ROAD — ALL STAGES & FARES FROM CBD");
    cout << "  " << left << setw(4) << "No."
         << setw(22) << "Stage"
         << "Fare from CBD\n";
    printLine('-', 44);
    for (int i = 0; i < (int)VALID_STAGES.size(); i++) {
        const string& s = VALID_STAGES[i];
        int f = STAGE_FARES.count(s) ? STAGE_FARES.at(s) : 0;
        cout << "  " << left << setw(4)  << (i + 1)
                     << setw(22) << s
                     << "KES " << f << "\n";
    }
}


// ═══════════════════════════════════════════════════════════
//  MAIN
// ═══════════════════════════════════════════════════════════
int main()
{
    printHeader("NAIROBI MATATU MANAGEMENT SYSTEM  —  THIKA ROAD (Route 45)");
    cout << "  Covers: CBD -> Ngara -> ... -> Thika -> Makongeni\n";
    cout << "  Data Structures: Queue | Stack | Tree | Graph\n";

    // ── Step 1: user must select a boarding stage first ──
    cout << "\n  You must select your boarding stage before using the system.\n";
    string boarding = selectBoardingStage();

    // ── Initialise the four data structures ──────────────
    PassengerQueue pq(boarding);
    RouteStack     rs("KBZ-045G");
    FareTree       ft;
    MatatuGraph    g = buildNetwork();

    // ── Main menu loop ────────────────────────────────────
    while (true) {
        printHeader("MAIN MENU  [Boarding stage: " + boarding + "]");
        cout << "  1. Passenger Queue        (Data Structure: Queue)\n";
        cout << "  2. Route History          (Data Structure: Stack)\n";
        cout << "  3. Fare Lookup            (Data Structure: Tree)\n";
        cout << "  4. Shortest Path          (Data Structure: Graph)\n";
        cout << "  5. View all route stages\n";
        cout << "  6. Change boarding stage\n";
        cout << "  0. Exit\n";

        int choice = readInt("\n  Select option [0-6]: ", 0, 6);

        switch (choice) {
            case 1: menuQueue(pq);            pressEnter(); break;
            case 2: menuStack(rs, boarding);  pressEnter(); break;
            case 3: menuFare(ft, boarding);   pressEnter(); break;
            case 4: menuGraph(g, boarding);   pressEnter(); break;
            case 5: displayAllStages();       pressEnter(); break;
            case 6: {
                string oldBoarding = boarding;
                boarding = selectBoardingStage();
                if (boarding != oldBoarding) {
                    pq.clear();
                    pq.setStage(boarding);
                    cout << "  [OK] Boarding stage updated. Queue has been reset.\n";
                }
                pressEnter();
                break;
            }
            case 0:
                cout << "\n  Sawa sawa! Safe travels on Thika Road.\n\n";
                return 0;
        }
    }
}
