#include <iostream>
#include "Field.h"

using namespace std;

// Простые тесты
void testVerticalWin() {
    Field f;
    f.makeTurn(4); f.makeTurn(1);
    f.makeTurn(4); f.makeTurn(2);
    f.makeTurn(4); f.makeTurn(3);
    f.makeTurn(4);
    cout << "Test vertical: " << (f.getWinner() == RED ? "PASS" : "FAIL") << endl;
}

void testHorizontalWin() {
    Field f;
    for (int i = 0; i < 3; i++) {
        f.makeTurn(1); f.makeTurn(2);
        f.makeTurn(3); f.makeTurn(4);
    }
    f.makeTurn(1); f.makeTurn(2);
    f.makeTurn(3); f.makeTurn(4);
    cout << "Test horizontal: " << (f.getWinner() == YELLOW ? "PASS" : "FAIL") << endl;
}

void testTurnSwitch() {
    Field f;
    bool ok = f.isRedTurnNow();
    f.makeTurn(1);
    ok = ok && !f.isRedTurnNow();
    f.makeTurn(1);
    ok = ok && f.isRedTurnNow();
    cout << "Test turn switch: " << (ok ? "PASS" : "FAIL") << endl;
}

void testInvalidMoves() {
    Field f;
    bool ok = !f.makeTurn(0) && !f.makeTurn(8);
    for (int i = 0; i < 6; i++) f.makeTurn(1);
    ok = ok && !f.makeTurn(1);
    cout << "Test invalid moves: " << (ok ? "PASS" : "FAIL") << endl;
}

void runTests() {
    cout << "\nRunning tests:" << endl;
    testVerticalWin();
    testHorizontalWin();
    testTurnSwitch();
    testInvalidMoves();
    cout << "Tests done.\n" << endl;
}

// Игра
void playGame() {
    Field field;

    while (!field.isOver()) {
        field.print();
        cout << "Turn: " << (field.isRedTurnNow() ? "Red" : "Yellow") << endl;
        cout << "Column (1-7): ";

        int column;
        cin >> column;

        if (column == 0) return;
        if (!field.makeTurn(column))
            cout << "Invalid move!" << endl;
    }

    field.print();
    field.printResult();
}

int main() {
    setlocale(LC_ALL, "Russian");

    runTests();
    playGame();

    return 0;
}