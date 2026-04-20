#include <iostream>
#include "LineList.h"
#include <ctime>
using namespace std;

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

int main(void) {
    setlocale(LC_ALL, "ru");
    int test[] = { 1000,5000,10000,50000,100000 };
    int size = sizeof(test) / sizeof(test[0]);
    int k = 2;

    cout << "LineList" << endl;
    for (int i = 0; i < size; i++) {
        int N = test[i];
        clock_t start = clock();
        int res = JosephProblemLineList<int>(N, k);
        clock_t end = clock();
        double time = double(end - start) / CLOCKS_PER_SEC;
        cout << N << "\t\t" << res << "\t\t" << time << " сек." << endl;
    }
    return 0;
}