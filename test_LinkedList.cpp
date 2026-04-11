#include <iostream>
using namespace std;

#include "Node.h"
#include "LinkedList.h"
#include "myLinkedList.h"

int main()
{
    myLinkedList obj;

    obj.insertAtLast(10);        
    obj.insertAtLast(0);
    obj.insertAtLast(-3);
    obj.insertAtLast(99);
    obj.insertAtLast(1024);
    obj.insertAtFirst(458);
    cout << endl << "Display: " << endl;
    obj.display();
    cout << "DeleteFrom First: " << obj.removeFromFirst() << endl; 
    cout << "Delete From Last: " << obj.removeFromLast() << endl;
    cout << endl << "Display: " << endl;
    obj.display();
    cout << endl << endl;

    return 0;
}
