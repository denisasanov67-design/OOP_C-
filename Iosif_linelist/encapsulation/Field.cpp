#include "Field.h"
#include <algorithm>

// Конструктор для опрделения первого игрока 
Field::Field(bool isRedFirst) {
    clear(isRedFirst);
}

// Очистка поля 
void Field::clear(bool isRedFirst) {
    isRedTurn = isRedFirst;
    winner = EMPTY;
    for (int i = 0; i < FIELD_WIDTH; i++)
        for (int j = 0; j < FIELD_HEIGHT; j++)
            cells[i][j] = EMPTY;
}

// Проверка победителя после хода
void Field::checkWinner() {
    const int DIR_NUMBER = 4;
    const int di[] = { 1, 0, 1, 1 };   
    const int dj[] = { 0, 1, 1, -1 };
    const int WIN_LENGTH = 4;

    for (int i = 0; i < FIELD_WIDTH; i++) {
        for (int j = 0; j < FIELD_HEIGHT; j++) {
            Cell start = cells[i][j];
            if (start == EMPTY) continue;

            for (int dir = 0; dir < DIR_NUMBER; dir++) {
                int length = 1, iline = i, jline = j;

                while (length < WIN_LENGTH) {
                    iline += di[dir];
                    jline += dj[dir];

                    if (iline < 0 || iline >= FIELD_WIDTH ||
                        jline < 0 || jline >= FIELD_HEIGHT)
                        break;
                    if (cells[iline][jline] != start)
                        break;

                    length++;
                }

                if (length == WIN_LENGTH) {
                    winner = start;
                    return;
                }
            }
        }
    }
}

// Выполнение хода
bool Field::makeTurn(int column) {
    if (winner != EMPTY || column < 1 || column > FIELD_WIDTH)
        return false;

    int col = column - 1;  


    for (int row = 0; row < FIELD_HEIGHT; row++) {
        if (cells[col][row] == EMPTY) {
            cells[col][row] = isRedTurn ? RED : YELLOW;
            checkWinner();
            isRedTurn = !isRedTurn;  
            return true;
        }
    }
    return false;  
}

// Проверка, окончена ли игра
bool Field::isOver() const {
    if (winner != EMPTY) return true;

    for (int i = 0; i < FIELD_WIDTH; i++)
        for (int j = 0; j < FIELD_HEIGHT; j++)
            if (cells[i][j] == EMPTY)
                return false;

    return true; 
}

// Получение состояния ячейки
Cell Field::getCell(int x, int y) const {
    if (x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT)
        return cells[x][y];
    return EMPTY;
}

// Чей ход сейчас
bool Field::isRedTurnNow() const {
    return isRedTurn;
}

// Победитель
Cell Field::getWinner() const {
    return winner;
}

// Вывод поля
void Field::print() const {
    cout << "\n  1 2 3 4 5 6 7\n";
    for (int row = FIELD_HEIGHT - 1; row >= 0; row--) {
        cout << row + 1 << " ";
        for (int col = 0; col < FIELD_WIDTH; col++) {
            char c;
            if (cells[col][row] == RED) c = 'R';
            else if (cells[col][row] == YELLOW) c = 'Y';
            else c = '.';
            cout << c << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Вывод результата
void Field::printResult() const {
    if (winner == RED)
        cout << "Победил красный игрок!" << endl;
    else if (winner == YELLOW)
        cout << "Победил жёлтый игрок!" << endl;
    else
        cout << "Ничья!" << endl;
}

// Клонирование поля 
Field Field::clone() const {
    Field copy;
    copy.isRedTurn = this->isRedTurn;
    copy.winner = this->winner;
    for (int i = 0; i < FIELD_WIDTH; i++)
        for (int j = 0; j < FIELD_HEIGHT; j++)
            copy.cells[i][j] = this->cells[i][j];
    return copy;
}