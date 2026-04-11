#include <iostream>
using namespace std;

#include "Stack.h"
#include "MyStack.h"
#include "Queue.h"
#include "MyQueue.h"

/*
STEP 1:
Insertion aur shifting queue mein karni hai.

STEP 2:
Queue ko maintain rakhne ke liye dequeue + enqueue use karte hain.

STEP 3:
Functions reuse:
append, display, getSize

STEP 4:
Learning:
Queue mein direct insert possible nahi hota, rotation use hoti hai.

STEP 5:
Skills:
Queue modify karna.
*/

int getSize(Queue *q);

void display(Queue *q, ostream &out)
{
    int size = getSize(q);
    for (int i = 0; i < size; i++)
    {
        int v = q->dequeue();
        out << v << " ";
        q->enqueue(v);
    }
    out << endl;
}

int getSize(Queue *q)
{
    int size = 0;
    int count = 0;

    while (!q->isEmpty())
    {
        int v = q->dequeue();
        q->enqueue(v);
        size++;
        count++;
        if (count == size) break;
    }
    return size;
}

void append(Queue *src, Queue *dst)
{
    int size = getSize(src);
    for (int i = 0; i < size; i++)
    {
        int v = src->dequeue();
        if (!dst->isFull())
            dst->enqueue(v);
        src->enqueue(v);
    }
}

void append(Queue *src, Queue *dst, int no)
{
    for (int i = 0; i < no; i++)
    {
        int v = src->dequeue();
        if (!dst->isFull())
            dst->enqueue(v);
        src->enqueue(v);
    }
}

void reverseAppend(Queue *src, Queue *dst)
{
    MyStack st(100);

    int size = getSize(src);
    for (int i = 0; i < size; i++)
    {
        int v = src->dequeue();
        st.push(v);
        src->enqueue(v);
    }

    while (!st.isEmpty())
    {
        int v;
        st.pop(v);
        dst->enqueue(v);
    }
}

void insert(Queue *q, int v, int pos)
{
    int size = getSize(q);

    for (int i = 0; i < size; i++)
    {
        int x = q->dequeue();

        if (i == pos)
            q->enqueue(v);

        q->enqueue(x);
    }

    if (pos >= size)
        q->enqueue(v);
}

void insert(Queue *src, Queue *dst, int pos)
{
    int size = getSize(src);

    for (int i = 0; i < size; i++)
    {
        int v = src->dequeue();

        if (i == pos)
            dst->enqueue(v);

        dst->enqueue(v);
        src->enqueue(v);
    }
}

void shiftRight(Queue *q, int pos)
{
    int size = getSize(q);

    for (int i = 0; i < size - pos; i++)
    {
        int v = q->dequeue();
        q->enqueue(v);
    }
}

void shiftLeft(Queue *q, int pos)
{
    int size = getSize(q);

    for (int i = 0; i < pos; i++)
    {
        int v = q->dequeue();
        q->enqueue(v);
    }
}

bool isEqual(Queue *a, Queue *b)
{
    int sizeA = getSize(a);
    int sizeB = getSize(b);

    if (sizeA != sizeB) return false;

    for (int i = 0; i < sizeA; i++)
    {
        int v1 = a->dequeue();
        int v2 = b->dequeue();

        if (v1 != v2)
        {
            a->enqueue(v1);
            b->enqueue(v2);
            return false;
        }

        a->enqueue(v1);
        b->enqueue(v2);
    }
    return true;
}
