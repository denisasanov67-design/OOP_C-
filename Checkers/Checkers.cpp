#include "Header.h"


// Board
Board::Board() {
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            cells[y][x] = EMPTY;
}

// Копия доски
Board Board::clone() const {
    Board new_board;
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            new_board.cells[y][x] = cells[y][x];
    return new_board;
}

// Проверка, находятся ли координаты в пределах доски
bool Board::is_valid_pos(int x, int y) const {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// Возвращает тип фигуры
char Board::get_piece(int x, int y) const {
    if (!is_valid_pos(x, y)) return EMPTY;
    return cells[y][x];
}

// Вносит фигуры
void Board::set_piece(int x, int y, char piece) {
    if (is_valid_pos(x, y))  cells[y][x] = piece;
}

// Количество белых фиугур
int Board::count_white() const {
    int cnt = 0;
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (cells[y][x] == WHITE_PIECE || cells[y][x] == WHITE_KING) cnt++;
    return cnt;
}

// Количество черных фиугур
int Board::count_black() const {
    int cnt = 0;
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (cells[y][x] == BLACK_PIECE || cells[y][x] == BLACK_KING) cnt++;
    return cnt;
}

bool Board::is_white(char p) const {
    return p == WHITE_PIECE || p == WHITE_KING;
}
                                                    // определение цвета фигуры
bool Board::is_black(char p) const {
    return p == BLACK_PIECE || p == BLACK_KING;
}

// являются ли две фигуры врагами
bool Board::is_enemy(char p1, char p2) const {
    if (p1 == EMPTY || p2 == EMPTY) return false;
    return (is_white(p1) && is_black(p2)) || (is_black(p1) && is_white(p2));
}






// Реализация MoveGenerator 
MoveGenerator::MoveGenerator(const Board& b, bool white_turn) : board(&b), is_white_turn(white_turn) {}


// Рекурсивный поиск всех возможных цепочек взятий для одной шашки
void MoveGenerator::find_captures_recursive(int x, int y, char current_piece, 
    const vector<pair<int, int>>& current_path, // последовательность посещенных клеток
    const vector<pair<int, int>>& captures_so_far, // Список координат уже срубленных шашек в этой цепочке.
    int original_x, int original_y, // координаты начальной позиции 
    vector<Move>& result_moves) const // список найденных ходов
{

    bool is_king = (current_piece == WHITE_KING || current_piece == BLACK_KING);
    int dx[] = { 1, 1, -1, -1 };
    int dy[] = { 1, -1, 1, -1 }; // 4 направления по диагонали

    bool found_capture = false;

    for (int i = 0; i < 4; i++) {
        int dir_x = dx[i];
        int dir_y = dy[i];

        // алгоритм для дамы
        if (is_king) {
            int enemy_x = x + dir_x;
            int enemy_y = y + dir_y;

            // идем по диагонали пока не вышли за пределы поля 
            while (board->is_valid_pos(enemy_x, enemy_y)) {
                char piece = board->get_piece(enemy_x, enemy_y);

                if (piece != EMPTY) {
                    if (board->is_enemy(current_piece, piece)) { // онаружение врага 
                        bool already_captured = false;

                        // захвачен враг или нет за один ход 
                        for (size_t j = 0; j < captures_so_far.size(); j++) {
                            if (captures_so_far[j].first == enemy_x && captures_so_far[j].second == enemy_y) {
                                already_captured = true;
                                break;
                            }
                        }

                        // если враг не захвачен 
                        if (!already_captured) {
                            int land_x = enemy_x + dir_x;
                            int land_y = enemy_y + dir_y;

                            while (board->is_valid_pos(land_x, land_y) && board->get_piece(land_x, land_y) == EMPTY) {
                                found_capture = true; // взятие найдено 

                                vector<pair<int, int>> new_path = current_path;
                                new_path.push_back(make_pair(land_x, land_y)); // добавление новой позиции в путь

                                vector<pair<int, int>> new_captures = captures_so_far; 
                                new_captures.push_back(make_pair(enemy_x, enemy_y)); // добавление  срубленной шашки

                                Board next_board = board->clone();
                                next_board.set_piece(x, y, EMPTY); // удаляет шашку с исходной позиции
                                next_board.set_piece(enemy_x, enemy_y, EMPTY); // удаляет врага с исходной позиции 
                                next_board.set_piece(land_x, land_y, current_piece); // записываем текущую позицию фигуры 

                                find_captures_recursive(land_x, land_y, current_piece,
                                    new_path, new_captures, original_x, original_y, result_moves); // рекурсивно ищет 

                                land_x += dir_x; //проверяет следующее пустое поле за врагом 
                                land_y += dir_y;
                            }
                        }
                    }
                    break;
                }
                enemy_x += dir_x;
                enemy_y += dir_y;
            }
        }
        
        // алгоритм для простой шашки   
        else {
            int enemy_x = x + dir_x;
            int enemy_y = y + dir_y; // клетка с врагом 
            int land_x = x + 2 * dir_x;
            int land_y = y + 2 * dir_y; // клетка с приземлением 

            if (board->is_valid_pos(land_x, land_y)) {
                char enemy_piece = board->get_piece(enemy_x, enemy_y);
                char land_spot = board->get_piece(land_x, land_y);


                // определяет срублена ли фигура 
                if (board->is_enemy(current_piece, enemy_piece) && land_spot == EMPTY) {
                    bool already_captured = false;
                    for (size_t j = 0; j < captures_so_far.size(); j++) {
                        if (captures_so_far[j].first == enemy_x && captures_so_far[j].second == enemy_y) {
                            already_captured = true;
                            break;
                        }
                    }

                    if (!already_captured) {
                        found_capture = true;

                        vector<pair<int, int>> new_path = current_path;
                        new_path.push_back(make_pair(land_x, land_y));

                        vector<pair<int, int>> new_captures = captures_so_far;
                        new_captures.push_back(make_pair(enemy_x, enemy_y));

                        // проверяем превращение в даму 
                        char next_piece = current_piece;
                        if (current_piece == WHITE_PIECE && land_y == 0) next_piece = WHITE_KING;
                        if (current_piece == BLACK_PIECE && land_y == 7) next_piece = BLACK_KING;

                        Board next_board = board->clone();
                        next_board.set_piece(x, y, EMPTY);
                        next_board.set_piece(enemy_x, enemy_y, EMPTY);
                        next_board.set_piece(land_x, land_y, next_piece);

                        find_captures_recursive(land_x, land_y, next_piece,
                            new_path, new_captures, original_x, original_y, result_moves);
                    }
                }
            }
        }
    }

    // если не найдено продолжение взятия, но были срубленные шашки 
    if (!found_capture && !captures_so_far.empty()) {

        // сохраняем код в результат 
        Move m;
        m.start_x = original_x;
        m.start_y = original_y;
        m.path = current_path;
        m.captures = captures_so_far;
        result_moves.push_back(m);
    }
}

// Все легальные ходы 
vector<Move> MoveGenerator::get_all_moves() const {
    vector<Move> captures; // все взятия   
    vector<Move> quiet_moves; // тихие ходы 

    // поиск взятий 
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if ((x + y) % 2 == 0) continue; // пропускает белые клетки 

            char p = board->get_piece(x, y);
            bool is_my_piece = is_white_turn ? board->is_white(p) : board->is_black(p); // проверка на свою шашку 

            if (is_my_piece) {
                vector<Move> piece_captures;
                vector<pair<int, int>> initial_path;
                initial_path.push_back(make_pair(x, y)); // создаёт начальный путь с текущей позицией 

                find_captures_recursive(x, y, p, initial_path, vector<pair<int, int>>(), x, y, piece_captures);

                for (size_t j = 0; j < piece_captures.size(); j++) { // добавление всех ходов в общий список
                    captures.push_back(piece_captures[j]);
                }
            }
        }
    }

    if (!captures.empty()) return captures; // возвращаем только взятие, если есть хотябы одно взятие

    // поиск тихиз ходов 
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if ((x + y) % 2 == 0) continue;

            char p = board->get_piece(x, y);
            bool is_my_piece = is_white_turn ? board->is_white(p) : board->is_black(p);

            if (is_my_piece) {
                bool is_king = (p == WHITE_KING || p == BLACK_KING);

                // если простая шашка 
                if (!is_king) {
                    int dir_y = is_white_turn ? -1 : 1; // белые вверх, черные вниз 
                    int dx_vals[2] = { -1, 1 };

                    for (int k = 0; k < 2; k++) {
                        int dx = dx_vals[k];
                        int nx = x + dx; // два направления по диагонали 
                        int ny = y + dir_y;
                        if (board->is_valid_pos(nx, ny) && board->get_piece(nx, ny) == EMPTY) {
                            Move m;
                            m.start_x = x;
                            m.start_y = y;
                            m.path.push_back(make_pair(x, y)); //сохраняем код в тихие ходы 
                            m.path.push_back(make_pair(nx, ny));
                            quiet_moves.push_back(m);
                        }
                    }
                }

                // если дамка 
                else {
                    int dirs[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };
                    for (int d_idx = 0; d_idx < 4; d_idx++) {
                        int nx = x + dirs[d_idx][0];
                        int ny = y + dirs[d_idx][1];
                        while (board->is_valid_pos(nx, ny) && board->get_piece(nx, ny) == EMPTY) { // Двигается пока клетка в пределах доски и пустая
                            Move m;
                            m.start_x = x;   
                            m.start_y = y;
                            m.path.push_back(make_pair(x, y)); // на каждой пустой клеке создает ход 
                            m.path.push_back(make_pair(nx, ny));
                            quiet_moves.push_back(m);
                            nx += dirs[d_idx][0]; // продолжает двигаться дальше 
                            ny += dirs[d_idx][1];
                        }
                    }
                }
            }
        }
    }
    return quiet_moves;
}


