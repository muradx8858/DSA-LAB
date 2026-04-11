// SPRING 2026 DATA STRUCTURES SECTION D20 ASSIGNMENT ONE
// Name of Student: Aman Ali
// Registration of Student: L1F24BSCS0667
// File p2.cpp contains complete code of problem 2
#include <iostream>
#include <string>
using namespace std;

/* Question */
/* Read An English Sentence (ends with a full stop) from a User. First Display the count of chars
   and words. Then copy the Sentence in word-reverse and display. Then calculate frequencies of
   each word and display. Do this using Stack only and then using Queue only in the same main. */

/* PROBLEM SOLUTION */
/*
   I will read a string from the user until a full stop is encountered.
   I will first iterate through the sentence to count characters and words.
   For the Stack-only part, I will parse each word and push it onto a MyStack.
   Popping these words will allow me to build the word-reversed sentence.
   To find frequencies, I will use a copy of the stack and the drain-and-restore pattern.
   I will then repeat the same logic using a MyQueue object.

   Steps:
   1. Read a sentence string from user ending with a '.'.
   2. Count characters (excluding '.') and count words.
   3. For Stack: parse and push each word, pop to reverse, count frequencies.
   4. For Queue: parse and enqueue each word, rotate to reverse, count frequencies.
*/

class Stack {
public:
    Stack(int _maxCapacity = 0) {
        currentIndex = -1;
        maxCapacity = _maxCapacity;
        values = new string[maxCapacity];
    }
    Stack(const Stack& s) {
        maxCapacity = s.maxCapacity;
        currentIndex = s.currentIndex;
        values = new string[maxCapacity];
        for (int i = 0; i <= currentIndex; i++) {
            values[i] = s.values[i];
        }
    }
    virtual ~Stack() { delete[] values; }
    virtual void push(string v) = 0;
    virtual bool pop(string& value) = 0;
    bool isEmpty() { return (currentIndex < 0); }
    bool isFull()  { return (currentIndex == maxCapacity - 1); }
protected:
    int currentIndex;
    string* values;
    int maxCapacity;
};

class Queue {
public:
    Queue(int cap) : startOfQIndex(-1), topOfQIndex(-1), maxCapacity(cap), count(0) {}
    Queue(const Queue& q) : startOfQIndex(q.startOfQIndex), topOfQIndex(q.topOfQIndex),
        maxCapacity(q.maxCapacity), count(q.count) {}
    virtual ~Queue() {}
    bool isEmpty() const { return (count == 0); }
    bool isFull()  const { return (count == maxCapacity); }
    virtual void enqueue(string value) = 0;
    virtual string dequeue() = 0;
protected:
    int startOfQIndex;
    int topOfQIndex;
    int maxCapacity;
    int count;
};

class MyStack : public Stack {
public:
    MyStack(int _cap = 0) : Stack(_cap) {}
    MyStack(const MyStack& ma) : Stack(ma) {}
    void push(string v) {
        if (!isFull()) {
            values[++currentIndex] = v;
        }
    }
    bool pop(string& v) {
        if (!isEmpty()) {
            v = values[currentIndex--];
            return true;
        }
        return false;
    }
};

class MyQueue : public Queue {
public:
    MyQueue(int s) : Queue(s) {
        if (s != 0) values = new string[maxCapacity];
        else        values = nullptr;
    }
    MyQueue(const MyQueue& mq) : Queue(mq) {
        if (maxCapacity != 0) {
            values = new string[maxCapacity];
            for (int i = 0; i < maxCapacity; i++) {
                values[i] = mq.values[i];
            }
        } else {
            values = nullptr;
        }
    }
    ~MyQueue() {
        if (values != nullptr) {
            delete[] values;
            values = nullptr;
        }
    }
    void enqueue(string value) {
        if (isFull()) return;
        if (isEmpty()) {
            topOfQIndex = 0;
            startOfQIndex = 0;
        }
        else {
            startOfQIndex = (startOfQIndex + 1) % maxCapacity;
        }
        values[startOfQIndex] = value;
        count++;
    }
    string dequeue() {
        if (isEmpty()) return "";
        string val = values[topOfQIndex];
        count--;
        if (isEmpty()) {
            startOfQIndex = topOfQIndex = -1;
        }
        else {
            topOfQIndex = (topOfQIndex + 1) % maxCapacity;
        }
        return val;
    }
protected:
    string* values;
};

