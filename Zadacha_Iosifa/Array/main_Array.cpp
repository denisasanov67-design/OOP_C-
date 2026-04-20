#include <iostream>
#include "array.h"
#include "ctime"
using namespace std;

/*
int main(void)
{
    setlocale(LC_ALL, "ru");
    try {
        Array arr(4);

        for (int i = 0; i < 6; i++) {
            arr.insert(i + 1);
        }
        cout << "Начальный массив:" << endl << arr << endl;



        for (int i = 0; i < 8; i += 2) {
            arr.insert(10 + i, i);
        }
        cout << "После вставки четных чисел:" << endl << arr << endl;


        for (int i = 1; i < 8; i += 2) {
            arr[i] = 20 + i;
        }
        cout << "После изменения через []:" << endl << arr << endl;


        for (int i = 6; i >= 0; i -= 3) {
            arr.remove(i);
        }
        cout << "После удаления:" << endl << arr << endl;


        Array arrCopy = arr;
        Array arr2(2);
        arr2 = arr;
        cout << "Копия массива:" << endl << arrCopy << endl;
        cout << arr[100] << endl;

    }
    catch (ArrayException& e) {
        cerr << "Произошла ошибка: выход за границы массива!" << endl;
    }

    return 0;
}
*/
int JosephProblem(int N, int k) {

    Array arr(N);
    for (int i = 0; i < N; i++) {
        arr.insert(i);
    }
    int index = 0;
    int currentSize = N;

    while (arr.getSize() > 1) {
        index = (index + k - 1) % arr.getSize();

        arr.remove(index);
        currentSize--;

        if (index == currentSize) index = 0;
    }
    return arr[0];
}


int JosephProblemListWrapper(int N, int k) {
    return JosephProblemLineList<int>(N, k);
}

int main(void)
{
    setlocale(LC_ALL, "ru");
    int test[] = { 1000,5000,10000,50000,100000 };
    int size = sizeof(test) / sizeof(test[0]);
    int k = 2;
    cout << "Array" << endl;
    for (int i = 0; i < size; i++) {
        int N = test[i];
        clock_t start = clock();
        int res = JosephProblem(N, k);
        clock_t end = clock();
        double time = double(end - start) / CLOCKS_PER_SEC;
        cout << N << "\t\t" << res << "\t\t" << time << " сек." << endl;
    }

    cout << "LineList" << endl;
    for (int i = 0; i < size; i++) {
        int N = test[i];
        clock_t start = clock();
        int res = JosephProblemListWrapper(N, k);
        clock_t end = clock();
        double time = double(end - start) / CLOCKS_PER_SEC;
        cout << N << "\t\t" << res << "\t\t" << time << " сек." << endl;
    }
    return 0;
}
