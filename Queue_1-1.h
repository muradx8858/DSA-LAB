#ifndef QUEUE_1_H
#define QUEUE_1_H

#include <iostream>
#include <stdexcept>
using namespace std;

// ─────────────────────────────────────────────
//  Abstract Base Class
// ─────────────────────────────────────────────
class Queue
{
public:
    Queue(int cap) : startIndex(-1), endIndex(-1), maxCapacity(cap), count(0) {}
    Queue(const Queue & q) : startIndex(q.startIndex), endIndex(q.endIndex),
                              maxCapacity(q.maxCapacity), count(q.count) {}
    virtual ~Queue() {}

    bool isEmpty() const;
    bool isFull()  const;

    virtual void enqueue(const int & value) = 0;
    virtual int  dequeue()                  = 0;
    virtual void display(ostream &)   const = 0;

protected:
    int startIndex;   // front of the queue
    int endIndex;     // rear  of the queue
    int maxCapacity;  // total capacity of the array
    int count;        // number of elements currently in queue
};

// Check if queue is empty
bool Queue::isEmpty() const
{
    return (count == 0);
}

// Check if queue is full
bool Queue::isFull() const
{
    return (count == maxCapacity);
}


// ─────────────────────────────────────────────
//  Concrete Derived Class – Circular Array Queue
// ─────────────────────────────────────────────
class CircularQueue : public Queue
{
public:
    // Constructor – allocate internal array
    CircularQueue(int cap)
        : Queue(cap), data(new int[cap]) {}

    // Copy constructor – deep copy
    CircularQueue(const CircularQueue & q)
        : Queue(q), data(new int[q.maxCapacity])
    {
        for (int i = 0; i < maxCapacity; ++i)
            data[i] = q.data[i];
    }

    // Destructor – free internal array
    virtual ~CircularQueue()
    {
        delete[] data;
    }

    // Assignment operator – deep copy
    CircularQueue & operator=(const CircularQueue & q)
    {
        if (this != &q)
        {
            delete[] data;
            startIndex   = q.startIndex;
            endIndex     = q.endIndex;
            maxCapacity  = q.maxCapacity;
            count        = q.count;
            data         = new int[maxCapacity];
            for (int i = 0; i < maxCapacity; ++i)
                data[i] = q.data[i];
        }
        return *this;
    }

    // ── enqueue ──────────────────────────────
    // Adds value to the rear of the queue.
    // Wraps around using modulo (circular behaviour).
    void enqueue(const int & value) override
    {
        if (isFull())
            throw overflow_error("enqueue() called on a full queue.");

        if (isEmpty())
        {
            startIndex = 0;
            endIndex   = 0;
        }
        else
        {
            endIndex = (endIndex + 1) % maxCapacity;
        }

        data[endIndex] = value;
        ++count;
    }

    // ── dequeue ──────────────────────────────
    // Removes and returns the front element.
    // Advances startIndex with wrap-around.
    int dequeue() override
    {
        if (isEmpty())
            throw underflow_error("dequeue() called on an empty queue.");

        int value = data[startIndex];

        if (count == 1)                          // queue becomes empty
        {
            startIndex = -1;
            endIndex   = -1;
        }
        else
        {
            startIndex = (startIndex + 1) % maxCapacity;
        }

        --count;
        return value;
    }

    // ── display ──────────────────────────────
    // Prints all elements front → rear to the given stream.
    void display(ostream & os) const override
    {
        if (isEmpty())
        {
            os << "Queue is empty." << endl;
            return;
        }

        os << "Queue (front -> rear): ";
        for (int i = 0; i < count; ++i)
        {
            os << data[(startIndex + i) % maxCapacity];
            if (i < count - 1) os << " -> ";
        }
        os << endl;
    }

    // Overloaded << operator for convenience
    friend ostream & operator<<(ostream & os, const CircularQueue & q)
    {
        q.display(os);
        return os;
    }

private:
    int * data;   // dynamically allocated circular buffer
};

#endif // QUEUE_1_H
