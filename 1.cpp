#include<iostream>
using namespace std;
#include"linkedlist.h"
#include"Mylinkedlist.h"
 

// Task_1.cpp
/*
1. Task is fix copy constructor (deep copy) and destructor (delete all nodes)
2. Copy: newHead=null, while original has nodes: create newNode(value), link to newHead, newHead=newNode
   Destructor: while(head): temp=head, head=head->next, delete temp
3. Use only provided LinkedList.h functions
4. Learned deep vs shallow copy difference
5. Can now handle custom memory management
*/

#include "myLinkedList.h"
int main() {
    myLinkedList list1;
    list1.insert(10); list1.insert(20); list1.insert(30);

    myLinkedList list2 = list1;  // Tests copy constructor
    list2.insertAtHead(5);      // Change copy only

    list1.print();  // 10→20→30
    list2.print();  // 5→10→20→30
    return 0;
}