#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// Константы для цветов
const char WHITE_PIECE = 'w';
const char WHITE_KING = 'W';
const char BLACK_PIECE = 'b';
const char BLACK_KING = 'B';
const char EMPTY = '.';

// Структура для описания хода (поддерживает цепочки)
struct Move {
    int start_x, start_y; // Откуда
    vector<pair<int, int>> path; // Весь путь: start -> клетка1 -> клетка2 -> ... -> end
    vector<pair<int, int>> captures; // Список срубленных шашек в порядке взятия

    int end_x() const { return path.back().first; }
    int end_y() const { return path.back().second; }
};

// Класс доски
class Board {
private:
    char cells[8][8];

public:
    Board();
    Board clone() const;
    bool is_valid_pos(int x, int y) const;
    char get_piece(int x, int y) const;
    void set_piece(int x, int y, char piece);
    int count_white() const;
    int count_black() const;
    bool is_white(char p) const;
    bool is_black(char p) const;
    bool is_enemy(char p1, char p2) const;
};

// Класс генератора ходов
class MoveGenerator {
private:
    const Board* board;
    bool is_white_turn;

    void find_captures_recursive(int x, int y, char current_piece,
        const vector<pair<int, int>>& current_path,
        const vector<pair<int, int>>& captures_so_far,
        int original_x, int original_y,
        vector<Move>& result_moves) const;

public:
    MoveGenerator(const Board& b, bool white_turn);
    vector<Move> get_all_moves() const;
};

// Структура для результата
struct SolverResult {
    bool solved = false;
    vector<Move> path;
};

// Класс решателя
class Solver {
private:
    bool we_are_white;
    int max_depth;
    SolverResult result;

    bool check_win(const Board& board, bool white_turn) const;
    void solve_recursive(const Board& board, bool white_turn, int depth);

public:
    Solver(bool we_are_white, int max_depth);
    bool solve(const Board& board, bool white_turn);
    vector<Move> get_solution() const;
    void clear_solution();
};


// Класс для работы с файлами и координатами
class FileManager {
public:
    static Board read_board(const string& filename);
    static void write_result(const string& filename, const Solver& solver);
    static pair<int, int> parse_coord(const string& s);
    static string format_coord(int x, int y);
    static string format_move(const Move& mv);
};

#endif // HEADER_H