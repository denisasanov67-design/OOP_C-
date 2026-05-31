
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

enum Color { WHITE, BLACK };

// игровое поле 8х8
class Board {
public:
    char grid[8][8];

    Board() { clear(); }

    void clear() {
        for(int r=0;r<8;r++)
            for(int c=0;c<8;c++)
                grid[r][c]='.';
    }

    bool inside(int x,int y) const { return x>=0 && x<8 && y>=0 && y<8; }
    char get(int x,int y) const { return grid[y][x]; }
    void set(int x,int y,char v) { grid[y][x]=v; }

    Board clone() const {
        Board b;
        for(int r=0;r<8;r++)
            for(int c=0;c<8;c++)
                b.grid[r][c]=grid[r][c];
        return b;
    }

    int countWhite() const {
        int cnt=0;
        for(int r=0;r<8;r++)
            for(int c=0;c<8;c++)
                if(grid[r][c]=='w' || grid[r][c]=='W') cnt++;
        return cnt;
    }

    int countBlack() const {
        int cnt=0;
        for(int r=0;r<8;r++)
            for(int c=0;c<8;c++)
                if(grid[r][c]=='b' || grid[r][c]=='B') cnt++;
        return cnt;
    }
};


// Хранение полного состояния партии 
class GameState {
public:
    Board board;
    Color turn;
};

// Описание одного хода фигуры 
class Move {
public:
    int sx, sy, ex, ey;
    vector<pair<int,int» captured;

    Move(int x1,int y1,int x2,int y2) : sx(x1), sy(y1), ex(x2), ey(y2) {}
};


// Все правила игры 
class RulesEngine {
public:
    static bool isWhite(char p) { return p=='w' || p=='W'; }
    static bool isBlack(char p) { return p=='b' || p=='B'; }
    static bool isEnemy(char a, char b) {
        if(a=='.' || b=='.') return false;
        return (isWhite(a)&&isBlack(b)) || (isBlack(a)&&isWhite(b));
    }

    static vector<Move> getValidMoves(const Board& board, Color turn) {
        vector<Move> allCaptures;
        vector<Move> quietMoves;

        // 1. Собираем все возможные взятия со всех шашек
        for(int y=0; y<8; ++y) {
            for(int x=0; x<8; ++x) {
                char p = board.get(x,y);
                bool isMyPiece = (turn==WHITE && isWhite(p)) || (turn==BLACK && isBlack(p));
                if(!isMyPiece) continue;

                vector<Move> pieceCaps;
                findCaptures(board, x, y, p, {}, pieceCaps);
                for(auto& m : pieceCaps) allCaptures.push_back(m);
            }
        }

        // Если есть хоть одно взятие - возвращаем только их (обязательное правило)
        if(!allCaptures.empty()) return allCaptures;

        // 2. Иначе собираем тихие ходы
        for(int y=0; y<8; ++y) {
            for(int x=0; x<8; ++x) {
                char p = board.get(x,y);
                bool isMyPiece = (turn==WHITE && isWhite(p)) || (turn==BLACK && isBlack(p));
                if(!isMyPiece) continue;
                findQuiet(board, x, y, p, quietMoves);
            }
        }
        return quietMoves;
    }

private:
    static void findCapt


ures(const Board& board, int x, int y, char piece,
                             const vector<pair<int,int»& capturedSoFar,
                             vector<Move>& result) {
        bool isKing = (piece == 'W' || piece == 'B');
        int dirs[4][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};

        for(auto& d : dirs) {
            if(isKing) {
                // Логика для ДАМКИ (летит пока не встретит врага)
                int mx = x + d[0], my = y + d[1];
                while(board.inside(mx, my) && board.get(mx, my) == '.') {
                    mx += d[0]; my += d[1];
                }
                
                // Если встретили врага
                if(board.inside(mx, my) && isEnemy(piece, board.get(mx, my))) {
                    bool already = false;
                    for(auto& c : capturedSoFar)
                        if(c.first==mx && c.second==my) { already=true; break; }
                    
                    if(!already) {
                        // Ищем куда приземлиться за врагом
                        int lx = mx + d[0], ly = my + d[1];
                        while(board.inside(lx, ly) && board.get(lx, ly) == '.') {
                            Move m(x, y, lx, ly);
                            m.captured = capturedSoFar;
                            m.captured.push_back({mx, my});

                            // Создаем виртуальную доску для проверки продолжения цепочки
                            Board nextBoard = board.clone();
                            nextBoard.set(x, y, '.');
                            nextBoard.set(mx, my, '.');
                            nextBoard.set(lx, ly, piece);

                            // Рекурсивно ищем продолжение взятия
                            findCaptures(nextBoard, lx, ly, piece, m.captured, result);
                            
                            // Добавляем текущий вариант (даже если рекурсия выше ничего не нашла, это конец цепочки)
                            result.push_back(m);

                            lx += d[0]; ly += d[1];
                        }
                    }
                }
            } else {
                // Логика для ПРОСТОЙ (прыжок через 1 клетку)
                int mx = x + d[0], my = y + d[1];
                int lx = x + 2*d[0], ly = y + 2*d[1];
                
                if(board.inside(lx, ly) && isEnemy(piece, board.get(mx, my)) && board.get(lx, ly)=='.') {
                    bool already = false;
                    for(auto& c : capturedSoFar)
                        if(c.first==mx && c.second==my) { already=true; break; }
                    
                    if(!already) {
                        Move m(x, y, lx, ly);
                        m.captured = capturedSoFar;
                        m.captured.push_back({mx, my});

                        // Проверка на превращение в дамку
                        char nextPiece = piece;
                        if(piece=='w' && ly==0) nextPiece='W';
                        if(piece=='b' && ly==7) nextPiece='B';

                        Board nextBoard = board.clone();
                        nextBoard.set(x, y, '.');
                        nextBoard.set(mx, my, '.');
                        nextBoard.set(lx, ly, nextPiece);

                        // Рекурсия 
                        findCaptures(nextBoard, lx, ly, nextPiece, m.captured, result);
                        result.push_back(m);
                    }
                }
            }
        }
    }

