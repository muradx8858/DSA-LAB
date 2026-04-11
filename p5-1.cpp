// SPRING 2026 DATA STRUCTURES SECTION D20 ASSIGNMENT ONE
// Name of Student: Aman Ali
// Registration of Student: L1F24BSCS0667
// File p5.cpp contains complete code of problem 5 (Bonus)
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

/* Question */
/* BONUS P3: Read Puzzle data from a file. Find and display ALL alternate paths
             from S to E and identify the shortest path.
   BONUS P4: Read Processors data from a file. Assign priorities (1 to N) to each
             process. Execute process with HIGHEST priority first each cycle.
             Decrement priority after each execution. Display priority in output. */

/* PROBLEM SOLUTION */
/*
   For Bonus P3:
   - Read maze from maze.txt (format: rows cols / numBlocked / blocked cell IDs)
   - Use recursive DFS with backtracking to find ALL paths from S to E.
   - Track and display the shortest path.

   For Bonus P4:
   - Read processes from processes.txt (format: N / timeSlice / executionTime jobDesc lines)
   - Assign priorities N, N-1, ..., 1 (first read = highest priority).
   - Use circular queue, each cycle pick highest priority process.
   - Decrement priority after each execution cycle.

   File formats:
   maze.txt:
     5 5
     4
     5 6 13 16

   processes.txt:
     3
     1
     4 reading file
     2 writing file
     6 compiling code
*/

const int MAX_CELLS = 105;

// ---- int Stack for path reconstruction ----
class Stack {
public:
    Stack(int cap = 0) {
        currentIndex = -1;
        maxCapacity = cap;
        values = new int[maxCapacity]{0};
    }
    virtual ~Stack() { delete[] values; }
    virtual void push(int v) = 0;
    virtual bool pop(int& v) = 0;
    bool isEmpty() { return (currentIndex < 0); }
    bool isFull()  { return (currentIndex == maxCapacity - 1); }
protected:
    int currentIndex;
    int* values;
    int maxCapacity;
};

class MyStack : public Stack {
public:
    MyStack(int cap = 0) : Stack(cap) {}
    void push(int v) {
        if (!isFull()) values[++currentIndex] = v;
    }
    bool pop(int& v) {
        if (!isEmpty()) { v = values[currentIndex--]; return true; }
        return false;
    }
};

// ---- Process class ----
class Process {
public:
    string id;
    int    executionTime;
    int    timeRemaining;
    string jobDescription;
    bool   isComplete;
    int    priority;

    Process() : id(""), executionTime(0), timeRemaining(0),
                jobDescription(""), isComplete(false), priority(0) {}

    Process(string _id, int _et, string _job, int _pri)
        : id(_id), executionTime(_et), timeRemaining(_et),
          jobDescription(_job), isComplete(false), priority(_pri) {}
};

// ---- Circular Queue for Process objects ----
class CircularQueue {
public:
    CircularQueue(int cap) {
        maxCapacity = cap;
        values = new Process[maxCapacity];
        front = rear = -1;
        count = 0;
    }
    ~CircularQueue() { delete[] values; }
    bool isEmpty() { return (count == 0); }
    bool isFull()  { return (count == maxCapacity); }
    void enqueue(Process p) {
        if (isFull()) return;
        if (isEmpty()) { front = rear = 0; }
        else { rear = (rear + 1) % maxCapacity; }
        values[rear] = p;
        count++;
    }
    Process dequeue() {
        Process p;
        if (isEmpty()) return p;
        p = values[front];
        count--;
        if (isEmpty()) { front = rear = -1; }
        else { front = (front + 1) % maxCapacity; }
        return p;
    }
private:
    Process* values;
    int front, rear, maxCapacity, count;
};

// ---- Utility ----
void printCell(int id, int eId) {
    if (id == 0)   { cout << "S"; return; }
    if (id == eId) { cout << "E"; return; }
    cout << id;
}

// ---- Recursive DFS to find ALL paths ----
void findAllPaths(int cur, int end, bool blocked[], bool onPath[],
                  int currentPath[], int pathLen, int rows, int cols,
                  int& pathCount, int& minLen, int shortestPath[], int& shortestLen) {
    if (cur == end) {
        pathCount++;
        cout << "  Path " << pathCount << " [" << pathLen << " steps]: ";
        for (int i = 0; i < pathLen; i++) {
            printCell(currentPath[i], end);
            if (i < pathLen - 1) cout << " -> ";
        }
        cout << endl;
        if (pathLen < minLen) {
            minLen = shortestLen = pathLen;
            for (int i = 0; i < pathLen; i++) shortestPath[i] = currentPath[i];
        }
        return;
    }
    int r = cur / cols, c = cur % cols;
    int neighbors[4] = {
        (c < cols - 1) ? cur + 1    : -1,
        (r < rows - 1) ? cur + cols : -1,
        (c > 0)        ? cur - 1    : -1,
        (r > 0)        ? cur - cols : -1
    };
    for (int i = 0; i < 4; i++) {
        int nb = neighbors[i];
        if (nb != -1 && !blocked[nb] && !onPath[nb]) {
            onPath[nb] = true;
            currentPath[pathLen] = nb;
            findAllPaths(nb, end, blocked, onPath, currentPath, pathLen + 1,
                         rows, cols, pathCount, minLen, shortestPath, shortestLen);
            onPath[nb] = false;
        }
    }
}

