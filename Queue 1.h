class Queue
{
 public:
  Queue(int cap) : startIndex(-1), endIndex(-1), maxCapacity(cap), count(0) {}
 Queue(const Queue & q) : startIndex(q.startIndex), endIndex(q.endIndex), maxCapacity(q.maxCapacity), count(q.count){}
  virtual ~Queue() {}

  bool isEmpty() const;
  bool isFull() const;
  virtual void enqueue(const  int & value) = 0;
  virtual int dequeue() = 0;
  virtual void display(ostream &) const = 0;
protected:
    int startIndex;     // corresponds to front
    int endIndex;       // corresponds to rear
    int maxCapacity;       // total size of array
    int count; // keep a count of values in queue
};

  // Check if queue is empty
bool Queue::isEmpty() const
{
      return (count == 0);
}
// Check if queue is full
bool Queue::isFull() const
{
  return (count == maxCapacity );
}
