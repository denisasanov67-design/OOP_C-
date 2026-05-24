#include <iostream>
#include <fstream>
#include <vector>
#include <queue> //для приоритетной очереди 
#include "Header.h"

using namespace std;


int main() {

    setlocale(LC_ALL, "ru");

    // открытие файла в бинарном режиме 
    ifstream file("ДНК_Лабиринт.bmp", ios::binary);


    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    // содержит информацию о файле
    file.read((char*)&fileHeader, sizeof(fileHeader));
    // информация о изображении
    file.read((char*)&infoHeader, sizeof(infoHeader));

    // размеры изображения 
    int width = infoHeader.width;
    int height = infoHeader.height;

    // двухмерный массив для хранения всех пикселение изображения 
    vector<vector<Pixel>> image(height, vector<Pixel>(width));
    int padding = (4 - (width * 3) % 4) % 4; // размер выравнивания 


    file.seekg(fileHeader.bfOffBits, ios::beg); // указатель на начало данных пикселей 

    // Чтение BMP
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            Pixel pixel;
            file.read((char*)&pixel, sizeof(Pixel)); // чтение 3 байта из файла 
            image[y][x] = pixel;
        }
        file.ignore(padding); // пропускаем байты выравнивания
    }
    file.close();

    // ининциализаци ячеек 1 = стена
    vector<vector<int>> maze(height, vector<int>(width, 1)); 
    pair<int, int> start, finish;
    bool foundStart = false, foundFinish = false;

    // Создание лабиринта
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Pixel p = image[y][x];

            // Красный цвет = старт 
            if (p.r > 150 && p.g < 100 && p.b < 100) {
                maze[y][x] = 0;
                finish = { y, x };
                foundFinish = true;
            }
            // Зелёный цвет = финиш 
            else if (p.r < 100 && p.g > 150 && p.b < 100) {
                maze[y][x] = 0;
                start = { y, x };
                foundStart = true;
            }
            // Белый = проход
            else if (p.r > 200 && p.g > 200 && p.b > 200) {
                maze[y][x] = 0;
            }
        }
    }


    cout << "Старт: (" << start.first << ", " << start.second << ")\n";
    cout << "Финиш: (" << finish.first << ", " << finish.second << ")\n";

    // Алгоритм Дейкстры

    // бесконечное расстояние
    const int INF = 1e9;

    // Матрица расстояний от старта до каждой клетки
    vector<vector<int>> dist(height, vector<int>(width, INF));
    // Матрица родителей для восстановления пути
    vector<vector<pair<int, int>>> parent(height, vector<pair<int, int>>(width, { -1, -1 }));

    priority_queue<Node, vector<Node>, greater<Node>> pq;

    // Инициализация стартовой точки
    dist[start.first][start.second] = 0;
    pq.push({ 0, start.first, start.second });


    //направления для премещения
    int dx[4] = { 1, -1, 0, 0 };
    int dy[4] = { 0, 0, 1, -1 };


    // пока в очереди есть необработанные узлы 
    while (!pq.empty()) {
        Node cur = pq.top();
        pq.pop();

        int y = cur.y;
        int x = cur.x;

        if (cur.dist > dist[y][x])
            continue;

        if (y == finish.first && x == finish.second)
            break;

        for (int i = 0; i < 4; i++) {
            int ny = y + dy[i];
            int nx = x + dx[i];

            if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                maze[ny][nx] == 0) {

                if (dist[ny][nx] > dist[y][x] + 1) {
                    dist[ny][nx] = dist[y][x] + 1;
                    parent[ny][nx] = { y, x };
                    pq.push({ dist[ny][nx], ny, nx });
                }
            }
        }
    }

    // Восстановление пути
    pair<int, int> cur = finish;
    bool pathFound = false;

    while (cur != start) {
        int y = cur.first;
        int x = cur.second;

        // Закрашиваем путь красным цветом
        image[y][x].r = 255;
        image[y][x].g = 0;
        image[y][x].b = 0;

        cur = parent[y][x];

    }

    // Сохранение BMP
    ofstream out("result.bmp", ios::binary);
    out.write((char*)&fileHeader, sizeof(fileHeader));
    out.write((char*)&infoHeader, sizeof(infoHeader));

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            out.write((char*)&image[y][x], sizeof(Pixel));
        }
        char pad[3] = { 0, 0, 0 };
        out.write(pad, padding);
    }

    out.close();
    cout << "Файл сохранен";

    return 0;
}