void runP3Bonus() {
    string filename;
    cout << "Enter maze filename (e.g. maze.txt): ";
    cin >> filename;

    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "ERROR: Cannot open '" << filename << "'" << endl;
        cout << "File format:" << endl;
        cout << "  Line 1: rows cols" << endl;
        cout << "  Line 2: number_of_blocked_cells" << endl;
        cout << "  Line 3: blocked_cell_ids" << endl;
        cout << "Example: 5 5 / 4 / 5 6 13 16" << endl;
        return;
    }

    int rows, cols;
    fin >> rows >> cols;
    int cells = rows * cols;

    bool blocked[MAX_CELLS] = {false};
    int numBlocked;
    fin >> numBlocked;
    for (int i = 0; i < numBlocked; i++) {
        int b; fin >> b;
        if (b > 0 && b < cells - 1) blocked[b] = true;
    }
    fin.close();

    cout << "\nMaze loaded: " << rows << " x " << cols << " (" << cells << " cells)" << endl;

    // Print the grid
    cout << "Grid (X=blocked):" << endl;
    for (int r = 0; r < rows; r++) {
        cout << "  ";
        for (int c = 0; c < cols; c++) {
            int id = r * cols + c;
            if (id == 0)         cout << setw(4) << "S";
            else if (id == cells - 1) cout << setw(4) << "E";
            else if (blocked[id]) cout << setw(4) << "X";
            else                  cout << setw(4) << id;
        }
        cout << endl;
    }

    bool onPath[MAX_CELLS]    = {false};
    int  currentPath[MAX_CELLS];
    int  shortestPath[MAX_CELLS];
    int  shortestLen = 0, minLen = 99999, pathCount = 0;
    for (int i = 0; i < MAX_CELLS; i++) currentPath[i] = shortestPath[i] = -1;

    onPath[0] = true;
    currentPath[0] = 0;

    cout << "\nAll paths from S to E:" << endl;
    findAllPaths(0, cells - 1, blocked, onPath, currentPath, 1,
                 rows, cols, pathCount, minLen, shortestPath, shortestLen);

    if (pathCount == 0) {
        cout << "No path exists." << endl;
        return;
    }

    cout << "\nTotal paths found: " << pathCount << endl;
    cout << "Shortest Path (" << shortestLen << " steps): ";
    for (int i = 0; i < shortestLen; i++) {
        printCell(shortestPath[i], cells - 1);
        if (i < shortestLen - 1) cout << " -> ";
    }
    cout << endl;
}

void runP4Bonus() {
    string filename;
    cout << "Enter processes filename (e.g. processes.txt): ";
    cin >> filename;

    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "ERROR: Cannot open '" << filename << "'" << endl;
        cout << "File format:" << endl;
        cout << "  Line 1: N (number of processes)" << endl;
        cout << "  Line 2: timeSlice" << endl;
        cout << "  Next N lines: executionTime jobDescription" << endl;
        return;
    }

    int N, T;
    fin >> N >> T;
    fin.ignore();

    CircularQueue CQ(500);

    for (int i = 1; i <= N; i++) {
        int et;
        string job;
        fin >> et;
        fin.ignore();
        getline(fin, job);

        string id = "p";
        for (int d = 0; d < 4; d++) id += char('0' + i);

        int priority = N + 1 - i;
        Process p(id, et, job, priority);
        CQ.enqueue(p);
    }
    fin.close();

    cout << "\nLoaded " << N << " processes. Time Slice = " << T << endl;
    cout << "\nPriority Simulation (Priority decrements each cycle):" << endl;
    cout << left
         << setw(5)  << "No."
         << setw(10) << "ProcessID"
         << setw(16) << "ExecutionTime"
         << setw(16) << "TimeRemaining"
         << setw(12) << "Priority"
         << "Status" << endl;
    cout << string(72, '-') << endl;

    int row = 1, totalTime = 0;

    while (!CQ.isEmpty()) {
        // Find highest priority process
        CircularQueue tempQ(N * 200);
        int maxPriority = -1;
        Process maxP;

        while (!CQ.isEmpty()) {
            Process p = CQ.dequeue();
            if (p.priority > maxPriority) {
                maxPriority = p.priority;
                maxP = p;
            }
            tempQ.enqueue(p);
        }

        // Restore all except the chosen one
        bool skipped = false;
        while (!tempQ.isEmpty()) {
            Process p = tempQ.dequeue();
            if (!skipped && p.id == maxP.id &&
                p.timeRemaining == maxP.timeRemaining &&
                p.priority == maxP.priority) {
                skipped = true;
                continue;
            }
            CQ.enqueue(p);
        }

        int usedPriority = maxP.priority;
        maxP.timeRemaining -= T;
        maxP.priority--;
        string status;

        if (maxP.timeRemaining <= 0) {
            maxP.timeRemaining = 0;
            maxP.isComplete    = true;
            status             = "completed";
        }
        else {
            status = "not complete";
            CQ.enqueue(maxP);
        }

        cout << left
             << setw(5)  << row
             << setw(10) << maxP.id
             << setw(16) << maxP.executionTime
             << setw(16) << maxP.timeRemaining
             << setw(12) << usedPriority
             << status   << endl;
        row++;
        totalTime++;
    }

    cout << "\nTotal Time taken = " << totalTime << endl;
    cout << "Average Time/Processor = " << fixed << setprecision(1)
         << (float)totalTime / N << endl;
}

int main() {
    cout << "=== Bonus P3: All Paths from File ===" << endl;
    runP3Bonus();

    cout << "\n=== Bonus P4: Priority Scheduling from File ===" << endl;
    runP4Bonus();

    return 0;
}
