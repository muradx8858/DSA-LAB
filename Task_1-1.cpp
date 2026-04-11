#include <iostream>
using namespace std;

#include "Stack.h"
#include "MyStack.h"
#include "Queue.h"
#include "MyQueue.h"

/*
STEP 1:
Is task mein hume Queue ke functions banane hain jaise read, display, append, reverseAppend, stats, isEqual.
Queue FIFO hoti hai (First In First Out).

STEP 2:
Hum queue se value nikaal ke temporary queue mein daalte hain aur phir wapas original mein daal dete hain
taake order same rahe.

STEP 3:
Reuse:
display, append, getSize same logic use karte hain.

STEP 4:
Learning:
Queue ko destroy kiye bina traverse karna.

STEP 5:
Skills:
Queue based problems solve karna.
*/

int getSize(Queue *q);

void read(Queue *q, istream &in)
{
    int x;
    while (!q->isFull())
    {
        in >> x;
        q->enqueue(x);
    }
}

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
    for (int i = 0; i < no && !src->isEmpty(); i++)
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
        if (!dst->isFull())
            dst->enqueue(v);
    }
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

void stats(Queue *q, int &mx, float &avg)
{
    int size = getSize(q);
    int sum = 0;
    mx = -999999;

    for (int i = 0; i < size; i++)
    {
        int v = q->dequeue();
        if (v > mx) mx = v;
        sum += v;
        q->enqueue(v);
    }

    avg = (float)sum / size;
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
