#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Константы для цветов
const char WHITE_PIECE = 'w';
const char WHITE_KING  = 'W';
const char BLACK_PIECE = 'b';
const char BLACK_KING  = 'B';
const char EMPTY       = '.';

// Структура для описания хода
struct Move {
    int start_x, start_y; // Откуда
    int end_x, end_y;     // Куда
    vector<pair<int, int>> captures; // Список срубленных шашек
};

// Структура для доски
struct Board {
    char cells[8][8]; // Игровое поле

    // Конструктор: очищает доску
    Board() {
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
                cells[y][x] = EMPTY;
    }

    // Копирование доски (нужно для рекурсии)
    Board clone() const {
        Board new_board;
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
                new_board.cells[y][x] = cells[y][x];
        return new_board;
    }

    // Проверка, находится ли клетка в пределах доски
    bool is_valid_pos(int x, int y) const {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    // Подсчет оставшихся шашек
    int count_white() const {
        int cnt = 0;
        for (int y = 0; y < 8; y++)            for (int x = 0; x < 8; x++)
                if (cells[y][x] == WHITE_PIECE || cells[y][x] == WHITE_KING) cnt++;
        return cnt;
    }

    int count_black() const {
        int cnt = 0;
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
                if (cells[y][x] == BLACK_PIECE || cells[y][x] == BLACK_KING) cnt++;
        return cnt;
    }
};

///////////////////////////////////////////////////////////////
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (Логика)
///////////////////////////////////////////////////////////////

// Является ли шашка белой?
bool is_white(char p) { return p == WHITE_PIECE || p == WHITE_KING; }

// Является ли шашка черной?
bool is_black(char p) { return p == BLACK_PIECE || p == BLACK_KING; }

// Являются ли две шашки врагами?
bool is_enemy(char p1, char p2) {
    if (p1 == EMPTY || p2 == EMPTY) return false;
    return (is_white(p1) && is_black(p2)) || (is_black(p1) && is_white(p2));
}

// Получить шашку по координатам
char get_piece(const Board& b, int x, int y) {
    if (!b.is_valid_pos(x, y)) return EMPTY;
    return b.cells[y][x];
}

///////////////////////////////////////////////////////////////
// ГЕНЕРАЦИЯ ХОДОВ
///////////////////////////////////////////////////////////////

// Рекурсивная функция для поиска цепочек взятий
void find_captures(const Board& board, int x, int y, char current_piece, 
                   const vector<pair<int, int>>& path_so_far, 
                   vector<Move>& result_moves) {
    
    bool is_king = (current_piece == WHITE_KING || current_piece == BLACK_KING);
    
    // 4 направления диагонали: (1,1), (1,-1), (-1,1), (-1,-1)
    int dx[] = {1, 1, -1, -1};
    int dy[] = {1, -1, 1, -1};
    for (int i = 0; i < 4; i++) {
        int dir_x = dx[i];
        int dir_y = dy[i];

        if (is_king) {
            // ЛОГИКА ДЛЯ ДАМКИ (летит через всё поле)
            int enemy_x = x + dir_x;
            int enemy_y = y + dir_y;

            // 1. Летим, пока не встретим шашку или край
            while (board.is_valid_pos(enemy_x, enemy_y) && get_piece(board, enemy_x, enemy_y) == EMPTY) {
                enemy_x += dir_x;
                enemy_y += dir_y;
            }

            // 2. Если встретили врага
            if (board.is_valid_pos(enemy_x, enemy_y) && is_enemy(current_piece, get_piece(board, enemy_x, enemy_y))) {
                
                // Проверка: не били ли мы уже эту шашку в этой цепочке?
                bool already_captured = false;
                for (auto p : path_so_far) {
                    if (p.first == enemy_x && p.second == enemy_y) already_captured = true;
                }

                if (!already_captured) {
                    // 3. Ищем, куда приземлиться ЗА врагом
                    int land_x = enemy_x + dir_x;
                    int land_y = enemy_y + dir_y;

                    while (board.is_valid_pos(land_x, land_y) && get_piece(board, land_x, land_y) == EMPTY) {
                        // Формируем ход
                        Move m;
                        m.start_x = x; m.start_y = y;
                        m.end_x = land_x; m.end_y = land_y;
                        m.captures = path_so_far;
                        m.captures.push_back({enemy_x, enemy_y});

                        // Создаем временную доску для проверки продолжения цепочки
                        Board next_board = board.clone();
                        next_board.cells[y][x] = EMPTY; // Убрали шашку со старта
                        next_board.cells[enemy_y][enemy_x] = EMPTY; // Срубили врага
                        next_board.cells[land_y][land_x] = current_piece; // Поставили на новую клетку

                        // Рекурсивно ищем продолжение взятия
                        find_captures(next_board, land_x, land_y, current_piece, m.captures, result_moves);
                        
                        // Добавляем этот вариант в список (это конец цепочки или промежуточный шаг)
                        result_moves.push_back(m);
                        land_x += dir_x;
                        land_y += dir_y;
                    }
                }
            }
        } else {
            // ЛОГИКА ДЛЯ ПРОСТОЙ ШАШКИ (бьет только через 1 клетку)
            int enemy_x = x + dir_x;
            int enemy_y = y + dir_y;
            int land_x = x + 2 * dir_x;
            int land_y = y + 2 * dir_y;

            if (board.is_valid_pos(land_x, land_y)) {
                char enemy_piece = get_piece(board, enemy_x, enemy_y);
                char land_spot = get_piece(board, land_x, land_y);

                if (is_enemy(current_piece, enemy_piece) && land_spot == EMPTY) {
                    
                    // Проверка на повторное взятие
                    bool already_captured = false;
                    for (auto p : path_so_far) {
                        if (p.first == enemy_x && p.second == enemy_y) already_captured = true;
                    }

                    if (!already_captured) {
                        Move m;
                        m.start_x = x; m.start_y = y;
                        m.end_x = land_x; m.end_y = land_y;
                        m.captures = path_so_far;
                        m.captures.push_back({enemy_x, enemy_y});

                        // Превращение в дамку?
                        char next_piece = current_piece;
                        if (current_piece == WHITE_PIECE && land_y == 0) next_piece = WHITE_KING;
                        if (current_piece == BLACK_PIECE && land_y == 7) next_piece = BLACK_KING;

                        // Временная доска
                        Board next_board = board.clone();
                        next_board.cells[y][x] = EMPTY;
                        next_board.cells[enemy_y][enemy_x] = EMPTY;
                        next_board.cells[land_y][land_x] = next_piece;

                        // Рекурсия
                        find_captures(next_board, land_x, land_y, next_piece, m.captures, result_moves);
                        result_moves.push_back(m);
                    }
                }
            }
        }
    }}

// Получить все допустимые ходы для игрока
vector<Move> get_all_moves(const Board& board, bool is_white_turn) {
    vector<Move> captures;
    vector<Move> quiet_moves;

    // 1. Сначала ищем ВСЕ возможные взятия на доске
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            char p = board.cells[y][x];
            bool is_my_piece = is_white_turn ? is_white(p) : is_black(p);

            if (is_my_piece) {
                vector<Move> piece_captures;
                find_captures(board, x, y, p, {}, piece_captures);
                for (auto m : piece_captures) captures.push_back(m);
            }
        }
    }

    // В шашках если есть взятие, тихие ходы запрещены
    if (!captures.empty()) return captures;

    // 2. Если взятий нет, ищем тихие ходы
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            char p = board.cells[y][x];
            bool is_my_piece = is_white_turn ? is_white(p) : is_black(p);
            
            if (is_my_piece) {
                bool is_king = (p == WHITE_KING || p == BLACK_KING);
                
                if (!is_king) {
                    // Обычная шашка ходит на 1 клетку вперед
                    int dir_y = is_white_turn ? -1 : 1;
                    for (int dx : {-1, 1}) {
                        int nx = x + dx;
                        int ny = y + dir_y;
                        if (board.is_valid_pos(nx, ny) && get_piece(board, nx, ny) == EMPTY) {
                            Move m; m.start_x=x; m.start_y=y; m.end_x=nx; m.end_y=ny;
                            quiet_moves.push_back(m);
                        }
                    }
                } else {
                    // Дамка ходит на любое свободное поле по диагонали
                    int dirs[4][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
                    for (auto& d : dirs) {
                        int nx = x + d[0];
                        int ny = y + d[1];                        while (board.is_valid_pos(nx, ny) && get_piece(board, nx, ny) == EMPTY) {
                            Move m; m.start_x=x; m.start_y=y; m.end_x=nx; m.end_y=ny;
                            quiet_moves.push_back(m);
                            nx += d[0]; ny += d[1];
                        }
                    }
                }
            }
        }
    }
    return quiet_moves;
}

