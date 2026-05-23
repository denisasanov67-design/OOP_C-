#include "Header.h"

#include <fstream>
#include <sstream>

using namespace std;

int main() {

    setlocale(LC_ALL, "Russian");

    try {

        string inputDate;

        cout << "Введите дату (дд.мм.гггг): ";
        cin >> inputDate;

        DateTime date = DateTime::parseISO(inputDate);

        string filename = date.getFileName();

        ifstream file(filename);

        if (!file.is_open()) {

            cout << "Ошибка: файл "
                << filename
                << " не найден."
                << endl;

            return 1;
        }

        string line;

        string ymd, hms;
        double t, r, el, az, fi, lg;

        bool dataFound = false;

        // Пропуск заголовка
        while (getline(file, line)) {

            if (!line.empty() &&
                isdigit(line[0])) {

                stringstream ss(line);

                ss >> ymd
                    >> hms
                    >> t
                    >> r
                    >> el
                    >> az
                    >> fi
                    >> lg;

                dataFound = true;

                break;
            }
        }

        if (!dataFound) {

            cout << "Ошибка: данные в файле отсутствуют."
                << endl;

            return 1;
        }

        string riseTime;
        string culmTime;
        string setTime;

        double prevEl = el;
        double maxEl = el;

        culmTime = hms;

        // Чтение остальных строк
        while (file >> ymd
            >> hms
            >> t
            >> r
            >> el
            >> az
            >> fi
            >> lg) {

            // Восход
            if (riseTime.empty() &&
                prevEl < 0 &&
                el >= 0) {

                riseTime = hms;
            }

            // Заход
            if (setTime.empty() &&
                prevEl >= 0 &&
                el < 0) {

                setTime = hms;
            }

            // Кульминация
            if (el > maxEl) {

                maxEl = el;
                culmTime = hms;
            }

            prevEl = el;
        }

        cout << endl;

        cout << "Дата: "
            << date.getFormattedDate()
            << endl;

        cout << "Восход Луны: "
            << formatTime(riseTime)
            << endl;

        cout << "Кульминация Луны: "
            << formatTime(culmTime)
            << endl;

        cout << "Заход Луны: "
            << formatTime(setTime)
            << endl;
    }

    catch (const DateTimeException& ex) {

        cout << ex.what() << endl;
    }

    catch (const exception& ex) {

        cout << "Ошибка: "
            << ex.what()
            << endl;
    }

    return 0;
}