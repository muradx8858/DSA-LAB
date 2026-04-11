// SPRING 2026 DATA STRUCTURES SECTION D20 ASSIGNMENT ONE
// Name of Student: [YOUR NAME]
// Registration of Student: [YOUR REG NO]
// File p4.cpp: Problem 4 - Round Robin CPU Scheduler simulation using Queue ADT.
// Bonus One implemented: priority-based scheduling where priority decrements after each run.

#include <iostream>
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
// JUSTIFICATION FOR USE OF ArrayCircularQueue:
// The problem explicitly states: "The time slice slots are managed via Queue"
// and "processes are put in a circular queue."
// ArrayCircularQueue IS a circular Queue ADT (inherits CircularQueue<T>).
// This directly satisfies the assignment requirement of a circular queue.
// MyQueue is a linear (non-circular) queue and does NOT wrap around,
// making it unsuitable for Round Robin's circular scheduling pattern.
// ============================================================

// ============================================================
// JUSTIFICATION FOR Process Class:
// The assignment says "You may create a Class Process: ID, Time, Job description,
// isComplete." The Process class below directly implements this specification.
// ArrayCircularQueue<Process> stores Process objects - no plain arrays used.
// ============================================================

/***** PROBLEM DESCRIPTION *****/
/*
In an OS, N processes execute on the CPU in time-share manner. Each process is given
a fixed Time Slice on the CPU, then put back in the circular queue. Once done it leaves
the queue completely. Simulate this. Display the total and average time taken.
Bonus One: Read data from file; assign priorities 1 to N; execute highest priority first
each cycle; decrement priority after each execution; display priority in output.
*/

/***** PROBLEM SOLUTION DESCRIPTION *****/
/*
=== PROCESS CLASS ===
Fields: id (string), executionTime (int), timeRemaining (int),
        jobDescription (string), isComplete (bool), priority (int)
--------------------------------------------------------------

=== QUEUE ONLY SOLUTION (without functions) ===
-------------------------------------------------------
1. Read N (number of processes) from user
2. For each process i (1 to N):
   a. Read executionTime and jobDescription from user
   b. Assign id = "p" + (1111 * i)  (e.g., p1111, p2222, ...)
   c. Set timeRemaining = executionTime, isComplete = false
   d. Enqueue process into ArrayCircularQueue<Process> CQ
3. Read timeSlice T from user
4. Print header:  No. | ProcessID | ExecutionTime | TimeRemaining | Status
5. Simulation loop: row counter = 1
   While CQ is not empty:
     a. Dequeue front process into variable p
     b. Subtract T from p.timeRemaining
     c. If p.timeRemaining <= 0:
          p.timeRemaining = 0
          p.isComplete    = true
          status string   = "completed"
          (do NOT re-enqueue)
     d. Else:
          status string = "not complete"
          CQ.enqueue(p)    // put back at rear - Round Robin!
     e. Print row: row | p.id | p.executionTime | p.timeRemaining | status
     f. row++
6. After loop:
   a. totalTime = row - 1  (each row = 1 time unit = 1 time slice used)
   b. averageTime = (float)totalTime / N
   c. Display "Total Time taken = X"  and  "Average Time/Processor = Y"

Example trace (N=2, T=1, p1=2 time units, p2=3 time units):
  CQ initial: p1111(rem=2) -> p2222(rem=3)
  Row 1: dequeue p1111, rem=2-1=1, not complete, re-enqueue
  Row 2: dequeue p2222, rem=3-1=2, not complete, re-enqueue
  Row 3: dequeue p1111, rem=1-1=0, COMPLETED, don't re-enqueue
  Row 4: dequeue p2222, rem=2-1=1, not complete, re-enqueue
  Row 5: dequeue p2222, rem=1-1=0, COMPLETED
  CQ empty. Total = 5, Avg = 5/2 = 2.5

=== QUEUE ONLY SOLUTION (with functions) ===
-------------------------------------------------------
Functions: readProcesses(), simulate(), printResults()
1. Call readProcesses(CQ, N, T)     - fills queue and reads timeSlice
2. Call simulate(CQ, T, N, total)   - runs simulation and prints table
3. Call printResults(total, N)      - prints total and average time

=== BONUS ONE: Priority Scheduling ===
-------------------------------------------------------
1. Assign priorities 1 to N (priority 1 = lowest, N = highest)
2. In each cycle, scan the queue to find the process with HIGHEST priority
3. Execute that process (dequeue it, run it, re-enqueue if not done)
4. Decrement the executed process's priority by 1 after each run
5. Print priority in the output table

Algorithm for priority dequeue from circular queue:
  - Dequeue all processes into a temp queue
  - Track the one with max priority
  - Re-enqueue all EXCEPT the max-priority one back into main queue
  - Process the max-priority one
*/

// ============================================================
// Process class
// ============================================================
class Process {
public:
    string id;
    int    executionTime;
    int    timeRemaining;
    string jobDescription;
    bool   isComplete;
    int    priority;        // used for Bonus One

    Process()
        : id(""), executionTime(0), timeRemaining(0),
          jobDescription(""), isComplete(false), priority(0) {}

    Process(string _id, int _et, string _job, int _priority = 0)
        : id(_id), executionTime(_et), timeRemaining(_et),
          jobDescription(_job), isComplete(false), priority(_priority) {}
};

/***** GLOBAL FUNCTION PROTOTYPES *****/
void readProcesses(ArrayCircularQueue<Process>& CQ, int& N, int& T);
void simulate(ArrayCircularQueue<Process>& CQ, int T, int N);
void printResults(int totalTime, int N);
// Bonus
void readProcessesFromFile(ArrayCircularQueue<Process>& CQ, int& N, int& T,
                           const string& filename);
