// SPRING 2026 DATA STRUCTURES SECTION D20 ASSIGNMENT ONE
// Name of Student: [YOUR NAME]
// Registration of Student: [YOUR REG NO]
// File p1.cpp: Problem 1 - Average, Maximum, and Minimum using Stack and Queue
// Must compile and run with the provided Stack.h, MyStack.h, Queue.h, MyQueue.h

#include <iostream>
using namespace std;
#include "Stack.h"
#include "MyStack.h"
#include "Queue.h"
#include "MyQueue.h"

// ============================================================
// BONUS MARKS - BUG FOUND AND CORRECTED IN Stack.h
// ============================================================
// BUG: Stack copy constructor sets currentIndex = -1 (hardcoded)
// This makes every copied Stack appear EMPTY even after copying all values.
// Result: any pop/top on the copy fails immediately.
//
// ORIGINAL (buggy):
//   Stack::Stack(const Stack & s) {
//       currentIndex = -1;          <-- BUG: always -1
//       ...
//   }
//
// CORRECTED (fix one line in Stack.h):
//   Stack::Stack(const Stack & s) {
//       currentIndex = s.currentIndex;  <-- FIX
//       ...
//   }
//
// WORKAROUND USED IN THIS FILE:
// Since we cannot guarantee the fix is applied, we use manualCopyStack()
// which pops and re-pushes through a temp stack to make a correct copy.
// This workaround produces the same result as a correct copy constructor.
// ============================================================

/***** PROBLEM DESCRIPTION *****/
/*
First N is read from the user and then N integers. Calculate and display Average,
Maximum and Minimum Values on screen. First do the problem using Stack only and
then using Queue Only in the same main.
*/

/***** PROBLEM SOLUTION DESCRIPTION *****/
/*
=== STACK ONLY SOLUTION (without functions) ===
-------------------------------------------------------
1. Read N from user
2. Create MyStack S of size N
3. Read N integers from user, push each onto S
   Example (N=5): push 23, 29, 23, 12, 100
   Stack (top->bottom):  100 | 12 | 23 | 29 | 23
4. Create copy CS using manualCopyStack(S, CS, N) - S is preserved
   CS (top->bottom):     100 | 12 | 23 | 29 | 23
5. Pop first value from CS into maxVal and minVal; sum = that value; count = 1
   val=100, maxVal=100, minVal=100, sum=100, count=1
6. Loop while CS is not empty:
   a. Pop value from CS into variable val
   b. sum += val;  count++
   c. if val > maxVal -> update maxVal
   d. if val < minVal -> update minVal
   Trace: val=12 -> sum=112, min=12 | val=23 -> | val=29 -> | val=23 -> max=100, min=12
7. Display: average = (float)sum/count, Max = maxVal, Min = minVal

=== STACK ONLY SOLUTION (with functions) ===
-------------------------------------------------------
Functions Used:  read(MyStack&, int n),  stats(MyStack&)
1. Read N from user; Create MyStack S of size N
2. Call read(S, N) - fills S by reading N values from user
   Stack after read: 100 | 12 | 23 | 29 | 23
3. Create empty copy CS(N); call manualCopyStack(S, CS, N) - S preserved
4. Call stats(CS) - CS is consumed, avg/max/min are computed and displayed

=== QUEUE ONLY SOLUTION (without functions) ===
-------------------------------------------------------
1. Read N from user
2. Create MyQueue Q of size N
3. Read N integers, enqueue each into Q
   Queue after input:  front -> 23 -> 29 -> 23 -> 12 -> 100 <- rear
4. Copy-construct CQ = Q  (Queue copy constructor is correct for freshly filled queue)
   CQ: front -> 23 -> 29 -> 23 -> 12 -> 100 <- rear
5. Dequeue first value into maxVal and minVal; sum = that value; count = 1
   val=23, maxVal=23, minVal=23, sum=23, count=1
6. Loop while CQ is not empty:
   a. val = CQ.dequeue()
   b. sum += val;  count++
   c. update maxVal and minVal
7. Display results

=== QUEUE ONLY SOLUTION (with functions) ===
-------------------------------------------------------
Functions Used:  read(MyQueue&, int n),  stats(MyQueue&)
1. Read N from user; Create MyQueue Q of size N
2. Call read(Q, N)
   Queue: front -> 23 -> 29 -> 23 -> 12 -> 100 <- rear
3. Copy-construct CQ = Q
4. Call stats(CQ) - CQ is consumed, results displayed
*/

/***** GLOBAL FUNCTION PROTOTYPES *****/
void manualCopyStack(MyStack& src, MyStack& dst, int n);
void read(MyStack& S, int n);
void stats(MyStack& CS);        // consumes CS - always pass a copy, not original
void read(MyQueue& Q, int n);
void stats(MyQueue& CQ);        // consumes CQ - always pass a copy, not original

