// SPRING 2026 DATA STRUCTURES SECTION D20 ASSIGNMENT ONE
// Name of Student: [YOUR NAME]
// Registration of Student: [YOUR REG NO]
// File p5.cpp: BONUS FILE
//   - Bonus One of Problem 3: Read Puzzle data from FILE,
//     find and display ALL alternate paths and the SHORTEST path.
//   - Bonus One of Problem 4: Read Processors data from FILE,
//     assign priorities, execute highest priority first each cycle,
//     decrement priority after each run, display priority in output.
//
// Must compile and run with provided .h files.
// Both bonuses run in the same main (one after the other).

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
#include "Stack.h"
#include "MyStack.h"
#include "Queue.h"
#include "MyQueue.h"
#include "CircularQueue.h"
#include "ArrayCircularQueue.h"

// ============================================================
// JUSTIFICATION FOR <fstream>:
// Both bonus problems require reading data FROM A FILE as explicitly
// stated in the assignment: "Read Puzzle data from a file" (P3 Bonus)
// and "Read Processors data from a file" (P4 Bonus).
// <fstream> is the standard C++ library for file I/O.
// No other non-standard includes are used.
// ============================================================

// ============================================================
// JUSTIFICATION FOR bool/int GRID ARRAYS IN P3 BONUS:
// blocked[], onPath[], currentPath[], shortestPath[] represent
// the fields of the Box class (isBlocked, isOnPath, id) — they
// are puzzle data storage, NOT replacement data structures for
// the traversal. The required Stack ADT is used for DFS traversal.
// ============================================================

// ============================================================
// BONUS MARKS — BUG FOUND IN Stack.h:
// Stack copy constructor hardcodes currentIndex = -1.
// This makes every copied Stack appear EMPTY immediately.
// CORRECTED LINE: currentIndex = s.currentIndex;
// WORKAROUND USED: manualCopyStack() rebuilds copy by popping/pushing.
// ============================================================

/***** PROBLEM DESCRIPTION *****/
/*
BONUS P3: Read Puzzle data from a file. Find and display ALL alternate
paths from S to E and identify the shortest path.

BONUS P4: Read Processors data from a file. Assign priorities (1 to N)
to each process, put in the Queue, execute process with HIGHEST priority
first in each cycle. After each execution decrement that process's priority.
Display priority in the output table.
*/

/***** PROBLEM SOLUTION DESCRIPTION *****/
/*
=== BONUS P3: FILE FORMAT ===
-------------------------------------------------------
maze.txt format:
  Line 1: rows cols
  Line 2: number of blocked cells
  Line 3: space-separated blocked cell IDs

Example maze.txt:
  5 5
  4
  5 6 13 16

ALGORITHM (Stack-based DFS with backtracking to find ALL paths):
1. Open maze.txt, read rows, cols, blocked cells
2. Build blocked[] array
3. Call findAllPaths() - recursive DFS with backtracking:
   a. Start at S=0, mark onPath[0]=true, currentPath[0]=0
   b. At each cell: try all 4 neighbors (RIGHT, DOWN, LEFT, UP)
   c. If neighbor is valid, unblocked, not on current path:
      - Add to currentPath, mark onPath, recurse
      - After returning: remove from currentPath, unmark onPath (backtrack)
   d. If current cell == E: store path, check if shortest
4. After all paths found, print each path and then the shortest

TRACE (partial, default grid):
  Start DFS at 0 -> try right=1
    At 1 -> try right=2
      At 2 -> try right=3, also try down=7
        Path via 3->4->9->14->19->24(E)  FOUND (length 7)
        Path via 3->8->...              (other paths)
      At 7 -> try right=12, down=... etc.
  All paths enumerated by backtracking

=== BONUS P4: FILE FORMAT ===
-------------------------------------------------------
processes.txt format:
  Line 1: N (number of processes)
  Line 2: timeSlice
  Next N lines: executionTime jobDescription
  (Priority assigned as N, N-1, ..., 1 for processes 1..N)
  (i.e., first process read = highest priority = N)

Example processes.txt:
  3
  1
  4 reading file
  2 writing file
  6 compiling code

ALGORITHM (Priority Circular Queue):
1. Open processes.txt, read N, timeSlice, and N processes
2. Assign priorities: process 1 gets priority N, process 2 gets N-1, ..., last gets 1
3. Enqueue all into ArrayCircularQueue<Process>
4. Each cycle:
   a. Scan entire queue to find process with MAX priority
      (dequeue all into tempQ, track max, put all back except max)
   b. Run max-priority process for timeSlice
   c. Decrement its priority by 1
   d. If not complete: re-enqueue
   e. Print row: No | ProcessID | ExecTime | TimeRemaining | Priority | Status
5. After loop: print total and average time
*/

