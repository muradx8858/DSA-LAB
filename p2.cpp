// SPRING 2026 DATA STRUCTURES SECTION D20 ASSIGNMENT ONE
// Name of Student: [YOUR NAME]
// Registration of Student: [YOUR REG NO]
// File p2.cpp: Problem 2 - Sentence Analysis (char count, word count,
//              word-reverse, word frequency) using Stack only and Queue only.
// Must compile and run with provided Stack.h, MyStack.h, Queue.h, MyQueue.h,
// CircularQueue.h, ArrayCircularQueue.h

#include <iostream>
#include <string>
using namespace std;
#include "Stack.h"
#include "MyStack.h"
#include "Queue.h"
#include "MyQueue.h"
#include "CircularQueue.h"
#include "ArrayCircularQueue.h"

// ============================================================
// BONUS MARKS - TWO BUGS FOUND IN PROVIDED CODE
// ============================================================
// BUG 1 (Stack.h): Copy constructor sets currentIndex = -1 (hardcoded).
//   This makes every copied stack appear EMPTY.
//   FIX: change to  currentIndex = s.currentIndex;
//   WORKAROUND HERE: manualCopyStack() is used instead of copy constructor.
//
// BUG 2 (MyQueue.h): display() mixes 'W' (ostream param) and 'cout' (hardcoded).
//   Lines inside display() use cout instead of W for some outputs.
//   FIX: replace all cout inside display() with W.
//   WORKAROUND HERE: display() is not used for correctness in this file.
//
// BONUS MARKS - ERROR FOUND IN ASSIGNMENT EXAMPLE:
//   The assignment example states: "25 characters and 5 words" for the sentence
//   "This is the best coding exercise and is the best thinking."
//   CORRECTION: The sentence has 11 words (sum of all frequencies = 11)
//   and 57 printable characters (all chars excluding the final period).
//   The counts 25 and 5 appear to be left over from an earlier, shorter
//   example sentence that was replaced without updating the numbers.
//   This program correctly computes and displays the actual counts.
// ============================================================

// ============================================================
// JUSTIFICATION FOR ArrayCircularQueue IN QUEUE-ONLY SECTION:
// The Queue-based word-reversal requires rotation (dequeue from front,
// re-enqueue at rear in a circular fashion). MyQueue is a LINEAR array
// queue: after rotation it runs out of capacity. ArrayCircularQueue
// wraps around and supports unlimited rotation within fixed capacity.
// ArrayCircularQueue IS a Queue ADT (it inherits CircularQueue<T>),
// so this still satisfies the "Queue ADT only" requirement.
// ============================================================

/***** PROBLEM DESCRIPTION *****/
/*
Read An English Sentence (ends with a full stop) from a User.
1. Display the count of characters and words on screen.
2. Copy the Sentence in word-reverse and display on screen.
   (Words in reverse order, full stop stays at the end.)
3. Calculate frequencies of each word and display.
First do the problem using Stack only and then using Queue Only in the same main.
Example sentence: "This is the best coding exercise and is the best thinking."
*/