    // генерация всех обычных ходов фигуры 
    static void findQuiet(const Board& board, int x, int y, char p, vector<Move>& result) {
        bool isKing = (p=='W' || p=='B');
        if(!isKing) {
            int dy = (p=='w') ? -1 : 1;
            for(int dx : {-1, 1}) {
                int nx = x+dx, ny = y+dy;
                if(board.inside(nx,ny) && board.get(nx,ny)=='.')
                    result.push_back(Move(x,y,nx,ny));
            }
        } else {


int dirs[4][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
            for(auto& d : dirs) {
                int nx = x+d[0], ny = y+d[1];
                while(board.inside(nx,ny) && board.get(nx,ny)=='.') {
                    result.push_back(Move(x,y,nx,ny));
                    nx += d[0]; ny += d[1];
                }
            }
        }
    }

public:
    // применение хода к позиции и возвращение новой доски 
    static Board applyMove(const Board& board, const Move& move) {
        Board next = board.clone();
        char p = next.get(move.sx, move.sy);
        next.set(move.sx, move.sy, '.');

        for(auto& c : move.captured)
            next.set(c.first, c.second, '.');

        next.set(move.ex, move.ey, p);

        // Превращение в дамку
        if(p=='w' && move.ey==0) next.set(move.ex, move.ey, 'W');
        if(p=='b' && move.ey==7) next.set(move.ex, move.ey, 'B');

        return next;
    }
};


// загрузка позиции из файла 
class Parser {
public:
    // преобразование координат 
    static pair<int,int> coordToXY(const string& s) {
        int x = s[0] - 'A';
        int y = 8 - (s[1] - '0');
        return {x, y};
    }

    static GameState readFile(const string& filename) {
        ifstream in(filename);
        if(!in) {
            cerr « "Error: Cannot open " « filename « endl;
            exit(1);
        }

        GameState state;
        string token;
        int count;

        in » token » count; // White: N
        for(int i=0; i<count; ++i) {
            string pos; in » pos;
            auto p = coordToXY(pos);
            state.board.set(p.first, p.second, 'w');
        }

        in » token » count; // Black: N
        for(int i=0; i<count; ++i) {
            string pos; in » pos;
            bool king = false;
            if(pos[0]=='M') { king=true; pos=pos.substr(1); }
            auto p = coordToXY(pos);
            state.board.set(p.first, p.second, king?'B':'b');
        }

        state.turn = WHITE;
        return state;
    }
};

// поиск выйгрышной последовательности ходов 
class Solver {
public:
    vector<Move> path;
    bool solved = false;

    // рекурсивный поиск выйгрышной последовательности 
    void search(const Board& board, Color turn, int depth, int maxDepth, Color rootSide) {
        if(solved) return;

        bool whiteWin = board.countBlack() == 0;
        bool blackWin = board.countWhite() == 0;
        
        // Победа по съедению или по блоку (у противника нет ходов)
        if((rootSide==WHITE && whiteWin) || (rootSide==BLACK && blackWin) ||
           RulesEngine::getValidMoves(board, turn).empty()) {
            solved = true;
            return;
        }

        if(depth >= maxDepth) return;

        vector<Move> moves = RulesEngine::getValidMoves(board, turn);
        for(auto& mv : moves) {
            path.push_back(mv);
            Board nextBoard = RulesEngine::applyMove(board, mv);
            search(nextBoard, turn==WHITE?BLACK:WHITE, depth+1, maxDepth, rootSide);
            if(solved) return;
            path.pop_back();
        }
    }
};


int main() {
    GameState initial = Parser::readFile("input.txt");

    Solver solver;
    // 6 полуходов = 3 полных хода.
    solver.search(initial.board, initial.turn, 0, 6, WHITE);

    ofstream out("output.txt");
    if(solver.solved) {
        for(auto& m : solver.path) {
            out « (char)('A'+m.sx) « (8-m.sy) 
                « " -> " 
                « (char)('A'+m.ex) « (8-m.ey) « "\n";
        }
    } else {
        out « "NO WIN FOUND\n";
    }
    out.close();

    return 0;
}