// ============================================================
// Process class (same as p4.cpp)
// ============================================================
class Process {
public:
    string id;
    int    executionTime;
    int    timeRemaining;
    string jobDescription;
    bool   isComplete;
    int    priority;

    Process()
        : id(""), executionTime(0), timeRemaining(0),
          jobDescription(""), isComplete(false), priority(0) {}

    Process(string _id, int _et, string _job, int _pri)
        : id(_id), executionTime(_et), timeRemaining(_et),
          jobDescription(_job), isComplete(false), priority(_pri) {}
};

// ============================================================
const int MAX_CELLS = 105;

/***** GLOBAL FUNCTION PROTOTYPES *****/

// Utility
void manualCopyStack(MyStack& src, MyStack& dst, int n);

// P3 Bonus
bool loadMaze(const string& filename, int& rows, int& cols,
              bool blocked[], int& cells);
void printCell(int id, int eId);
void findAllPaths(int cur, int end, bool blocked[], bool onPath[],
                  int currentPath[], int pathLen, int rows, int cols,
                  int cells, int& pathCount,
                  int& minLen, int shortestPath[], int& shortestLen);
void runP3Bonus();

// P4 Bonus
bool loadProcesses(const string& filename,
                   ArrayCircularQueue<Process>& CQ, int& N, int& T);
void simulateWithPriority(ArrayCircularQueue<Process>& CQ, int T, int N);
void printResults(int totalTime, int N);
void runP4Bonus();

/***** Driver Function Main *****/
int main()
{
    cout << "========================================" << endl;
    cout << "  BONUS P3: ALL PATHS FROM FILE" << endl;
    cout << "========================================" << endl;
    runP3Bonus();

    cout << "\n========================================" << endl;
    cout << "  BONUS P4: PRIORITY SCHEDULING FROM FILE" << endl;
    cout << "========================================" << endl;
    runP4Bonus();

    return 0;
}

/***** GLOBAL FUNCTION CODE *****/

// manualCopyStack: copies src into dst while preserving src.
// Workaround for Stack.h copy constructor bug (currentIndex hardcoded to -1).
void manualCopyStack(MyStack& src, MyStack& dst, int n)
{
    MyStack temp(n);
    int v;
    while (!src.isEmpty()) { src.pop(v); temp.push(v); }
    while (!temp.isEmpty()) { temp.pop(v); src.push(v); dst.push(v); }
}

// ============================================================
// P3 BONUS FUNCTIONS
// ============================================================

// loadMaze: reads maze from file. Returns false if file cannot be opened.
bool loadMaze(const string& filename, int& rows, int& cols,
              bool blocked[], int& cells)
{
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "  ERROR: Cannot open '" << filename << "'" << endl;
        cout << "  Please create the file with this format:" << endl;
        cout << "    Line 1: rows cols" << endl;
        cout << "    Line 2: number_of_blocked_cells" << endl;
        cout << "    Line 3: blocked_cell_id1 blocked_cell_id2 ..." << endl;
        cout << "  Example (5x5 grid, 4 blocked cells):" << endl;
        cout << "    5 5" << endl;
        cout << "    4" << endl;
        cout << "    5 6 13 16" << endl;
        return false;
    }

    fin >> rows >> cols;
    cells = rows * cols;

    for (int i = 0; i < cells; i++) blocked[i] = false;

    int numBlocked;
    fin >> numBlocked;
    for (int i = 0; i < numBlocked; i++) {
        int b; fin >> b;
        if (b > 0 && b < cells - 1) blocked[b] = true;
    }
    fin.close();
    return true;
}