/***** Driver Function Main *****/
int main()
{
    int n;
    cout << "Enter N (number of values): ";
    cin >> n;

    // ============================================================
    // STACK ONLY - without functions
    // ============================================================
    cout << "\n=== STACK ONLY (without functions) ===" << endl;
    MyStack S(n);
    int val;
    for (int i = 0; i < n; i++) {
        cout << "  Enter value " << (i + 1) << ": ";
        cin >> val;
        S.push(val);
    }

    {
        MyStack CS(n);
        manualCopyStack(S, CS, n);          // workaround for buggy copy constructor

        int sum = 0, count = 0, maxVal, minVal;
        CS.pop(maxVal);
        minVal = maxVal;
        sum    = maxVal;
        count  = 1;

        while (!CS.isEmpty()) {
            CS.pop(val);
            sum += val;
            count++;
            if (val > maxVal) maxVal = val;
            if (val < minVal) minVal = val;
        }
        cout << "  Average = " << (float)sum / count
             << "   Max = " << maxVal
             << "   Min = " << minVal << endl;
    }

    // ============================================================
    // STACK ONLY - with functions
    // ============================================================
    cout << "\n=== STACK ONLY (with functions) ===" << endl;
    {
        MyStack CS(n);
        manualCopyStack(S, CS, n);          // S is preserved; CS will be consumed by stats()
        stats(CS);
    }

    // ============================================================
    // QUEUE ONLY - without functions
    // ============================================================
    cout << "\n=== QUEUE ONLY (without functions) ===" << endl;
    MyQueue Q(n);
    cout << "  (Re-enter same " << n << " values for Queue demonstration)" << endl;
    for (int i = 0; i < n; i++) {
        cout << "  Enter value " << (i + 1) << ": ";
        cin >> val;
        Q.enqueue(val);
    }

    {
        MyQueue CQ = Q;                     // Queue copy constructor is correct here

        int sum = 0, count = 0, maxVal, minVal;
        maxVal = CQ.dequeue();
        minVal = maxVal;
        sum    = maxVal;
        count  = 1;

        while (!CQ.isEmpty()) {
            val = CQ.dequeue();
            sum += val;
            count++;
            if (val > maxVal) maxVal = val;
            if (val < minVal) minVal = val;
        }
        cout << "  Average = " << (float)sum / count
             << "   Max = " << maxVal
             << "   Min = " << minVal << endl;
    }

    // ============================================================
    // QUEUE ONLY - with functions
    // ============================================================
    cout << "\n=== QUEUE ONLY (with functions) ===" << endl;
    {
        MyQueue CQ = Q;                     // Q is preserved; CQ is consumed by stats()
        stats(CQ);
    }

    return 0;
}

/***** GLOBAL FUNCTION CODE *****/

// manualCopyStack: copies src into dst while restoring src.
// Needed because Stack copy constructor has a bug (currentIndex reset to -1).
// src and dst must both have capacity n.
void manualCopyStack(MyStack& src, MyStack& dst, int n)
{
    MyStack temp(n);
    int v;
    // Step 1: pop src into temp (reverses order into temp)
    while (!src.isEmpty()) {
        src.pop(v);
        temp.push(v);
    }
    // Step 2: pop temp back into src AND dst simultaneously (restores original order)
    while (!temp.isEmpty()) {
        temp.pop(v);
        src.push(v);
        dst.push(v);
    }
}

// read: reads n integers from user and pushes onto S
void read(MyStack& S, int n)
{
    int val;
    for (int i = 0; i < n; i++) {
        cout << "  Enter value " << (i + 1) << ": ";
        cin >> val;
        S.push(val);
    }
}

// stats: computes and displays average, max, min by consuming CS (pass a copy!)
void stats(MyStack& CS)
{
    if (CS.isEmpty()) {
        cout << "  Stack is empty - nothing to compute." << endl;
        return;
    }
    int val, sum = 0, count = 0, maxVal, minVal;
    CS.pop(maxVal);
    minVal = maxVal;
    sum    = maxVal;
    count  = 1;

    while (!CS.isEmpty()) {
        CS.pop(val);
        sum += val;
        count++;
        if (val > maxVal) maxVal = val;
        if (val < minVal) minVal = val;
    }
    cout << "  Average = " << (float)sum / count
         << "   Max = " << maxVal
         << "   Min = " << minVal << endl;
}

// read: reads n integers from user and enqueues into Q
void read(MyQueue& Q, int n)
{
    int val;
    for (int i = 0; i < n; i++) {
        cout << "  Enter value " << (i + 1) << ": ";
        cin >> val;
        Q.enqueue(val);
    }
}

// stats: computes and displays average, max, min by consuming CQ (pass a copy!)
void stats(MyQueue& CQ)
{
    if (CQ.isEmpty()) {
        cout << "  Queue is empty - nothing to compute." << endl;
        return;
    }
    int val, sum = 0, count = 0, maxVal, minVal;
    maxVal = CQ.dequeue();
    minVal = maxVal;
    sum    = maxVal;
    count  = 1;

    while (!CQ.isEmpty()) {
        val = CQ.dequeue();
        sum += val;
        count++;
        if (val > maxVal) maxVal = val;
        if (val < minVal) minVal = val;
    }
    cout << "  Average = " << (float)sum / count
         << "   Max = " << maxVal
         << "   Min = " << minVal << endl;
}