// Solver
Solver::Solver(bool we_are_white, int max_depth) : we_are_white(we_are_white), max_depth(max_depth) {}


// определение победителя в текущей позиции 
bool Solver::check_win(const Board& board, bool white_turn) const {
    if (we_are_white && board.count_black() == 0) return true;
    if (!we_are_white && board.count_white() == 0) return true; // провека на  уничтожение всех фигру противника 

    MoveGenerator mg(board, !white_turn);
    bool enemy_stuck = mg.get_all_moves().empty(); // если список ходов пустой - True

    if (we_are_white && !white_turn && enemy_stuck) return true; // проверка на наличие ходов у противника 
    if (!we_are_white && white_turn && enemy_stuck) return true;

    return false;
}

// решатель - поиск в глубину 
void Solver::solve_recursive(const Board& board, bool white_turn, int depth) {

    // три условия победы 
    if (result.solved) return; // если решение найдено - выход     
    if (check_win(board, white_turn)) {
        result.solved = true; // победа - сворачиваемся 
        return;
    }
    if (depth >= max_depth) return; // если глубина больше заданной - прекращаем 

    // генерация легальных ходов 
    MoveGenerator mg(board, white_turn);
    vector<Move> moves = mg.get_all_moves(); // все возможные ходы 

    // перебор всех ходов 
    for (size_t i = 0; i < moves.size(); i++) { 
        const Move& mv = moves[i];

        Board next_board = board.clone(); // проверяем все ходы на симмулированной доске 
        next_board.set_piece(mv.start_x, mv.start_y, EMPTY); 

        for (size_t j = 0; j < mv.captures.size(); j++) {
            const pair<int, int>& cap = mv.captures[j];  
            next_board.set_piece(cap.first, cap.second, EMPTY); // удаление срубленных шашек 
        }

        char p = board.get_piece(mv.start_x, mv.start_y);
        int end_x = mv.end_x();
        int end_y = mv.end_y(); // определение конечной позиции 

        if (p == WHITE_PIECE && end_y == 0) p = WHITE_KING; // если шашка достигла противоположной линии - становиться дамой
        if (p == BLACK_PIECE && end_y == 7) p = BLACK_KING;
        next_board.set_piece(end_x, end_y, p);

        result.path.push_back(mv); // сохраняем текущий ход в путь  
        solve_recursive(next_board, !white_turn, depth + 1); // рек вызов для след игрока 
        if (result.solved) return;
        result.path.pop_back(); // отменяем ход и переходим к следующему (бэктрекинг)
    }
}

