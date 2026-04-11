class myLinkedList:public LinkedList
{
public:
  void insertAtLast(int value);
  void insertAtFirst(int value);
  void display();
  int removeFromFirst();
  int removeFromLast();
  bool isEmpty();

};

bool myLinkedList::isEmpty()
{
  return first==nullptr && last==nullptr;
}

int myLinkedList::removeFromLast()
{
  if (first==nullptr && last == nullptr) //empty linked list
    {
      cout << "LL is empty; returing NULL value" << endl;
      return -99999;
    }
  else if (first == last) //single node case
    {
      int returingValue = first->data;
      delete last;
      first = nullptr;
      last = nullptr;
      return returingValue;
    }
  else
    {
      Node*t = first;
      while(1)
        {
	  if (t->next == last)
	    {
	      break;
	    }
	  t = t->next;
        }

      int returingValue = last->data;
      delete last;
      t->next = nullptr;
      last = t;
      return returingValue;
    }
}

int myLinkedList::removeFromFirst()
{
  if (first==nullptr && last == nullptr) //empty linked list
    {
      cout << "LL is empty; returing NULL value" << endl;
      return -99999;
    }
  else if (first == last) //single node case
    {
      int returingValue = first->data;
      delete last;
      first = nullptr;
      last = nullptr;
      return returingValue;
    }
  else
    {
      int returingValue = first->data;
      Node*t = first;
      first = first->next;
      delete t;
      t = nullptr;
      return returingValue;
    }
}

void myLinkedList::display()
{
  if (first == nullptr && last == nullptr)
    {
      cout << "->\n";
      return;
    }
  Node* t = first;
  while (1)
    {
      cout << t->data;
      cout << " -> ";
      t = t->next;
      if (t == nullptr)
	{
	  //cout << "nullptr";
	  break;
	}
    }
  cout << endl;
}

void myLinkedList::insertAtFirst(int value)
{
  Node*nn = new Node;
  nn->data = value;
  nn->next = nullptr;
  if (first==nullptr && last == nullptr) //first node case
    {
      first = nn;
      last = nn;
    }
  else // for all Nodes other than the first
    {
      nn->next = first;
      first = nn;
    }
}

void myLinkedList::insertAtLast(int value)
{
  Node*nn = new Node;
  nn->data = value;
  nn->next = nullptr;
  if (first==nullptr && last == nullptr) //first node case
    {
      first = nn;
      last = nn;
    }
  else // for all Nodes other than the first
    {
      last->next = nn;
      last = nn;
    }
}
