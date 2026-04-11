// SPRING 2026 DATA STRUCTURES SECTION D20 ASSIGNMENT ONE
// Name of Student: Aman Ali
// Registration of Student: L1F24BSCS0667
// File p1.cpp contains complete code of problem 1
#include <iostream>
using namespace std;

/* Question */
/* First N is read from the user and then N integers. Calculate and display Average,
   Maximum and Minimum Values on screen. First do the problem using Stack only and
   then using Queue Only in the same main. */

/* PROBLEM SOLUTION */
/*
   I will read N from the user and then read N integers.
   For the Stack-only part, I will push all values onto a MyStack and then
   pop them one by one to compute average, max, and min.
   For the Queue-only part, I will enqueue all values into a MyQueue and
   dequeue them to compute the same stats.

   Steps:
   1. Read N from user.
   2. Create MyStack of size N, read N values and push onto stack.
   3. Pop values to compute average, max, min and display.
   4. Create MyQueue of size N, read N values and enqueue.
   5. Dequeue values to compute average, max, min and display.
*/

class Stack {
public:
    Stack(int _maxCapacity = 0) {
        currentIndex = -1;
        maxCapacity = _maxCapacity;
        values = new int[maxCapacity]{0};
    }
    Stack(const Stack& s) {
        maxCapacity = s.maxCapacity;
        currentIndex = s.currentIndex;
        values = new int[maxCapacity]{0};
        for (int i = 0; i <= currentIndex; i++) {
            values[i] = s.values[i];
        }
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

class Queue {
public:
    Queue(int cap) : startOfQIndex(-1), topOfQIndex(-1), maxCapacity(cap), count(0) {}
    Queue(const Queue& q) : startOfQIndex(q.startOfQIndex), topOfQIndex(q.topOfQIndex),
        maxCapacity(q.maxCapacity), count(q.count) {}
    virtual ~Queue() {}
    bool isEmpty() const { return (count == 0); }
    bool isFull()  const { return (count == maxCapacity); }
    virtual void enqueue(const int& value) = 0;
    virtual int  dequeue() = 0;
protected:
    int startOfQIndex;
    int topOfQIndex;
    int maxCapacity;
    int count;
};

class MyStack : public Stack {
public:
    MyStack(int _cap = 0) : Stack(_cap) {}
    MyStack(const MyStack& ma) : Stack(ma) {}
    void push(int v) {
        if (!isFull()) {
            values[++currentIndex] = v;
        }
    }
    bool pop(int& v) {
        if (!isEmpty()) {
            v = values[currentIndex--];
            return true;
        }
        return false;
    }
};

class MyQueue : public Queue {
public:
    MyQueue(int s) : Queue(s) {
        if (s != 0) values = new int[maxCapacity]{0};
        else        values = nullptr;
    }
    MyQueue(const MyQueue& mq) : Queue(mq) {
        if (maxCapacity != 0) {
            values = new int[maxCapacity]{0};
            for (int i = 0; i < maxCapacity; i++) {
                values[i] = mq.values[i];
            }
        } else {
            values = nullptr;
        }
    }
    ~MyQueue() {
        if (values != nullptr) {
            delete[] values;
            values = nullptr;
        }
    }
    void enqueue(const int& value) {
        if (isFull()) return;
        if (isEmpty()) topOfQIndex = 0;
        startOfQIndex++;
        values[startOfQIndex] = value;
        count++;
    }
    int dequeue() {
        if (isEmpty()) return -1;
        int val = values[topOfQIndex];
        count--;
        topOfQIndex++;
        if (isEmpty()) startOfQIndex = topOfQIndex = -1;
        return val;
    }
protected:
    int* values;
};

void instack(MyStack& s, int n) {
    int val;
    for (int i = 0; i < n; i++) {
        cout << "Enter value " << i + 1 << ": ";
        cin >> val;
        s.push(val);
    }
}

void outstack(MyStack s, int n) {
    if (s.isEmpty()) {
        return;
    }
    else {
        int current, maxVal, minVal;
        long sum = 0;
        s.pop(current);
        maxVal = minVal = sum = current;
        while (s.pop(current)) {
            sum += current;
            if (current > maxVal) maxVal = current;
            if (current < minVal) minVal = current;
        }
        cout << "Average: " << (float)sum / n << endl;
        cout << "Max: " << maxVal << endl;
        cout << "Min: " << minVal << endl;
    }
}

void inqueue(MyQueue& q, int n) {
    int val;
    for (int i = 0; i < n; i++) {
        cout << "Enter value " << i + 1 << ": ";
        cin >> val;
        q.enqueue(val);
    }
}

void outqueue(MyQueue q, int n) {
    if (q.isEmpty()) {
        return;
    }
    else {
        int current = q.dequeue();
        int maxVal, minVal;
        long sum = current;
        maxVal = minVal = current;
        while (!q.isEmpty()) {
            current = q.dequeue();
            sum += current;
            if (current > maxVal) maxVal = current;
            if (current < minVal) minVal = current;
        }
        cout << "Average: " << (float)sum / n << endl;
        cout << "Max: " << maxVal << endl;
        cout << "Min: " << minVal << endl;
    }
}

int main() {
    int n;
    cout << "Enter N: ";
    cin >> n;

    if (n > 0) {
        // Stack Logic
        MyStack s(n);
        instack(s, n);
        cout << endl << "--- Stack Results ---" << endl;
        outstack(s, n);

        // Queue Logic
        cout << endl;
        MyQueue q(n);
        inqueue(q, n);
        cout << endl << "--- Queue Results ---" << endl;
        outqueue(q, n);
    }
    else {
        cout << "N must be greater than 0." << endl;
    }

    return 0;
}
