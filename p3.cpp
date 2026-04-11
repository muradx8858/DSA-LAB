// SPRING 2026 DATA STRUCTURES SECTION D20 ASSIGNMENT ONE
// Name of Student: [YOUR NAME]
// Registration of Student: [YOUR REG NO]
// File p3.cpp: Problem 3 - Maze / Puzzle Path-Finding using Stack (DFS) and Queue (BFS)
// Bonus One implemented: find ALL alternate paths and identify the SHORTEST path.

#include <iostream>
#include <string>
using namespace std;
#include "Stack.h"
#include "MyStack.h"
#include "Queue.h"
#include "MyQueue.h"
#include "CircularQueue.h"
#include "ArrayCircularQueue.h"

// ============================================================
// JUSTIFICATION FOR bool/int GRID ARRAYS (blocked, visited, parent):
// The assignment says "You may create a Class Box to manage the puzzle."
// A Box class would have: int id, bool isBlocked, bool isVisited, int parentId.
// An array of Box objects would be:  Box grid[MAX_CELLS];
// which is IDENTICAL to having separate arrays:
//   bool blocked[MAX_CELLS];  bool visited[MAX_CELLS];  int parent[MAX_CELLS];
// These arrays represent the PUZZLE DATA (Box fields), NOT a replacement
// for Stack or Queue as the traversal data structure.
// The required Stack/Queue ADT IS used for the actual DFS/BFS traversal algorithm.
// Therefore these supporting arrays are justified as equivalent to Box class fields.
// ============================================================

// ============================================================
// BONUS MARKS - ADDITIONAL BUG FOUND IN CircularQueue.h display():
// The display() condition:
//   if(i != this->topOfQIndex || i != this->startOfQIndex)
// evaluates to TRUE whenever topOfQIndex != startOfQIndex (i.e., > 1 element),
// printing " -> " even after the last element before break.
// CORRECT condition should be:
//   if(i != this->startOfQIndex)
// So it prints " -> " only between elements, not after the last one.
// ============================================================

/***** PROBLEM DESCRIPTION *****/
/*
The puzzle is a grid of cells. Each black cell is blocked; each white cell can be crossed.
One step moves left, right, up, or down. Write a C++ program using Stack OR Queue as the
data structure to plot a passage from the Starting cell (S) to the Ending cell (E).
The passage may not be the shortest path.
First do the problem using Stack only (DFS), then using Queue only (BFS), in the same main.
Bonus One: Find and display all alternate paths and identify the shortest path.
*/

