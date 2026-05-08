#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int FIELD_WIDTH = 7;   // Ширина поля
const int FIELD_HEIGHT = 6;  // Высота поля 

// Состояние ячейки
enum Cell {
    EMPTY = 0, 
    RED = 1,
    YELLOW = 2
};

class Field {

private:
    Cell cells[FIELD_WIDTH][FIELD_HEIGHT];  
    bool isRedTurn;      // Чей ход
    Cell winner;        // Победитель 
    void checkWinner();  

public:
    Field(bool isRedFirst = true);
        
    void clear(bool redFirst = true);
    bool makeTurn(int column);
    bool isOver() const;

    Cell getCell(int x, int y) const;
    bool isRedTurnNow() const;
    Cell getWinner() const;

    void print() const;
    void printResult() const;

    // Для тестов: получить копию поля
    Field clone() const;
};

#endif 