///////////////////////////////////////////////////////////////
// РЕШАТЕЛЬ (DFS)
///////////////////////////////////////////////////////////////

struct SolverResult {
    bool solved = false;
    vector<Move> path;
};

void solve(const Board& board, bool white_turn, int depth, int max_depth, 
           bool we_are_white, SolverResult& result) {
    
    // Если уже нашли решение, выходим
    if (result.solved) return;

    // Проверка победы
    bool white_wins = (board.count_black() == 0);
    bool black_wins = (board.count_white() == 0);
    
    // Условие победы для нас:
    // 1. Мы съели всех врагов
    // 2. У врагов нет ходов (блокировка)
    bool enemy_stuck = get_all_moves(board, !white_turn).empty(); 
    
    bool we_won = false;
    if (we_are_white && (white_wins || (!white_turn && enemy_stuck))) we_won = true;
    if (!we_are_white && (black_wins || (white_turn && enemy_stuck))) we_won = true;

    if (we_won) {
        result.solved = true;
        return;
    }

    // Если глубина исчерпана
    if (depth >= max_depth) return;

    // Генерируем ходы    vector<Move> moves = get_all_moves(board, white_turn);

    // Перебираем каждый ход
    for (const auto& mv : moves) {
        // Применяем ход
        Board next_board = board.clone();
        next_board.cells[mv.start_y][mv.start_x] = EMPTY;
        
        // Убираем срубленные
        for (auto cap : mv.captures) {
            next_board.cells[cap.second][cap.first] = EMPTY;
        }

        // Ставим шашку на новое место (с учетом превращения в дамку)
        char p = board.cells[mv.start_y][mv.start_x];
        if (p == WHITE_PIECE && mv.end_y == 0) p = WHITE_KING;
        if (p == BLACK_PIECE && mv.end_y == 7) p = BLACK_KING;
        next_board.cells[mv.end_y][mv.end_x] = p;

        // Добавляем ход в путь
        result.path.push_back(mv);

        // Рекурсивный вызов
        solve(next_board, !white_turn, depth + 1, max_depth, we_are_white, result);

        // Если решение найдено внутри рекурсии, выходим
        if (result.solved) return;

        // Откат (backtracking)
        result.path.pop_back();
    }
}