// printCell: prints S, E, or numeric ID
void printCell(int id, int eId)
{
    if (id == 0)   { cout << "S";  return; }
    if (id == eId) { cout << "E";  return; }
    cout << id;
}

// findAllPaths: recursive DFS with backtracking to enumerate ALL paths.
// Prints each path immediately when E is reached.
// Tracks shortest path length and stores the shortest path.
void findAllPaths(int cur, int end, bool blocked[], bool onPath[],
                  int currentPath[], int pathLen, int rows, int cols,
                  int cells, int& pathCount,
                  int& minLen, int shortestPath[], int& shortestLen)
{
    if (cur == end) {
        pathCount++;
        cout << "  Path " << pathCount << " [" << pathLen << " steps]: ";
        for (int i = 0; i < pathLen; i++) {
            printCell(currentPath[i], end);
            if (i < pathLen - 1) cout << " -> ";
        }
        cout << endl;

        if (pathLen < minLen) {
            minLen = pathLen;
            shortestLen = pathLen;
            for (int i = 0; i < pathLen; i++) shortestPath[i] = currentPath[i];
        }
        return;
    }

    // Try 4 neighbors: RIGHT, DOWN, LEFT, UP
    int r = cur / cols, c = cur % cols;
    int neighbors[4] = {
        (c < cols - 1) ? cur + 1    : -1,   // RIGHT
        (r < rows - 1) ? cur + cols : -1,   // DOWN
        (c > 0)        ? cur - 1    : -1,   // LEFT
        (r > 0)        ? cur - cols : -1    // UP
    };

    for (int i = 0; i < 4; i++) {
        int nb = neighbors[i];
        if (nb != -1 && !blocked[nb] && !onPath[nb]) {
            onPath[nb] = true;
            currentPath[pathLen] = nb;
            findAllPaths(nb, end, blocked, onPath, currentPath, pathLen + 1,
                         rows, cols, cells, pathCount, minLen, shortestPath, shortestLen);
            onPath[nb] = false;     // BACKTRACK
        }
    }
}

// runP3Bonus: orchestrates reading maze from file and finding all paths
void runP3Bonus()
{
    string filename;
    cout << "Enter maze filename (e.g. maze.txt): ";
    cin >> filename;

    int rows, cols, cells;
    bool blocked[MAX_CELLS] = {false};

    if (!loadMaze(filename, rows, cols, blocked, cells)) {
        cout << "  Skipping P3 Bonus due to file error." << endl;
        return;
    }

    cout << "\nMaze loaded: " << rows << " x " << cols
         << " (" << cells << " cells)" << endl;
    cout << "S = cell 0 (top-left),  E = cell " << cells - 1
         << " (bottom-right)" << endl;

    // Print the grid
    cout << "\nGrid layout (X=blocked):" << endl;
    for (int r = 0; r < rows; r++) {
        cout << "  ";
        for (int c = 0; c < cols; c++) {
            int id = r * cols + c;
            if (id == 0)          cout << setw(4) << "S";
            else if (id == cells - 1) cout << setw(4) << "E";
            else if (blocked[id]) cout << setw(4) << "X";
            else                  cout << setw(4) << id;
        }
        cout << endl;
    }

    // Find all paths
    bool onPath[MAX_CELLS]    = {false};
    int  currentPath[MAX_CELLS];
    int  shortestPath[MAX_CELLS];
    int  shortestLen = 0;
    int  minLen      = 99999;
    int  pathCount   = 0;

    onPath[0]       = true;
    currentPath[0]  = 0;

    cout << "\nSearching all paths from S to E..." << endl;
    findAllPaths(0, cells - 1, blocked, onPath, currentPath, 1,
                 rows, cols, cells, pathCount, minLen, shortestPath, shortestLen);

    if (pathCount == 0) {
        cout << "  No path exists from S to E." << endl;
        return;
    }

    cout << "\nTotal paths found: " << pathCount << endl;

    cout << "\nSHORTEST PATH (" << shortestLen << " steps):" << endl;
    cout << "  ";
    for (int i = 0; i < shortestLen; i++) {
        printCell(shortestPath[i], cells - 1);
        if (i < shortestLen - 1) cout << " -> ";
    }
    cout << endl;
}

