#include<iostream>
using namespace std;
#include"linkedlist.h"
#include"Mylinkedlist.h"
// Add to myLinkedList.h public section:
int getSize() {
    int cnt = 0;
    Node* t = head;
    while (t) { cnt++; t = t->next; }
    return cnt;
}

bool search(int v) {
    Node* t = head;
    while (t) {
        if (t->data == v) return true;
        t = t->next;
    }
    return false;
}

int peek(int pos) {
    Node* t = head;
    for (int i = 0; i < pos && t; i++) t = t->next;
    return t ? t->data : -1;
}

int get(int pos) {
    if (pos < 0 || !head) return -1;
    if (pos == 0) {
        int val = head->data;
        Node* temp = head;
        head = head->next;
        delete temp;
        return val;
    }
    Node* prev = head;
    for (int i = 0; i < pos - 1 && prev->next; i++) prev = prev->next;
    if (prev->next) {
        int val = prev->next->data;
        Node* temp = prev->next;
        prev->next = prev->next->next;
        delete temp;
        return val;
    }
    return -1;
}

// Task_2.cpp test
int main() {
    myLinkedList l;
    l.insert(10); l.insert(20); l.insert(30);
    cout << l.getSize() << endl;     // 3
    cout << l.search(20) << endl;    // 1
    cout << l.peek(1) << endl;       // 20
    cout << l.get(0) << endl;        // 10 (list now 20→30)
    return 0;
}