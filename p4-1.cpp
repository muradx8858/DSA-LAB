// SPRING 2026 DATA STRUCTURES SECTION D20 ASSIGNMENT ONE
// Name of Student: Aman Ali
// Registration of Student: L1F24BSCS0667
// File p4.cpp contains complete code of problem 4
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

/* Question */
/* In an OS, N processes execute on the CPU in time-share manner. Each process is given
   a fixed Time Slice on the CPU, then put back in the circular queue. Once done it leaves
   the queue completely. Simulate this. Display total and average time taken.
   Bonus: Assign priorities 1 to N; execute highest priority first each cycle;
   decrement priority after each execution; display priority in output. */

/* PROBLEM SOLUTION */
/*
   I will use a circular queue to store Process objects.
   Each process has an id, executionTime, timeRemaining, jobDescription, isComplete, priority.
   In the simulation loop:
     - Dequeue the front process.
     - Subtract timeSlice from timeRemaining.
     - If timeRemaining <= 0: process is complete, don't re-enqueue.
     - Otherwise: re-enqueue at the back (Round Robin!).
   For Bonus: scan queue each cycle to find highest priority process and run it first.
   Priority decrements by 1 after each execution.

   Steps:
   1. Read N processes and time slice from user.
   2. Enqueue all into circular queue with IDs p1111, p2222, ...
   3. Simulate Round Robin and print table.
   4. Print total and average time.
   5. Bonus: assign priorities N..1, pick highest priority each cycle.
*/

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

    Process(string _id, int _et, string _job, int _pri = 0)
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
    int size() { return count; }
private:
    Process* values;
    int front, rear, maxCapacity, count;
};

void readProcesses(CircularQueue& CQ, int& N, int& T) {
    cout << "How many Processes: ";
    cin >> N;
    cout << "Please enter data of " << N << " Process:" << endl;
    for (int i = 1; i <= N; i++) {
        int et;
        string job;
        cout << "Process " << i << ": Execution Time: ";
        cin >> et;
        cin.ignore();
        cout << "Process " << i << ": Job Description: ";
        getline(cin, job);
        string id = "p";
        for (int d = 0; d < 4; d++) id += char('0' + i);
        Process p(id, et, job);
        CQ.enqueue(p);
    }
    cout << "Enter Time Slice of microprocessor: ";
    cin >> T;
}

void simulate(CircularQueue& CQ, int T, int N) {
    cout << "\nSimulation Using " << N << " Processes:" << endl;
    cout << left
         << setw(5)  << "No."
         << setw(10) << "ProcessID"
         << setw(16) << "ExecutionTime"
         << setw(16) << "TimeRemaining"
         << "Status" << endl;
    cout << string(60, '-') << endl;

    int row = 1, totalTime = 0;

    while (!CQ.isEmpty()) {
        Process p = CQ.dequeue();
        p.timeRemaining -= T;
        string status;

        if (p.timeRemaining <= 0) {
            p.timeRemaining = 0;
            p.isComplete    = true;
            status          = "completed";
        }
        else {
            status = "not complete";
            CQ.enqueue(p);
        }

        cout << left
             << setw(5)  << row
             << setw(10) << p.id
             << setw(16) << p.executionTime
             << setw(16) << p.timeRemaining
             << status   << endl;
        row++;
        totalTime++;
    }

    cout << "\nTotal Time taken = " << totalTime << endl;
    cout << "Average Time/Processor = " << fixed << setprecision(1)
         << (float)totalTime / N << endl;
}

void simulateWithPriority(CircularQueue& CQ, int T, int N) {
    cout << "\nPriority Simulation Using " << N << " Processes:" << endl;
    cout << left
         << setw(5)  << "No."
         << setw(10) << "ProcessID"
         << setw(16) << "ExecutionTime"
         << setw(16) << "TimeRemaining"
         << setw(12) << "Priority"
         << "Status" << endl;
    cout << string(70, '-') << endl;

    int row = 1, totalTime = 0;

    while (!CQ.isEmpty()) {
        // Find highest priority process
        CircularQueue tempQ(N * 100);
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

        // Restore all back except the chosen one (skip first match)
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

        // Run the selected process
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
    // =====================================================
    // QUEUE ONLY - without functions
    // =====================================================
    cout << "=== Round Robin (without functions) ===" << endl;
    int N;
    cout << "How many Processes: ";
    cin >> N;

    CircularQueue CQ(N);
    cout << "Please enter data of " << N << " Process:" << endl;
    for (int i = 1; i <= N; i++) {
        int et;
        string job;
        cout << "Process " << i << ": Execution Time: ";
        cin >> et;
        cin.ignore();
        cout << "Process " << i << ": Job Description: ";
        getline(cin, job);
        string id = "p";
        for (int d = 0; d < 4; d++) id += char('0' + i);
        Process p(id, et, job);
        CQ.enqueue(p);
    }

    int T;
    cout << "Enter Time Slice of microprocessor: ";
    cin >> T;

    cout << "\nSimulation Using " << N << " Processes:" << endl;
    cout << left
         << setw(5)  << "No."
         << setw(10) << "ProcessID"
         << setw(16) << "ExecutionTime"
         << setw(16) << "TimeRemaining"
         << "Status" << endl;
    cout << string(60, '-') << endl;

    int row = 1, totalTime = 0;
    while (!CQ.isEmpty()) {
        Process p = CQ.dequeue();
        p.timeRemaining -= T;
        string status;
        if (p.timeRemaining <= 0) {
            p.timeRemaining = 0;
            p.isComplete    = true;
            status          = "completed";
        }
        else {
            status = "not complete";
            CQ.enqueue(p);
        }
        cout << left
             << setw(5)  << row
             << setw(10) << p.id
             << setw(16) << p.executionTime
             << setw(16) << p.timeRemaining
             << status   << endl;
        row++;
        totalTime++;
    }
    cout << "\nTotal Time taken = " << totalTime << endl;
    cout << "Average Time/Processor = " << fixed << setprecision(1)
         << (float)totalTime / N << endl;

    // =====================================================
    // QUEUE ONLY - with functions
    // =====================================================
    cout << "\n=== Round Robin (with functions) ===" << endl;
    int N2, T2;
    CircularQueue CQ2(100);
    readProcesses(CQ2, N2, T2);
    simulate(CQ2, T2, N2);

    // =====================================================
    // BONUS: Priority Scheduling
    // =====================================================
    cout << "\n=== Bonus: Priority Scheduling ===" << endl;
    cout << "(Priority 1=lowest, N=highest; decrements each cycle)" << endl;

    int N3, T3;
    CircularQueue CQ3(100);
    cout << "How many Processes: ";
    cin >> N3;
    cout << "Please enter data of " << N3 << " Process:" << endl;
    for (int i = 1; i <= N3; i++) {
        int et;
        string job;
        cout << "Process " << i << ": Execution Time: ";
        cin >> et;
        cin.ignore();
        cout << "Process " << i << ": Job Description: ";
        getline(cin, job);
        string id = "p";
        for (int d = 0; d < 4; d++) id += char('0' + i);
        Process p(id, et, job, N3 + 1 - i);
        CQ3.enqueue(p);
    }
    cout << "Enter Time Slice: ";
    cin >> T3;
    simulateWithPriority(CQ3, T3, N3);

    return 0;
}
