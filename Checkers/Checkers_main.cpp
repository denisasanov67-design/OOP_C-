#include "Header.h"

int main() {
    // Чтение доски из файла
    Board board = FileManager::read_board("text.txt");

    // Создание решателя для чёрных (we_are_white = false) 
    // с максимальной глубиной 6 полуходов (3 хода)
    Solver solver(false, 6);

    // Запуск решения (ход чёрных = false)
    solver.solve(board, false);

    // Запись результата в файл
    FileManager::write_result("output.txt", solver);

    return 0;
}