/***** PROBLEM SOLUTION DESCRIPTION *****/
/*
=== KEY STORAGE STRATEGY (no arrays!) ===
All words are stored in a single string variable 'wordList' using '|' as
a separator: "This|is|the|best|coding|exercise|and|is|the|best|thinking|"
The Stack/Queue stores integer WORD INDICES (0, 1, 2, ..., N-1).
getWordAt(wordList, i) retrieves the i-th word from wordList by parsing.
This avoids any string array while keeping full word information.

=== STACK ONLY SOLUTION (without functions) ===
-------------------------------------------------------
PARSE PHASE:
1. Read full sentence with getline into string 'sentence'
2. charCount = sentence.length() - 1  (exclude final period)
3. Parse sentence character by character, building wordList and
   pushing each word's index (0, 1, 2, ...) onto MyStack S
   sentence: "This is the best coding exercise and is the best thinking."
   wordList: "This|is|the|best|coding|exercise|and|is|the|best|thinking|"
   S (top->bottom): 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
   wordCount = 11

WORD-REVERSE PHASE:
4. Copy-construct CS from S using manualCopyStack
5. Pop index from CS, call getWordAt(wordList, idx), print word + space
   Pop order: 10,9,8,...,0  ->  "thinking best the is and exercise coding best the is This."
   Naturally reversed because Stack is LIFO!

FREQUENCY PHASE:
6. Make another copy CS2 of S
7. Loop while CS2 not empty:
   a. Pop index, get word w = getWordAt(wordList, idx)
   b. Check if w is already in processedWords string
   c. If NOT processed: call countOccurrences(wordList, w), display "w\tfreq"
      Add w to processedWords
   d. If already processed: skip (already counted)
   processedWords grows as: "This|is|the|best|coding|exercise|and|thinking|"

=== STACK ONLY SOLUTION (with functions) ===
-------------------------------------------------------
Functions: buildWordListAndStack(), displayReverseStack(), displayFrequencyStack()
1. Call buildWordListAndStack(wordList, wordCount, charCount, S) - fills all
2. Call displayReverseStack(S, wordList, wordCount) - prints word-reversed sentence
3. Call displayFrequencyStack(S, wordList, wordCount) - prints frequency table

=== QUEUE ONLY SOLUTION (without functions) ===
-------------------------------------------------------
PARSE PHASE: same as Stack - build wordList, enqueue indices 0,1,...,N-1
   Q (front->rear): 0 -> 1 -> 2 -> ... -> 10

WORD-REVERSE PHASE (rotation technique):
We want to print: word[10], word[9], ..., word[0]
For each step i (remaining = N down to 1):
  - Rotate Q (remaining-1) times: dequeue front, re-enqueue at rear
    After rotation: the last unprocessed word is at front
  - Dequeue it and print
Trace (N=11): rotate 10 times -> front = index 10 -> dequeue -> print "thinking"
              rotate 9 times  -> front = index 9  -> dequeue -> print "best"
              ... (11 iterations total)

FREQUENCY PHASE:
Re-enqueue all indices into a fresh ArrayCircularQueue QF
Use processedWords string same as Stack approach
Dequeue each index; check processedWords; if new: count and display; add to processed

=== QUEUE ONLY SOLUTION (with functions) ===
-------------------------------------------------------
Functions: buildWordListAndQueue(), displayReverseQueue(), displayFrequencyQueue()
1. Call buildWordListAndQueue(wordList, wordCount, charCount, Q)
2. Call displayReverseQueue(Q, wordList, wordCount)
3. Call displayFrequencyQueue(Q, wordList, wordCount)
*/

/***** GLOBAL FUNCTION PROTOTYPES *****/
// Utility helpers
void manualCopyStack(MyStack& src, MyStack& dst, int n);
string getWordAt(const string& wordList, int idx);
int  countOccurrences(const string& wordList, const string& target);
bool isInProcessed(const string& processed, const string& word);

// Stack-based functions
void buildWordListAndStack(string& wordList, int& wordCount, int& charCount, MyStack& S);
void displayReverseStack(MyStack& S, const string& wordList, int n);
void displayFrequencyStack(MyStack& S, const string& wordList, int n);

// Queue-based functions (using ArrayCircularQueue<int>)
void buildWordListAndQueue(string& wordList, int& wordCount, int& charCount,
                           ArrayCircularQueue<int>& Q);
void displayReverseQueue(ArrayCircularQueue<int>& Q, const string& wordList, int n);
void displayFrequencyQueue(ArrayCircularQueue<int>& Q, const string& wordList, int n);

