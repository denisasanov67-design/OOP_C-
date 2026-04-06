#include <iostream>
#include "Header.h"
#include "string"

using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");
    DateTime d1;

    d1.input();

    if (!d1.Valid())
    {
        cout<<"Некорректная дата"<<endl;
        return 0;
    }

    cout<<"Форматы вывода:"<<endl;

    d1.Format1();
    d1.Format2();
    d1.Format3();

    string days[] = {
    "Воскресенье","Понедельник","Вторник",
    "Среда","Четверг","Пятница","Суббота" };

    cout<<"День недели: "<<days[d1.Week()]<<endl;

    DateTime d2;

    d2.input();

    cout << "Разница между датами: "
    << (d1 - d2)
    << " дней" << endl;


    DateTime easter = d1.getEsterDate(2024);

    cout<<"Пасха 2024: ";
    easter.Format1();

    return 0;
}
