class LinkedList
{
protected:
    Node*first;
    Node*last;
public:
  LinkedList(){first = nullptr; last = nullptr;};
  virtual ~LinkedList();
    virtual void insertAtLast(int) = 0;
    virtual void insertAtFirst(int) = 0;
    virtual int removeFromFirst() = 0;
    virtual int removeFromLast() = 0;
    virtual bool isEmpty() = 0;
    virtual void display() = 0;
};

LinkedList:: ~LinkedList()
{
  if(first != nullptr)
    {
      delete first;
      first = nullptr;
    }
    if(last != nullptr)
    {
      delete last;
      last = nullptr;
    }
}