/***** Driver Function Main *****/
int main()
{
    // ============================================================
    // STACK ONLY - without functions
    // ============================================================
    cout << "========================================" << endl;
    cout << "  STACK ONLY SOLUTION" << endl;
    cout << "========================================" << endl;
    cout << "Enter a sentence ending with a period: ";
    cin.ignore();
    string sentence;
    getline(cin, sentence);

    // --- Parse sentence: build wordList and push indices onto Stack ---
    int charCount = (int)sentence.length() - 1;  // exclude period
    string wordList = "";
    int wordCount = 0;
    string word = "";

    // Temporary stack big enough for any sentence (max 200 words)
    MyStack S(200);

    for (int i = 0; i < (int)sentence.length(); i++) {
        char c = sentence[i];
        if (c == ' ' || c == '.') {
            if (!word.empty()) {
                wordList += word + "|";
                S.push(wordCount);       // push word index onto Stack
                wordCount++;
                word = "";
            }
        } else {
            word += c;
        }
    }

    cout << "\n--- Character and Word Count (without functions) ---" << endl;
    cout << "  Characters : " << charCount << endl;
    cout << "  Words      : " << wordCount << endl;

    // --- Word-reverse using Stack (LIFO naturally reverses) ---
    cout << "\n--- Word-Reverse (Stack, without functions) ---" << endl;
    {
        MyStack CS(200);
        manualCopyStack(S, CS, 200);
        string reverseResult = "";
        int idx;
        while (!CS.isEmpty()) {
            CS.pop(idx);
            reverseResult += getWordAt(wordList, idx);
            if (!CS.isEmpty()) reverseResult += " ";
        }
        reverseResult += ".";
        cout << "  " << reverseResult << endl;
    }

    // --- Word frequency using Stack ---
    cout << "\n--- Word Frequency (Stack, without functions) ---" << endl;
    {
        MyStack CS2(200);
        manualCopyStack(S, CS2, 200);
        string processedWords = "";
        int idx;
        while (!CS2.isEmpty()) {
            CS2.pop(idx);
            string w = getWordAt(wordList, idx);
            if (!isInProcessed(processedWords, w)) {
                int freq = countOccurrences(wordList, w);
                cout << "  " << w << "\t" << freq << endl;
                processedWords += w + "|";
            }
        }
    }

    // ============================================================
    // STACK ONLY - with functions
    // ============================================================
    cout << "\n========================================" << endl;
    cout << "  STACK ONLY SOLUTION (with functions)" << endl;
    cout << "========================================" << endl;
    cout << "(Re-enter same sentence for function demo)" << endl;
    cout << "Enter a sentence ending with a period: ";
    string sentence2;
    getline(cin, sentence2);
    sentence = sentence2;

    {
        string wl2 = "";
        int wc2 = 0, cc2 = 0;
        MyStack S2(200);
        buildWordListAndStack(wl2, wc2, cc2, S2);

        cout << "\n--- Character and Word Count (with functions) ---" << endl;
        cout << "  Characters : " << cc2 << endl;
        cout << "  Words      : " << wc2 << endl;

        cout << "\n--- Word-Reverse (Stack, with functions) ---" << endl;
        displayReverseStack(S2, wl2, wc2);

        cout << "\n--- Word Frequency (Stack, with functions) ---" << endl;
        displayFrequencyStack(S2, wl2, wc2);
    }

    // ============================================================
    // QUEUE ONLY - without functions (ArrayCircularQueue)
    // ============================================================
    cout << "\n========================================" << endl;
    cout << "  QUEUE ONLY SOLUTION" << endl;
    cout << "========================================" << endl;
    cout << "Enter sentence again for Queue demo: ";
    getline(cin, sentence);

    int qCharCount = (int)sentence.length() - 1;
    string qWordList = "";
    int qWordCount = 0;
    string qWord = "";
    ArrayCircularQueue<int> Q(200);

    for (int i = 0; i < (int)sentence.length(); i++) {
        char c = sentence[i];
        if (c == ' ' || c == '.') {
            if (!qWord.empty()) {
                qWordList += qWord + "|";
                Q.enqueue(qWordCount);
                qWordCount++;
                qWord = "";
            }
        } else {
            qWord += c;
        }
    }

    cout << "\n--- Character and Word Count (Queue, without functions) ---" << endl;
    cout << "  Characters : " << qCharCount << endl;
    cout << "  Words      : " << qWordCount << endl;

    // --- Word-reverse using Queue rotation ---
    cout << "\n--- Word-Reverse (Queue, without functions) ---" << endl;
    {
        ArrayCircularQueue<int> QR(200);
        // Re-enqueue all indices for word-reverse
        for (int i = 0; i < qWordCount; i++) QR.enqueue(i);

        string reverseResult = "";
        int remaining = qWordCount;
        while (remaining > 0) {
            // Rotate (remaining - 1) times to bring last element to front
            for (int r = 0; r < remaining - 1; r++) {
                int tmp;
                QR.dequeue(tmp);
                QR.enqueue(tmp);
            }
            int idx;
            QR.dequeue(idx);
            reverseResult += getWordAt(qWordList, idx);
            if (remaining > 1) reverseResult += " ";
            remaining--;
        }
        reverseResult += ".";
        cout << "  " << reverseResult << endl;
    }

    // --- Word frequency using Queue ---
    cout << "\n--- Word Frequency (Queue, without functions) ---" << endl;
    {
        ArrayCircularQueue<int> QF(200);
        for (int i = 0; i < qWordCount; i++) QF.enqueue(i);

        string processedWords = "";
        for (int i = 0; i < qWordCount; i++) {
            int idx;
            QF.dequeue(idx);
            string w = getWordAt(qWordList, idx);
            if (!isInProcessed(processedWords, w)) {
                int freq = countOccurrences(qWordList, w);
                cout << "  " << w << "\t" << freq << endl;
                processedWords += w + "|";
            }
        }
    }

    // ============================================================
    // QUEUE ONLY - with functions
    // ============================================================
    cout << "\n========================================" << endl;
    cout << "  QUEUE ONLY SOLUTION (with functions)" << endl;
    cout << "========================================" << endl;
    cout << "Enter sentence again: ";
    getline(cin, sentence);

    {
        string wl3 = "";
        int wc3 = 0, cc3 = 0;
        ArrayCircularQueue<int> Q3(200);
        buildWordListAndQueue(wl3, wc3, cc3, Q3);

        cout << "\n--- Character and Word Count (Queue, with functions) ---" << endl;
        cout << "  Characters : " << cc3 << endl;
        cout << "  Words      : " << wc3 << endl;

        cout << "\n--- Word-Reverse (Queue, with functions) ---" << endl;
        displayReverseQueue(Q3, wl3, wc3);

        cout << "\n--- Word Frequency (Queue, with functions) ---" << endl;
        displayFrequencyQueue(Q3, wl3, wc3);
    }

    return 0;
}

