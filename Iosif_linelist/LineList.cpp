#ifndef ARRAY_TPP
#define ARRAY_TPP

#include "LineList.h"

template <class T>
LineList<T>::LineList() : start(nullptr) {}

template <class T>
LineList<T>::~LineList() {
    while (start) {
        LineListElem<T>* temp = start->getNext();
        delete start;
        start = temp;
    }
}

template <class T>
void LineList<T>::insertLast(const T& data) {
    if (!start) {
        start = new LineListElem<T>(data, nullptr);
    }
    else {
        LineListElem<T>* curr = start;
        while (curr->getNext()) curr = curr->getNext();
        curr->setNext(new LineListElem<T>(data, nullptr));
    }
}

template <class T>
LineListElem<T>* LineList<T>::getStart() {
    return start;
}

template <class T>
void LineList<T>::deleteAfter(LineListElem<T>* ptr) {
    if (!ptr || !ptr->next) throw LineListException();
    LineListElem<T>* temp = ptr->next;
    ptr->next = temp->next;
    if (temp == start) start = ptr->next;
    delete temp;
}

template <class T>
void LineList<T>::setStart(LineListElem<T>* newStart) {
    start = newStart;
}

#endif