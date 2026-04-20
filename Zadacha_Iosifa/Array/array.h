#ifndef ARRAY_H  
#define ARRAY_H

#include <iostream>
using namespace std;

const int DEFAULT_CAPACITY = 10;
class ArrayException {};

class Array {
    int* ptr;
    int size;
    int capacity;
    void increaseCapacity(int newCapacity);
public:
    explicit Array(int startCapacity = DEFAULT_CAPACITY);
    ~Array();
    Array(const Array& arr);
    Array& operator = (const Array& arr);
    int& operator[](int index);
    int operator[](int index) const;
    void insert(int elem, int index);
    void insert(int elem);
    void remove(int index);
    int getSize() const;
    friend ostream& operator<<(ostream& out, const Array& arr);
};


struct LineListException {};


template<class T>
class LineListElem {
    T data;
    LineListElem* next;
    template<class U> friend class LineList;
public:
    LineListElem(const T& adata, LineListElem* anext) : data(adata), next(anext) {}
    const T& getData() const { return data; }
    LineListElem* getNext() { return next; }
    void setNext(LineListElem* anext) { next = anext; }
};

template <class T>
class LineList {
    LineListElem<T>* start;

public:
    LineList() : start(nullptr) {}

    ~LineList() {
        while (start) {
            LineListElem<T>* temp = start->getNext();
            delete start;
            start = temp;
        }
    }

    void insertLast(const T& data) {
        if (!start) {
            start = new LineListElem<T>(data, nullptr);
        }
        else {
            LineListElem<T>* curr = start;
            while (curr->getNext()) curr = curr->getNext();
            curr->setNext(new LineListElem<T>(data, nullptr));
        }
    }

    LineListElem<T>* getStart() { return start; }

    void deleteAfter(LineListElem<T>* ptr) {
        if (!ptr || !ptr->next) throw LineListException();
        LineListElem<T>* temp = ptr->next;
        ptr->next = temp->next;
        if (temp == start) start = ptr->next;
        delete temp;
    }

    void setStart(LineListElem<T>* newStart) { start = newStart; }
};


template <class T>
int JosephProblemLineList(int N, int k) {
    if (N <= 0) return -1;
    if (k <= 0) k = 1;

    LineList<int> list;

    for (int i = 0; i < N; i++) {
        list.insertLast(i);
    }

    LineListElem<int>* curr = list.getStart();
    while (curr && curr->getNext()) curr = curr->getNext();
    if (curr) curr->setNext(list.getStart());

    LineListElem<int>* prev = curr;
    curr = list.getStart();

    while (curr->getNext() != curr) {
        for (int i = 0; i < k - 1; ++i) {
            prev = curr;
            curr = curr->getNext();
        }
        prev->setNext(curr->getNext());
        if (curr == list.getStart()) {
            list.setStart(curr->getNext());
        }
        LineListElem<int>* toDelete = curr;
        curr = prev->getNext();
        delete toDelete;
    }

    int result = curr->getData();
    delete curr;
    const_cast<LineList<int>&>(list).setStart(nullptr);

    return result;
}

#endif