void sstack(string sentence) {
    int wordsCount = 0;
    string temp = "";

    // First pass: count words
    for (int i = 0; i < (int)sentence.length(); i++) {
        if (sentence[i] == ' ' || sentence[i] == '.') {
            if (temp != "") {
                wordsCount++;
                temp = "";
            }
        }
        else {
            temp += sentence[i];
        }
    }

    // Second pass: push words onto stack
    MyStack s(wordsCount);
    temp = "";
    for (int i = 0; i < (int)sentence.length(); i++) {
        if (sentence[i] == ' ' || sentence[i] == '.') {
            if (temp != "") {
                s.push(temp);
                temp = "";
            }
        }
        else {
            temp += sentence[i];
        }
    }

    // Word-Reverse: pop from stack (LIFO gives reverse order)
    cout << "Word-Reverse (Stack): ";
    MyStack copyS = s;
    string word;
    while (copyS.pop(word)) {
        cout << word;
        if (copyS.isEmpty()) {
            cout << ".";
        }
        else {
            cout << " ";
        }
    }
    cout << endl;

    // Word Frequencies using Stack (drain-and-restore pattern)
    cout << "Word Frequencies (Stack):" << endl;
    MyStack freqS = s;
    while (!freqS.isEmpty()) {
        string target;
        freqS.pop(target);
        int cnt = 1;
        int size = 0;

        // Count how many are in the remaining stack
        // Move rest to temp stack
        MyStack tempS(wordsCount);
        while (freqS.pop(word)) {
            tempS.push(word);
        }

        // Check each one
        MyStack restoreS(wordsCount);
        while (tempS.pop(word)) {
            if (word == target) {
                cnt++;
            }
            else {
                restoreS.push(word);
            }
        }

        // Restore non-matching words back into freqS
        while (restoreS.pop(word)) {
            freqS.push(word);
        }

        cout << target << " " << cnt << endl;
    }
}

void squeue(string sentence) {
    int wordsCount = 0;
    string temp = "";

    // First pass: count words
    for (int i = 0; i < (int)sentence.length(); i++) {
        if (sentence[i] == ' ' || sentence[i] == '.') {
            if (temp != "") {
                wordsCount++;
                temp = "";
            }
        }
        else {
            temp += sentence[i];
        }
    }

    // Second pass: enqueue words into queue
    MyQueue q(wordsCount);
    temp = "";
    for (int i = 0; i < (int)sentence.length(); i++) {
        if (sentence[i] == ' ' || sentence[i] == '.') {
            if (temp != "") {
                q.enqueue(temp);
                temp = "";
            }
        }
        else {
            temp += sentence[i];
        }
    }

    // Word-Reverse using Queue rotation technique
    // For each step, rotate (remaining-1) times so last word comes to front
    cout << "Word-Reverse (Queue): ";
    MyQueue rotateQ(wordsCount);
    for (int i = 0; i < wordsCount; i++) {
        rotateQ.enqueue(q.dequeue());
    }

    int remaining = wordsCount;
    string reverseResult = "";
    while (remaining > 0) {
        for (int r = 0; r < remaining - 1; r++) {
            rotateQ.enqueue(rotateQ.dequeue());
        }
        string w = rotateQ.dequeue();
        reverseResult += w;
        if (remaining > 1) reverseResult += " ";
        remaining--;
    }
    cout << reverseResult << "." << endl;

    // Word Frequencies using Queue (drain-and-restore pattern)
    // Re-fill a fresh queue for frequency counting
    MyQueue freqQ(wordsCount);
    temp = "";
    for (int i = 0; i < (int)sentence.length(); i++) {
        if (sentence[i] == ' ' || sentence[i] == '.') {
            if (temp != "") {
                freqQ.enqueue(temp);
                temp = "";
            }
        }
        else {
            temp += sentence[i];
        }
    }

    cout << "Word Frequencies (Queue):" << endl;
    int rem = wordsCount;
    while (rem > 0) {
        string target = freqQ.dequeue();
        int cnt = 1;
        int size = rem - 1;
        rem = 0;
        for (int i = 0; i < size; i++) {
            string current = freqQ.dequeue();
            if (current == target) {
                cnt++;
            }
            else {
                freqQ.enqueue(current);
                rem++;
            }
        }
        cout << target << " " << cnt << endl;
    }
}

int main() {
    string sentence;
    cout << "Enter sentence: ";
    getline(cin, sentence);

    int chars = 0;
    int words = 0;
    string word = "";

    for (int i = 0; i < (int)sentence.length(); i++) {
        if (sentence[i] != '.') {
            chars++;
        }
        if (sentence[i] == ' ' || sentence[i] == '.') {
            if (word != "") {
                words++;
                word = "";
            }
        }
        else {
            word += sentence[i];
        }
    }

    cout << "Characters: " << chars << endl;
    cout << "Words: " << words << endl;

    cout << endl << "--- Stack ---" << endl;
    sstack(sentence);

    cout << endl << "--- Queue ---" << endl;
    squeue(sentence);

    return 0;
}