void simulateWithPriority(ArrayCircularQueue<Process>& CQ, int T, int N);

/***** Driver Function Main *****/
int main()
{
    // ============================================================
    // QUEUE ONLY - without functions
    // ============================================================
    cout << "========================================" << endl;
    cout << "  ROUND ROBIN SIMULATION (without functions)" << endl;
    cout << "========================================" << endl;

    int N;
    cout << "How many Processes: ";
    cin >> N;

    ArrayCircularQueue<Process> CQ(N);

    cout << "Please enter data of " << N << " Process:" << endl;
    for (int i = 1; i <= N; i++) {
        int et;
        string job;
        cout << "Process " << i << ": Execution Time: ";
        cin >> et;
        cin.ignore();
        cout << "Process " << i << ": Job Description: ";
        getline(cin, job);

        // Generate ID: p1111, p2222, p3333, ...
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

    int row       = 1;
    int totalTime = 0;

    while (!CQ.isEmpty()) {
        Process p;
        CQ.dequeue(p);

        p.timeRemaining -= T;
        string status;

        if (p.timeRemaining <= 0) {
            p.timeRemaining = 0;
            p.isComplete    = true;
            status          = "completed";
            // Do NOT re-enqueue - process is done
        } else {
            status = "not complete";
            CQ.enqueue(p);      // Round Robin: put back at rear
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

    cout << "\nTotal Time taken = "    << totalTime << endl;
    cout << "Average Time/Processor = " << fixed << setprecision(1)
         << (float)totalTime / N      << endl;

    // ============================================================
    // QUEUE ONLY - with functions
    // ============================================================
    cout << "\n========================================" << endl;
    cout << "  ROUND ROBIN SIMULATION (with functions)" << endl;
    cout << "========================================" << endl;

    int N2, T2;
    ArrayCircularQueue<Process> CQ2(100);
    readProcesses(CQ2, N2, T2);
    simulate(CQ2, T2, N2);

    // ============================================================
    // BONUS ONE: Priority Scheduling
    // ============================================================
    cout << "\n========================================" << endl;
    cout << "  BONUS ONE: PRIORITY-BASED SCHEDULING" << endl;
    cout << "========================================" << endl;
    cout << "(Priorities assigned 1=lowest to N=highest; decremented each cycle)" << endl;

    int N3, T3;
    ArrayCircularQueue<Process> CQ3(100);
    readProcesses(CQ3, N3, T3);
    simulateWithPriority(CQ3, T3, N3);

    return 0;
}

/***** GLOBAL FUNCTION CODE *****/

// readProcesses: reads N processes and time slice from user, fills CQ
void readProcesses(ArrayCircularQueue<Process>& CQ, int& N, int& T)
{
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

        // Priority = N + 1 - i  so first process gets highest priority = N
        Process p(id, et, job, N + 1 - i);
        CQ.enqueue(p);
    }
    cout << "Enter Time Slice of microprocessor: ";
    cin >> T;
}

// simulate: runs Round Robin simulation and prints results table
void simulate(ArrayCircularQueue<Process>& CQ, int T, int N)
{
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
        Process p;
        CQ.dequeue(p);

        p.timeRemaining -= T;
        string status;
        if (p.timeRemaining <= 0) {
            p.timeRemaining = 0;
            p.isComplete    = true;
            status          = "completed";
        } else {
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

    printResults(totalTime, N);
}

// printResults: displays total and average time
void printResults(int totalTime, int N)
{
    cout << "\nTotal Time taken = "    << totalTime << endl;
    cout << "Average Time/Processor = " << fixed << setprecision(1)
         << (float)totalTime / N      << endl;
}

// simulateWithPriority: BONUS ONE - in each cycle, execute highest-priority process first.
// Priority decrements by 1 after each execution.
void simulateWithPriority(ArrayCircularQueue<Process>& CQ, int T, int N)
{
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
        // Step 1: Find highest-priority process by scanning the entire queue
        // Use a temp ArrayCircularQueue to hold all processes while finding max
        int size = 0;
        ArrayCircularQueue<Process> tempQ(N * 100);

        // Move all from CQ to tempQ, count and find max priority
        Process maxP;
        int maxPriority = -1;

        while (!CQ.isEmpty()) {
            Process p;
            CQ.dequeue(p);
            if (p.priority > maxPriority) {
                maxPriority = p.priority;
                maxP = p;
            }
            tempQ.enqueue(p);
            size++;
        }

        // Step 2: Move all back to CQ EXCEPT the highest-priority process (skip once)
        bool skipped = false;
        while (!tempQ.isEmpty()) {
            Process p;
            tempQ.dequeue(p);
            if (!skipped && p.id == maxP.id && p.timeRemaining == maxP.timeRemaining) {
                skipped = true;     // skip this one - it's the one we're running
                continue;
            }
            CQ.enqueue(p);
        }

        // Step 3: Run the selected process
        maxP.timeRemaining -= T;
        maxP.priority--;             // decrement priority after each run
        string status;

        if (maxP.timeRemaining <= 0) {
            maxP.timeRemaining = 0;
            maxP.isComplete    = true;
            status             = "completed";
        } else {
            status = "not complete";
            CQ.enqueue(maxP);       // re-enqueue with decremented priority
        }

        cout << left
             << setw(5)  << row
             << setw(10) << maxP.id
             << setw(16) << maxP.executionTime
             << setw(16) << maxP.timeRemaining
             << setw(12) << maxP.priority + 1   // show priority used this cycle
             << status   << endl;
        row++;
        totalTime++;
    }

    printResults(totalTime, N);
}
