// SPRING 2026 DATA STRUCTURES SECTION D20 ASSIGNMENT ONE
// Name of Student: Aman Ali
// Registration of Student: L1F24BSCS0667
// File p3.cpp contains complete code of problem 3
#include <iostream>
#include <string>
using namespace std;

/* Question */
/* The puzzle is a grid of cells. Each black cell is blocked; each white cell can be crossed.
   One step can move left, right, up, or down. Write a program using Stack (DFS) and
   Queue (BFS) to plot a path from the Starting cell (S) to the Ending cell (E).
   Bonus: Find and display all alternate paths and the shortest one. */

/* PROBLEM SOLUTION */
/*
   I will represent the maze as a 1D array of cells (cell_id = row * cols + col).
   S is always cell 0 (top-left) and E is always the last cell (bottom-right).
   For Stack (DFS): push neighbors onto stack, pop and explore. LIFO gives one valid path.
   For Queue (BFS): enqueue neighbors, dequeue and explore. FIFO guarantees shortest path.
   Path reconstruction uses a parent[] array - trace back from E to S.
   For Bonus: recursive DFS with backtracking finds ALL possible paths.

   Steps:
   1. Read rows, cols and blocked cells.
   2. DFS using Stack: push 0, mark visited, pop and push unvisited neighbors.
   3. Reconstruct and print path using parent[].
   4. BFS using Queue: same approach but enqueue instead of push.
   5. BFS gives shortest path guaranteed.
   6. Bonus: recursive DFS with backtracking to find all paths.
*/

const int MAX_CELLS = 105;

// ---- int Stack (for DFS) ----
class Stack {
public:
    Stack(int _maxCapacity = 0) {
        currentIndex = -1;
        maxCapacity = _maxCapacity;
        values = new int[maxCapacity]{0};
    }
    virtual ~Stack() { delete[] values; }
    virtual void push(int v) = 0;
    virtual bool pop(int& value) = 0;
    bool isEmpty() { return (currentIndex < 0); }
    bool isFull()  { return (currentIndex == maxCapacity - 1); }
protected:
    int currentIndex;
    int* values;
    int maxCapacity;
};

class MyStack : public Stack {
public:
    MyStack(int _cap = 0) : Stack(_cap) {}
    void push(int v) {
        if (!isFull()) values[++currentIndex] = v;
    }
    bool pop(int& v) {
        if (!isEmpty()) { v = values[currentIndex--]; return true; }
        return false;
    }
};

// ---- Simple int Circular Queue (for BFS) ----
class IntQueue {
public:
    IntQueue(int cap) {
        maxCapacity = cap;
        values = new int[maxCapacity]{0};
        front = rear = -1;
        count = 0;
    }
    ~IntQueue() { delete[] values; }
    bool isEmpty() { return (count == 0); }
    bool isFull()  { return (count == maxCapacity); }
    void enqueue(int v) {
        if (isFull()) return;
        if (isEmpty()) { front = rear = 0; }
        else { rear = (rear + 1) % maxCapacity; }
        values[rear] = v;
        count++;
    }
    int dequeue() {
        if (isEmpty()) return -1;
        int v = values[front];
        count--;
        if (isEmpty()) { front = rear = -1; }
        else { front = (front + 1) % maxCapacity; }
        return v;
    }
private:
    int* values;
    int front, rear, maxCapacity, count;
};

// ---- Utility: print a cell as S, E, or its id ----
void printCell(int id, int eId) {
    if (id == 0)   { cout << "S"; return; }
    if (id == eId) { cout << "E"; return; }
    cout << id;
}

// ---- Reconstruct and print path S -> ... -> E using parent[] ----
void printPath(int parent[], int eId) {
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
        printCell(cell, eId);
        first = false;
    }
    cout << endl;
}

// ---- Recursive DFS to find ALL paths (Bonus) ----
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