///////////////////////////////////////////////////////////////
// ВВОД / ВЫВОД
///////////////////////////////////////////////////////////////

pair<int, int> parse_coord(string s) {
    int x = s[0] - 'A';          // 'A' -> 0, 'B' -> 1
    int y = 8 - (s[1] - '0');    // '8' -> 0, '1' -> 7
    return {x, y};
}

string format_coord(int x, int y) {
    string s;
    s += (char)('A' + x);
    s += to_string(8 - y);
    return s;
}
int main() {
    // Чтение из файла
    ifstream in("input.txt");
    if (!in) {
        cerr << "Error: input.txt not found!" << endl;
        return 1;
    }

    Board board;
    string token;
    int count;

    // Читаем белых
    in >> token >> count; // token = "White:"
    for (int i = 0; i < count; i++) {
        string pos; in >> pos;
        auto p = parse_coord(pos);
        board.cells[p.second][p.first] = WHITE_PIECE;
    }

    // Читаем черных
    in >> token >> count; // token = "Black:"
    for (int i = 0; i < count; i++) {
        string pos; in >> pos;
        bool is_king = false;
        if (pos[0] == 'M') { // Формат ME1 -> дамка на E1
            is_king = true;
            pos = pos.substr(1);
        }
        auto p = parse_coord(pos);
        board.cells[p.second][p.first] = is_king ? BLACK_KING : BLACK_PIECE;
    }
    in.close();

    // Решение
    // Ищем выигрыш белых за 6 полуходов (3 полных хода)
    SolverResult result;
    solve(board, true, 0, 6, true, result); // true = ход белых, we_are_white = true

    // Запись в файл
    ofstream out("text.txt");
    if (result.solved) {
        for (const auto& mv : result.path) {
            out << format_coord(mv.start_x, mv.start_y) 
                << " -> " 
                << format_coord(mv.end_x, mv.end_y) << endl;
        }
    } else {
        out << "NO WIN FOUND" << endl;
    }    out.close();

    return 0;
}