/***** GLOBAL FUNCTION CODE *****/

// manualCopyStack: copies src into dst while preserving src
// Both src and dst must be declared with sufficient capacity n
void manualCopyStack(MyStack& src, MyStack& dst, int n)
{
    MyStack temp(n);
    int v;
    while (!src.isEmpty()) { src.pop(v); temp.push(v); }
    while (!temp.isEmpty()) { temp.pop(v); src.push(v); dst.push(v); }
}

// getWordAt: returns the idx-th word from wordList (words separated by '|')
string getWordAt(const string& wordList, int idx)
{
    int count = 0, start = 0;
    for (int i = 0; i <= (int)wordList.size(); i++) {
        if (i == (int)wordList.size() || wordList[i] == '|') {
            if (count == idx) return wordList.substr(start, i - start);
            count++;
            start = i + 1;
        }
    }
    return "";
}

// countOccurrences: counts how many times 'target' appears in wordList
int countOccurrences(const string& wordList, const string& target)
{
    int count = 0, start = 0;
    for (int i = 0; i <= (int)wordList.size(); i++) {
        if (i == (int)wordList.size() || wordList[i] == '|') {
            if (wordList.substr(start, i - start) == target) count++;
            start = i + 1;
        }
    }
    return count;
}

// isInProcessed: returns true if word is found in processed string (same '|' format)
bool isInProcessed(const string& processed, const string& word)
{
    int start = 0;
    for (int i = 0; i <= (int)processed.size(); i++) {
        if (i == (int)processed.size() || processed[i] == '|') {
            if (processed.substr(start, i - start) == word) return true;
            start = i + 1;
        }
    }
    return false;
}