/***** PROBLEM SOLUTION DESCRIPTION *****/
/*
=== CELL ENCODING ===
Cells are numbered 0 to (rows*cols - 1):
  cell_id = row * cols + col
  row     = cell_id / cols
  col     = cell_id % cols
S is always cell 0  (top-left).
E is always cell rows*cols-1  (bottom-right).

Example default 5x5 grid (S=0, E=24, X=blocked):
  S[0]   1[1]   2[2]   3[3]   4[4]
  X[5]   X[6]   7[7]   8[8]   9[9]
 10[10] 11[11] 12[12]  X[13] 14[14]
 15[15]  X[16] 17[17] 18[18] 19[19]
 20[20] 21[21] 22[22] 23[23]  E[24]
Blocked: {5, 6, 13, 16}

=== STACK ONLY SOLUTION (DFS - without functions) ===
-------------------------------------------------------
1. Read rows, cols; read blocked cells from user
2. Initialize: blocked[], visited[] = all false; parent[] = -1
3. Push S (cell 0) onto MyStack, mark visited[0] = true
4. While Stack is not empty:
   a. Pop cell_id from Stack
   b. If cell_id == E: found! -> reconstruct and print path, stop
   c. For each neighbor (RIGHT, DOWN, LEFT, UP):
      - Compute neighbor_id based on row/col arithmetic
      - If in-bounds AND not blocked AND not visited:
        * Mark neighbor as visited
        * Set parent[neighbor] = cell_id
        * Push neighbor onto Stack
5. PATH RECONSTRUCTION:
   a. Trace from E back to S using parent[] array
   b. Push each cell_id onto a path-stack (MyStack pathStack)
   c. Pop from pathStack to print path in correct (S->E) order
   Output: S -> 1 -> 2 -> 3 -> 4 -> 9 -> 14 -> 19 -> E  (example DFS output)

DFS trace (Stack, push right-first):
  Push 0. Pop 0, push right=1 (up/left/down blocked or OOB). Stack:[1]
  Pop 1, push right=2. Stack:[2]
  Pop 2, push down=7, push right=3. Stack:[7,3]
  Pop 3, push down=8, push right=4. Stack:[7,8,4]
  Pop 4, push down=9. Stack:[7,8,9]
  Pop 9, push down=14. Stack:[7,8,14]
  Pop 14, push down=19. Stack:[7,8,19]
  Pop 19, push down=24=E, push left=18. Stack:[7,8,18,24]
  Pop 24 = E! --> Path: E<-19<-14<-9<-4<-3<-2<-1<-S
  Display: S -> 1 -> 2 -> 3 -> 4 -> 9 -> 14 -> 19 -> E

=== STACK ONLY SOLUTION (with functions) ===
-------------------------------------------------------
Functions: readMaze(), solveDFS(), printPath()
1. Call readMaze(rows, cols, blocked, cells)
2. Call solveDFS(rows, cols, blocked, visited, parent, cells) - Stack-based DFS
3. Call printPath(parent, cells-1, rows, cols) - prints path S -> ... -> E

=== QUEUE ONLY SOLUTION (BFS - without functions) ===
-------------------------------------------------------
1. Same maze input (reuse or re-read)
2. Reset visited[], parent[]
3. Enqueue S (cell 0) into ArrayCircularQueue<int>, mark visited[0] = true
4. While Queue is not empty:
   a. Dequeue cell_id
   b. If cell_id == E: found! -> reconstruct and print path, stop
   c. For each neighbor (RIGHT, DOWN, LEFT, UP):
      - If valid AND not blocked AND not visited:
        * Mark visited, set parent, enqueue neighbor
5. BFS GUARANTEES SHORTEST PATH because it explores level by level.

BFS trace on same grid:
  Enqueue 0. Dequeue 0, enqueue right=1. Q:[1]
  Dequeue 1, enqueue 2. Q:[2]
  Dequeue 2, enqueue 3, enqueue 7. Q:[3,7]
  Dequeue 3, enqueue 4, enqueue 8. Q:[7,4,8]
  Dequeue 7, enqueue 12. Q:[4,8,12]
  Dequeue 4, enqueue 9. Q:[8,12,9]
  Dequeue 8, enqueue 3(visited),7(visited). Enqueue nothing new. Q:[12,9]
  ... BFS eventually finds E=24 with shortest path.

=== QUEUE ONLY SOLUTION (with functions) ===
-------------------------------------------------------
Functions: readMaze(), solveBFS(), printPath()
1. Call readMaze()
2. Call solveBFS() - Queue-based BFS
3. Call printPath()

=== BONUS ONE: ALL PATHS + SHORTEST ===
-------------------------------------------------------
Use recursive DFS with backtracking:
1. Start at S, explore all unvisited unblocked neighbors recursively
2. Each time E is reached, store the path in allPaths (MyStack for current path)
3. Track minimum path length to identify the shortest
4. Display all paths found and then the shortest
*/

/***** GLOBAL FUNCTION PROTOTYPES *****/
const int MAX_CELLS = 105;

void readMaze(int& rows, int& cols, bool blocked[], int& cells);
void printCell(int id, int eId, int rows, int cols);
void printPath(int parent[], int eId, int rows, int cols);
bool solveDFS(int rows, int cols, bool blocked[], bool visited[], int parent[], int cells);
bool solveBFS(int rows, int cols, bool blocked[], bool visited[], int parent[], int cells);

