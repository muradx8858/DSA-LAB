#include<iostream>
using namespace std;
#include"linkedlist.h"
#include"Mylinkedlist.h"
// Add to myLinkedList.h public section:
void put(int v, int pos) {
    Node* nn = new Node(v);
    if (pos == 0) {
        nn->next = head;
        head = nn;
        return;
    }
    Node* t = head;
    for (int i = 0; i < pos - 1 && t->next; i++) t = t->next;
    nn->next = t ? t->next : NULL;
    if (t) t->next = nn;
}

void swap(int xi, int yi) {
    if (xi == yi || xi < 0 || yi < 0) return;
    int sz = getSize();
    if (xi >= sz || yi >= sz) return;

    int vx = peek(xi), vy = peek(yi);
    get(xi); get(yi);
    put(vy, xi);
    put(vx, yi);
}

void sort(bool asc) {
    int n = getSize();
    for (int i = 0; i < n - 1; i++) {
        int minpos = i;
        for (int j = i + 1; j < n; j++) {
            int a = peek(minpos), b = peek(j);
            if (asc ? (b < a) : (b > a)) minpos = j;
        }
        if (minpos != i) swap(i, minpos);
    }
}

// Task_3.cpp test
int main() {
    myLinkedList l;
    l.insert(30); l.insert(10); l.insert(20);  // 30?10?20

    l.put(15, 1);        // 30?15?10?20
    l.swap(1, 2);        // 30?10?15?20
    l.sort(true);        // 10?15?20?30

    l.print();
    return 0;
}