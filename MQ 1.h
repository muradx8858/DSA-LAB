class MyQueue: public Queue
{
public:
  MyQueue(int s);
  MyQueue(const MyQueue & mq);
  ~MyQueue();
  void enqueue(const int & value);
  int dequeue();
  void display(ostream &) const;
protected:
  int * values;
};