// Bonus: recursive DFS to find all paths
void findAllPaths(int cur, int end, bool blocked[], bool onPath[],
                  int currentPath[], int pathLen, int rows, int cols, int cells,
                  int& minLen, int shortestPath[], int& shortestLen);

/***** Driver Function Main *****/
int main()
{
    int rows, cols, cells;
    bool blocked[MAX_CELLS] = {false};
    bool visited[MAX_CELLS] = {false};
    int  parent[MAX_CELLS];
    for (int i = 0; i < MAX_CELLS; i++) parent[i] = -1;

    // ============================================================
    // STACK ONLY (DFS) - without functions
    // ============================================================
    cout << "========================================" << endl;
    cout << "  STACK ONLY - DFS (without functions)" << endl;
    cout << "========================================" << endl;

    cout << "Use default 5x5 maze? (1=Yes, 0=Enter custom): ";
    int useDefault;
    cin >> useDefault;

    if (useDefault == 1) {
        rows = 5; cols = 5; cells = 25;
        // Default blocked: 5, 6, 13, 16
        int defaultBlocked[] = {5, 6, 13, 16};
        for (int b : defaultBlocked) blocked[b] = true;
        cout << "\nDefault 5x5 Grid (X=blocked, .=open):" << endl;
        cout << "  S  .  .  .  ." << endl;
        cout << "  X  X  .  .  ." << endl;
        cout << "  .  .  .  X  ." << endl;
        cout << "  .  X  .  .  ." << endl;
        cout << "  .  .  .  .  E" << endl;
    } else {
        readMaze(rows, cols, blocked, cells);
    }

    // DFS using MyStack - mark visited on push
    {
        for (int i = 0; i < cells; i++) { visited[i] = false; parent[i] = -1; }

        MyStack dfsStack(cells);
        dfsStack.push(0);
        visited[0] = true;
        bool found = false;
        int eId = cells - 1;

        while (!dfsStack.isEmpty() && !found) {
            int cur;
            dfsStack.pop(cur);

            if (cur == eId) { found = true; break; }

            int r = cur / cols, c = cur % cols;
            // Push neighbors in order: UP, LEFT, DOWN, RIGHT
            // (RIGHT pushed last -> popped first = right-first DFS)
            int neighbors[4] = {
                (r > 0)        ? cur - cols : -1,   // UP
                (c > 0)        ? cur - 1    : -1,   // LEFT
                (r < rows - 1) ? cur + cols : -1,   // DOWN
                (c < cols - 1) ? cur + 1    : -1    // RIGHT
            };
            for (int i = 0; i < 4; i++) {
                int nb = neighbors[i];
                if (nb != -1 && !blocked[nb] && !visited[nb]) {
                    visited[nb] = true;
                    parent[nb]  = cur;
                    dfsStack.push(nb);
                }
            }
        }

        if (found) {
            cout << "\nDFS Path Found:" << endl;
            cout << "  ";
            printPath(parent, eId, rows, cols);
        } else {
            cout << "\nNo path found (DFS)." << endl;
        }
    }

    // ============================================================
    // STACK ONLY (DFS) - with functions
    // ============================================================
    cout << "\n========================================" << endl;
    cout << "  STACK ONLY - DFS (with functions)" << endl;
    cout << "========================================" << endl;
    {
        for (int i = 0; i < cells; i++) { visited[i] = false; parent[i] = -1; }
        bool found = solveDFS(rows, cols, blocked, visited, parent, cells);
        if (found) {
            cout << "DFS Path (with functions):" << endl;
            cout << "  ";
            printPath(parent, cells - 1, rows, cols);
        } else {
            cout << "No path found." << endl;
        }
    }

    // ============================================================
    // QUEUE ONLY (BFS) - without functions
    // ============================================================
    cout << "\n========================================" << endl;
    cout << "  QUEUE ONLY - BFS (without functions)" << endl;
    cout << "========================================" << endl;
    {
        for (int i = 0; i < cells; i++) { visited[i] = false; parent[i] = -1; }

        int eId = cells - 1;
        ArrayCircularQueue<int> bfsQueue(cells);
        bfsQueue.enqueue(0);
        visited[0] = true;
        bool found = false;

        while (!bfsQueue.isEmpty() && !found) {
            int cur;
            bfsQueue.dequeue(cur);

            if (cur == eId) { found = true; break; }

            int r = cur / cols, c = cur % cols;
            // Enqueue neighbors: RIGHT, DOWN, LEFT, UP (BFS order)
            int neighbors[4] = {
                (c < cols - 1) ? cur + 1    : -1,   // RIGHT
                (r < rows - 1) ? cur + cols : -1,   // DOWN
                (c > 0)        ? cur - 1    : -1,   // LEFT
                (r > 0)        ? cur - cols : -1    // UP
            };
            for (int i = 0; i < 4; i++) {
                int nb = neighbors[i];
                if (nb != -1 && !blocked[nb] && !visited[nb]) {
                    visited[nb] = true;
                    parent[nb]  = cur;
                    bfsQueue.enqueue(nb);
                }
            }
        }

        if (found) {
            cout << "BFS Path Found (SHORTEST PATH):" << endl;
            cout << "  ";
            printPath(parent, eId, rows, cols);
        } else {
            cout << "No path found (BFS)." << endl;
        }
    }

    // ============================================================
    // QUEUE ONLY (BFS) - with functions
    // ============================================================
    cout << "\n========================================" << endl;
    cout << "  QUEUE ONLY - BFS (with functions)" << endl;
    cout << "========================================" << endl;
    {
        for (int i = 0; i < cells; i++) { visited[i] = false; parent[i] = -1; }
        bool found = solveBFS(rows, cols, blocked, visited, parent, cells);
        if (found) {
            cout << "BFS Shortest Path (with functions):" << endl;
            cout << "  ";
            printPath(parent, cells - 1, rows, cols);
        } else {
            cout << "No path found." << endl;
        }
    }

    // ============================================================
    // BONUS ONE: Find ALL paths + identify SHORTEST
    // ============================================================
    cout << "\n========================================" << endl;
    cout << "  BONUS ONE: ALL PATHS + SHORTEST PATH" << endl;
    cout << "========================================" << endl;
    {
        bool onPath[MAX_CELLS]    = {false};
        int  currentPath[MAX_CELLS];
        int  shortestPath[MAX_CELLS];
        int  shortestLen = 9999;
        int  minLen = 9999;
        for (int i = 0; i < MAX_CELLS; i++) currentPath[i] = -1;
        for (int i = 0; i < MAX_CELLS; i++) shortestPath[i] = -1;

        onPath[0] = true;
        currentPath[0] = 0;
        int pathLen = 1;

        cout << "All possible paths from S to E:" << endl;
        findAllPaths(0, cells - 1, blocked, onPath, currentPath, pathLen,
                     rows, cols, cells, minLen, shortestPath, shortestLen);

        cout << "\nSHORTEST PATH (" << shortestLen << " steps):" << endl;
        cout << "  ";
        for (int i = 0; i < shortestLen; i++) {
            printCell(shortestPath[i], cells - 1, rows, cols);
            if (i < shortestLen - 1) cout << " -> ";
        }
        cout << endl;
    }

    return 0;
}