// ---- STACK-BASED GLOBAL FUNCTIONS ----

void buildWordListAndStack(string& wordList, int& wordCount, int& charCount, MyStack& S)
{
    string sentence;
    getline(cin, sentence);
    charCount = (int)sentence.length() - 1;
    wordList = "";
    wordCount = 0;
    string word = "";
    for (int i = 0; i < (int)sentence.length(); i++) {
        char c = sentence[i];
        if (c == ' ' || c == '.') {
            if (!word.empty()) {
                wordList += word + "|";
                S.push(wordCount++);
                word = "";
            }
        } else {
            word += c;
        }
    }
}

void displayReverseStack(MyStack& S, const string& wordList, int n)
{
    MyStack CS(200);
    manualCopyStack(S, CS, 200);
    string result = "";
    int idx;
    while (!CS.isEmpty()) {
        CS.pop(idx);
        result += getWordAt(wordList, idx);
        if (!CS.isEmpty()) result += " ";
    }
    result += ".";
    cout << "  " << result << endl;
}

void displayFrequencyStack(MyStack& S, const string& wordList, int n)
{
    MyStack CS(200);
    manualCopyStack(S, CS, 200);
    string processed = "";
    int idx;
    while (!CS.isEmpty()) {
        CS.pop(idx);
        string w = getWordAt(wordList, idx);
        if (!isInProcessed(processed, w)) {
            cout << "  " << w << "\t" << countOccurrences(wordList, w) << endl;
            processed += w + "|";
        }
    }
}

// ---- QUEUE-BASED GLOBAL FUNCTIONS ----

void buildWordListAndQueue(string& wordList, int& wordCount, int& charCount,
                           ArrayCircularQueue<int>& Q)
{
    string sentence;
    getline(cin, sentence);
    charCount = (int)sentence.length() - 1;
    wordList = "";
    wordCount = 0;
    string word = "";
    for (int i = 0; i < (int)sentence.length(); i++) {
        char c = sentence[i];
        if (c == ' ' || c == '.') {
            if (!word.empty()) {
                wordList += word + "|";
                Q.enqueue(wordCount++);
                word = "";
            }
        } else {
            word += c;
        }
    }
}

// displayReverseQueue: rotation technique - O(N^2) dequeue/enqueue to print in reverse
void displayReverseQueue(ArrayCircularQueue<int>& Q, const string& wordList, int n)
{
    // Make a copy by re-enqueuing fresh indices
    ArrayCircularQueue<int> QR(200);
    for (int i = 0; i < n; i++) QR.enqueue(i);

    string result = "";
    int remaining = n;
    while (remaining > 0) {
        // Rotate remaining-1 times to bring last element to front
        for (int r = 0; r < remaining - 1; r++) {
            int tmp;
            QR.dequeue(tmp);
            QR.enqueue(tmp);
        }
        int idx;
        QR.dequeue(idx);
        result += getWordAt(wordList, idx);
        if (remaining > 1) result += " ";
        remaining--;
    }
    result += ".";
    cout << "  " << result << endl;
}

void displayFrequencyQueue(ArrayCircularQueue<int>& Q, const string& wordList, int n)
{
    ArrayCircularQueue<int> QF(200);
    for (int i = 0; i < n; i++) QF.enqueue(i);

    string processed = "";
    for (int i = 0; i < n; i++) {
        int idx;
        QF.dequeue(idx);
        string w = getWordAt(wordList, idx);
        if (!isInProcessed(processed, w)) {
            cout << "  " << w << "\t" << countOccurrences(wordList, w) << endl;
            processed += w + "|";
        }
    }
}