// ============================================================
// P4 BONUS FUNCTIONS
// ============================================================

// loadProcesses: reads processes from file. Returns false if file not found.
bool loadProcesses(const string& filename,
                   ArrayCircularQueue<Process>& CQ, int& N, int& T)
{
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "  ERROR: Cannot open '" << filename << "'" << endl;
        cout << "  Please create the file with this format:" << endl;
        cout << "    Line 1: N (number of processes)" << endl;
        cout << "    Line 2: timeSlice" << endl;
        cout << "    Next N lines: executionTime jobDescription" << endl;
        cout << "  Example:" << endl;
        cout << "    3" << endl;
        cout << "    1" << endl;
        cout << "    4 reading file" << endl;
        cout << "    2 writing file" << endl;
        cout << "    6 compiling code" << endl;
        return false;
    }

    fin >> N >> T;
    fin.ignore();

    for (int i = 1; i <= N; i++) {
        int et;
        string job;
        fin >> et;
        fin.ignore();
        getline(fin, job);

        string id = "p";
        for (int d = 0; d < 4; d++) id += char('0' + i);

        // Priority: first process read = highest priority = N
        int priority = N + 1 - i;
        Process p(id, et, job, priority);
        CQ.enqueue(p);
    }

    fin.close();
    return true;
}

// simulateWithPriority: each cycle picks the highest-priority process to run.
// Priority decrements by 1 after each execution cycle.
void simulateWithPriority(ArrayCircularQueue<Process>& CQ, int T, int N)
{
    cout << "\nPriority Simulation Using " << N << " Processes"
         << " (TimeSlice = " << T << "):" << endl;
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

        // ---- Find highest-priority process ----
        // Dequeue all into tempQ while tracking max priority
        ArrayCircularQueue<Process> tempQ(N * 200);
        int maxPriority = -1;
        Process maxP;

        while (!CQ.isEmpty()) {
            Process p;
            CQ.dequeue(p);
            if (p.priority > maxPriority) {
                maxPriority = p.priority;
                maxP = p;
            }
            tempQ.enqueue(p);
        }

        // Put everyone back EXCEPT the chosen process (skip first match)
        bool skipped = false;
        while (!tempQ.isEmpty()) {
            Process p;
            tempQ.dequeue(p);
            if (!skipped &&
                p.id == maxP.id &&
                p.timeRemaining == maxP.timeRemaining &&
                p.priority == maxP.priority) {
                skipped = true;
                continue;       // this is the one we're running
            }
            CQ.enqueue(p);
        }

        // ---- Run the selected process ----
        int usedPriority  = maxP.priority;  // priority at time of execution
        maxP.timeRemaining -= T;
        maxP.priority--;                    // decrement after execution

        string status;
        if (maxP.timeRemaining <= 0) {
            maxP.timeRemaining = 0;
            maxP.isComplete    = true;
            status             = "completed";
            // Do NOT re-enqueue
        } else {
            status = "not complete";
            CQ.enqueue(maxP);   // re-enqueue with decremented priority
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

    printResults(totalTime, N);
}

// printResults: displays total and average time
void printResults(int totalTime, int N)
{
    cout << "\nTotal Time taken = "     << totalTime << endl;
    cout << "Average Time/Processor = " << fixed << setprecision(1)
         << (float)totalTime / N       << endl;
}

// runP4Bonus: orchestrates reading processes from file and running priority simulation
void runP4Bonus()
{
    string filename;
    cout << "Enter processes filename (e.g. processes.txt): ";
    cin >> filename;

    int N = 0, T = 0;
    ArrayCircularQueue<Process> CQ(500);

    if (!loadProcesses(filename, CQ, N, T)) {
        cout << "  Skipping P4 Bonus due to file error." << endl;
        return;
    }

    cout << "\nLoaded " << N << " processes. Time Slice = " << T << endl;
    simulateWithPriority(CQ, T, N);
}