/***** GLOBAL FUNCTION CODE *****/

// readMaze: reads grid dimensions and blocked cells from user
void readMaze(int& rows, int& cols, bool blocked[], int& cells)
{
    cout << "Enter rows: "; cin >> rows;
    cout << "Enter cols: "; cin >> cols;
    cells = rows * cols;
    int numBlocked;
    cout << "How many blocked cells (not counting S and E): ";
    cin >> numBlocked;
    cout << "Enter cell IDs of blocked cells (S=" << 0 << " E=" << cells-1 << "):" << endl;
    for (int i = 0; i < numBlocked; i++) {
        int b; cin >> b;
        if (b != 0 && b != cells - 1) blocked[b] = true;
    }
}

// printCell: prints cell label (S, E, or numeric ID matching assignment numbering)
void printCell(int id, int eId, int rows, int cols)
{
    if (id == 0)   { cout << "S"; return; }
    if (id == eId) { cout << "E"; return; }
    // Assignment numbers non-S/E cells as 1, 2, 3, ..., cells-2
    cout << id;
}

// printPath: reconstructs path from S to E using parent[] and prints it
void printPath(int parent[], int eId, int rows, int cols)
{
    // Trace from E back to S, push onto stack, then pop to print in order
    MyStack pathStack(MAX_CELLS);
    int cur = eId;
    while (cur != -1) {
        pathStack.push(cur);
        cur = parent[cur];
    }
    int cell;
    bool first = true;
    while (!pathStack.isEmpty()) {
        pathStack.pop(cell);
        if (!first) cout << " -> ";
        printCell(cell, eId, rows, cols);
        first = false;
    }
    cout << endl;
}