// Заупск поиска выйгрышной комбинации 
bool Solver::solve(const Board& board, bool white_turn) {
    result.solved = false;
    result.path.clear(); // очистка результата прошлого поиска 
    solve_recursive(board, white_turn, 0);
    return result.solved;
}

// Возвращает найденную последовательность ходов
vector<Move> Solver::get_solution() const { // 	Вектор ходов, ведущих к победе
    return result.path;
}

// очищает результат предыдущего поиска
void Solver::clear_solution() {
    result.solved = false;
    result.path.clear();
}



//FileManager
// 
// Преобразует шахматную нотацию во внутренние координаты 
pair<int, int> FileManager::parse_coord(const string& s) {
    int x = s[0] - 'A';
    int y = 8 - (s[1] - '0');
    return make_pair(x, y);
}

// чтение позиции из текстового файла   
Board FileManager::read_board(const string& filename) {
    ifstream in(filename);
    Board board;

    string token;
    int count;

    in >> token >> count; // читает первую строку например (White: 3)

    // чтение координат белых 
    for (int i = 0; i < count; i++) {
        string pos;
        in >> pos;
        bool is_king = false;
        if (pos[0] == 'M') {
            is_king = true; // проверяет на дамку 
            pos = pos.substr(1);
        }
        pair<int, int> p = parse_coord(pos);
        board.set_piece(p.first, p.second, is_king ? WHITE_KING : WHITE_PIECE);
    }

    in >> token >> count; 

    //чтение координат черных 
    for (int i = 0; i < count; i++) {
        string pos;
        in >> pos;
        bool is_king = false;
        if (pos[0] == 'M') {
            is_king = true;
            pos = pos.substr(1); // удаление префикса ""
        }
        pair<int, int> p = parse_coord(pos);
        board.set_piece(p.first, p.second, is_king ? BLACK_KING : BLACK_PIECE);
    }

    in.close();
    return board;
}


// Преобразует внутренние координаты в шахматную нотацию 
string FileManager::format_coord(int x, int y) {
    string s;
    s += (char)('A' + x);
    s += to_string(8 - y);
    return s;
}


// Преобразует объект Move в читаемую строку
string FileManager::format_move(const Move& mv) {
    string result;
    for (size_t i = 0; i < mv.path.size(); i++) {
        if (i > 0) result += " -> ";
        result += format_coord(mv.path[i].first, mv.path[i].second);
    }
    return result;
}

void FileManager::write_result(const string& filename, const Solver& solver) {
    ofstream out(filename);
    vector<Move> solution = solver.get_solution(); // запрос у решателя найденной последовательности ходов 

    // если решение найдено
    if (!solution.empty()) { 
        for (size_t i = 0; i < solution.size(); i++) {
            out << format_move(solution[i]) << endl;
        }
    }
    else {
        out << "NO WIN FOUND" << endl;
    }
    out.close();
}



