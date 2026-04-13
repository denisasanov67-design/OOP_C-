#include <iostream>
#include <filesystem>
#include "Header.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;    

namespace fs = filesystem;

int main()
{
    string path = "Moon"; // путь к папке с данными 
    setlocale(LC_ALL, "Russian");

    DateTime Moon;

    Moon.inputMoonDate();
    Moon.Format1();

    string Year = to_string(Moon.getYear()); // хранение года 

    for (const auto& file : fs::directory_iterator(path))
    {
        string filename = file.path().filename().string().substr(4, 4); // отдлельное название каждого файла 

        if (filename != Year) continue;
  
        ifstream inFile(file.path()); // открытие файла  

        string line; 
        getline(inFile, line); // пропускаем заголовок 
        getline(inFile, line); // пропускаем HMS
        getline(inFile, line); // пропускаем T
        getline(inFile, line); // пропускаем R
        getline(inFile, line); // пропускаем El
        getline(inFile, line); // пропускаем Az
        getline(inFile, line); // пропускаем FI
        getline(inFile, line); // пропускаем LG

        double prev_El = 0, max_El = -1000; 
        string rise, // переменненная для хранения данных о восходе 
               set, // переменная для хранения данных о заходе 
               culm, // переменная для хранения данных о кульминации 
               date; // переменная для хранения даты
        bool first = true;

        while (getline(inFile, line)) 
        {

            stringstream ss(line);
            string ymd, hms;
            double T, R, El;

            ss >> ymd >> hms >> T >> R >> El;

                if (date.empty()) date = ymd;

                if (!first && prev_El < 0 && El >= 0)
                {
                    rise = hms;
                }

                if (!first && prev_El >= 0 && El < 0)
                {
                    set = hms;
                }

                if (El > max_El)
                {
                    max_El = El;
                    culm = hms;
                }

                prev_El = El;
                first = false;
            }
            inFile.close();

  
            cout << "\nДата: " << date.substr(6, 2) << "." << date.substr(4, 2) << "." << date.substr(0, 4) << endl;
            cout << "Восход Луны: " << Moon.formatTime(rise) << endl;
            cout << "Кульминация Луны: " << Moon.formatTime(culm) << endl;
            cout << "Заход Луны: " << Moon.formatTime(set) << endl;        
        }
        return 0;
    }
    
    /*DateTime d1;

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
    */

