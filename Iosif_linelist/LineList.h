#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
using namespace std;

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
    LineList();
    ~LineList();
    void insertLast(const T& data);
    LineListElem<T>* getStart();
    void deleteAfter(LineListElem<T>* ptr);
    void setStart(LineListElem<T>* newStart);
};

#include "array.tpp"

#endif