int main() {
    int rows, cols, cells;
    bool blocked[MAX_CELLS] = {false};
    bool visited[MAX_CELLS] = {false};
    int  parent[MAX_CELLS];
    for (int i = 0; i < MAX_CELLS; i++) parent[i] = -1;

    cout << "Use default 5x5 maze? (1=Yes, 0=Custom): ";
    int useDefault;
    cin >> useDefault;

    if (useDefault == 1) {
        rows = 5; cols = 5; cells = 25;
        int def[] = {5, 6, 13, 16};
        for (int b : def) blocked[b] = true;
        cout << "\nDefault 5x5 Grid (X=blocked):" << endl;
        cout << "  S  .  .  .  ." << endl;
        cout << "  X  X  .  .  ." << endl;
        cout << "  .  .  .  X  ." << endl;
        cout << "  .  X  .  .  ." << endl;
        cout << "  .  .  .  .  E" << endl;
    }
    else {
        cout << "Enter rows: "; cin >> rows;
        cout << "Enter cols: "; cin >> cols;
        cells = rows * cols;
        int numBlocked;
        cout << "How many blocked cells: "; cin >> numBlocked;
        cout << "Enter cell IDs (S=0, E=" << cells - 1 << "): ";
        for (int i = 0; i < numBlocked; i++) {
            int b; cin >> b;
            if (b > 0 && b < cells - 1) blocked[b] = true;
        }
    }

    int eId = cells - 1;

    // =====================================================
    // STACK ONLY - DFS
    // =====================================================
    cout << "\n--- Stack (DFS) ---" << endl;
    {
        for (int i = 0; i < cells; i++) { visited[i] = false; parent[i] = -1; }

        MyStack dfsStack(cells);
        dfsStack.push(0);
        visited[0] = true;
        bool found = false;

        while (!dfsStack.isEmpty() && !found) {
            int cur;
            dfsStack.pop(cur);
            if (cur == eId) { found = true; break; }

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
                    dfsStack.push(nb);
                }
            }
        }

        if (found) {
            cout << "DFS Path: ";
            printPath(parent, eId);
        }
        else {
            cout << "No path found." << endl;
        }
    }

    // =====================================================
    // QUEUE ONLY - BFS (guarantees shortest path)
    // =====================================================
    cout << "\n--- Queue (BFS) ---" << endl;
    {
        for (int i = 0; i < cells; i++) { visited[i] = false; parent[i] = -1; }

        IntQueue bfsQueue(cells);
        bfsQueue.enqueue(0);
        visited[0] = true;
        bool found = false;

        while (!bfsQueue.isEmpty() && !found) {
            int cur = bfsQueue.dequeue();
            if (cur == eId) { found = true; break; }

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
                    bfsQueue.enqueue(nb);
                }
            }
        }

        if (found) {
            cout << "BFS Shortest Path: ";
            printPath(parent, eId);
        }
        else {
            cout << "No path found." << endl;
        }
    }

    // =====================================================
    // BONUS: All Paths + Shortest Path
    // =====================================================
    cout << "\n--- Bonus: All Paths ---" << endl;
    {
        bool onPath[MAX_CELLS]    = {false};
        int  currentPath[MAX_CELLS];
        int  shortestPath[MAX_CELLS];
        int  shortestLen = 0, minLen = 99999, pathCount = 0;
        for (int i = 0; i < MAX_CELLS; i++) currentPath[i] = shortestPath[i] = -1;

        onPath[0] = true;
        currentPath[0] = 0;

        findAllPaths(0, eId, blocked, onPath, currentPath, 1,
                     rows, cols, pathCount, minLen, shortestPath, shortestLen);

        if (pathCount == 0) {
            cout << "No path exists." << endl;
        }
        else {
            cout << "\nTotal paths found: " << pathCount << endl;
            cout << "Shortest Path (" << shortestLen << " steps): ";
            for (int i = 0; i < shortestLen; i++) {
                printCell(shortestPath[i], eId);
                if (i < shortestLen - 1) cout << " -> ";
            }
            cout << endl;
        }
    }

    return 0;
}