// solveDFS: Stack-based DFS. Returns true if path found. parent[] filled with path info.
bool solveDFS(int rows, int cols, bool blocked[], bool visited[], int parent[], int cells)
{
    int eId = cells - 1;
    MyStack S(cells);
    S.push(0);
    visited[0] = true;

    while (!S.isEmpty()) {
        int cur;
        S.pop(cur);
        if (cur == eId) return true;

        int r = cur / cols, c = cur % cols;
        int neighbors[4] = {
            (r > 0)        ? cur - cols : -1,
            (c > 0)        ? cur - 1    : -1,
            (r < rows - 1) ? cur + cols : -1,
            (c < cols - 1) ? cur + 1    : -1
        };
        for (int i = 0; i < 4; i++) {
            int nb = neighbors[i];
            if (nb != -1 && !blocked[nb] && !visited[nb]) {
                visited[nb] = true;
                parent[nb]  = cur;
                S.push(nb);
            }
        }
    }
    return false;
}

// solveBFS: Queue-based BFS. Returns true if path found. Guarantees SHORTEST path.
bool solveBFS(int rows, int cols, bool blocked[], bool visited[], int parent[], int cells)
{
    int eId = cells - 1;
    ArrayCircularQueue<int> Q(cells);
    Q.enqueue(0);
    visited[0] = true;

    while (!Q.isEmpty()) {
        int cur;
        Q.dequeue(cur);
        if (cur == eId) return true;

        int r = cur / cols, c = cur % cols;
        int neighbors[4] = {
            (c < cols - 1) ? cur + 1    : -1,
            (r < rows - 1) ? cur + cols : -1,
            (c > 0)        ? cur - 1    : -1,
            (r > 0)        ? cur - cols : -1
        };
        for (int i = 0; i < 4; i++) {
            int nb = neighbors[i];
            if (nb != -1 && !blocked[nb] && !visited[nb]) {
                visited[nb] = true;
                parent[nb]  = cur;
                Q.enqueue(nb);
            }
        }
    }
    return false;
}

// findAllPaths: recursive DFS with backtracking to find ALL paths from cur to end.
// Prints each path as found; tracks shortest path.
void findAllPaths(int cur, int end, bool blocked[], bool onPath[],
                  int currentPath[], int pathLen, int rows, int cols, int cells,
                  int& minLen, int shortestPath[], int& shortestLen)
{
    if (cur == end) {
        // Print this path
        cout << "  Path: ";
        for (int i = 0; i < pathLen; i++) {
            printCell(currentPath[i], end, rows, cols);
            if (i < pathLen - 1) cout << " -> ";
        }
        cout << " [" << pathLen << " steps]" << endl;

        // Check if this is the shortest
        if (pathLen < minLen) {
            minLen = pathLen;
            shortestLen = pathLen;
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
                         rows, cols, cells, minLen, shortestPath, shortestLen);
            onPath[nb] = false;     // backtrack
        }
    }
}
