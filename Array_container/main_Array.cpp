#include <iostream>
#include "array.h"

using namespace std;

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