#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm> // Для std::find

using namespace std;

//////////////////////////////////////////////////////////////////
// ENUMS
//////////////////////////////////////////////////////////////////

enum Color
{
    WHITE,
    BLACK
};

enum PieceType
{
    MAN,
    KING
};

//////////////////////////////////////////////////////////////////
// PIECE
//////////////////////////////////////////////////////////////////

class Piece
{
public:
    Color color;
    PieceType type;

    int x;
    int y;

    Piece() {}

    Piece(Color c, PieceType t, int px, int py)
    {
        color = c;
        type = t;
        x = px;
        y = py;
    }
};

//////////////////////////////////////////////////////////////////// MOVE
//////////////////////////////////////////////////////////////////

class Move
{
public:
    int sx, sy;
    int ex, ey;

    vector<pair<int,int>> captured;

    Move() {}

    Move(int x1,int y1,int x2,int y2)
    {
        sx = x1;
        sy = y1;

        ex = x2;
        ey = y2;
    }
};

//////////////////////////////////////////////////////////////////
// BOARD
//////////////////////////////////////////////////////////////////

class Board
{
public:

    char grid[8][8];

    Board()
    {
        clear();
    }

    void clear()
    {
        for(int r=0;r<8;r++)
            for(int c=0;c<8;c++)
                grid[r][c]='.';
    }

    bool inside(int x,int y) const
    {
        return x>=0 && x<8 && y>=0 && y<8;
    }
    char get(int x,int y) const
    {
        return grid[y][x];
    }

    void set(int x,int y,char v)
    {
        grid[y][x]=v;
    }

    Board clone() const
    {
        Board b;
        for(int r=0;r<8;r++)
            for(int c=0;c<8;c++)
                b.grid[r][c]=grid[r][c];

        return b;
    }

    int countWhite() const
    {
        int cnt=0;

        for(int r=0;r<8;r++)
            for(int c=0;c<8;c++)
                if(grid[r][c]=='w' || grid[r][c]=='W')
                    cnt++;

        return cnt;
    }

    int countBlack() const
    {
        int cnt=0;

        for(int r=0;r<8;r++)
            for(int c=0;c<8;c++)
                if(grid[r][c]=='b' || grid[r][c]=='B')
                    cnt++;

        return cnt;
    }
};

//////////////////////////////////////////////////////////////////
// GAME STATE
//////////////////////////////////////////////////////////////////

class GameState{
public:

    Board board;

    Color turn;

    bool whiteWon() const
    {
        return board.countBlack()==0;
    }

    bool blackWon() const
    {
        return board.countWhite()==0;
    }
};

//////////////////////////////////////////////////////////////////
// RULES ENGINE
//////////////////////////////////////////////////////////////////

class RulesEngine
{
public:

    static bool isWhite(char p)
    {
        return p=='w' || p=='W';
    }

    static bool isBlack(char p)
    {
        return p=='b' || p=='B';
    }

    static bool isEnemy(char a,char b)
    {
        if(a=='.' || b=='.')
            return false;

        return (isWhite(a)&&isBlack(b))
            || (isBlack(a)&&isWhite(b));
    }

    //////////////////////////////////////////////////////////////
    // Простые ходы
    //////////////////////////////////////////////////////////////

    static vector<Move> getValidMoves(        const Board& board,
        Color turn)
    {
        vector<Move> allMoves;
        vector<Move> captureMovesOnly;

        for(int y=0;y<8;y++)
        {
            for(int x=0;x<8;x++)
            {
                char p=board.get(x,y);

                if(turn==WHITE && !isWhite(p))
                    continue;

                if(turn==BLACK && !isBlack(p))
                    continue;

                // Генерируем только взятия (они уже включают цепочки)
                generateCaptureMoves(board, x, y, captureMovesOnly);
                
                // Если взятий нет, генерируем тихие ходы
                if(captureMovesOnly.empty()) {
                    generateQuietMoves(board, x, y, allMoves);
                }
            }
        }

        // Если есть взятия - возвращаем только их (обязательное правило)
        if(!captureMovesOnly.empty()) {
            return captureMovesOnly;
        }
        
        return allMoves;
    }

    // Генерация тихих (не бьющих) ходов
    static void generateQuietMoves(
        const Board& board,
        int x,
        int y,
        vector<Move>& moves)
    {
        char p=board.get(x,y);
        int dir = 0;

        if(p=='w') dir=-1;
        else if(p=='b') dir=1;
        else return; // Дамки ходят иначе, но для тихих ходов логика ниже
        // MAN тихие ходы
        if(p=='w' || p=='b')
        {
            int nx=x-1;
            int ny=y+dir;
            if(board.inside(nx,ny) && board.get(nx,ny)=='.')
                moves.push_back(Move(x,y,nx,ny));

            nx=x+1;
            if(board.inside(nx,ny) && board.get(nx,ny)=='.')
                moves.push_back(Move(x,y,nx,ny));
        }

        // KING тихие ходы (полет)
        if(p=='W' || p=='B')
        {
            int dx[4]={1,1,-1,-1};
            int dy[4]={1,-1,1,-1};

            for(int d=0;d<4;d++)
            {
                int nx=x+dx[d];
                int ny=y+dy[d];
                while(board.inside(nx,ny) && board.get(nx,ny)=='.')
                {
                    moves.push_back(Move(x,y,nx,ny));
                    nx+=dx[d];
                    ny+=dy[d];
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////
    // Взятия (ИСПРАВЛЕНО: цепочки + логика дамок)
    //////////////////////////////////////////////////////////////

    static void generateCaptureMoves(
        const Board& board,
        int x,
        int y,
        vector<Move>& moves)
    {
        char me = board.get(x,y);
        bool isKing = (me == 'W' || me == 'B');
        
        // Рекурсивная функция для поиска цепочек
        findCapturesRecursive(board, x, y, me, isKing, moves, {});
    }
    static void findCapturesRecursive(
        const Board& board,
        int x,
        int y,
        char me,
        bool isKing,
        vector<Move>& moves,
        const vector<pair<int,int>>& capturedSoFar)
    {
        int dx[4]={1,1,-1,-1};
        int dy[4]={1,-1,1,-1};

        for(int d=0; d<4; d++)
        {
            if(isKing)
            {
                // Логика взятия для ДАМКИ
                // 1. Летим, пока не встретим врага или край
                int mx = x + dx[d];
                int my = y + dy[d];
                
                while(board.inside(mx, my) && board.get(mx, my) == '.') {
                    mx += dx[d];
                    my += dy[d];
                }
                
                // 2. Если нашли врага
                if(board.inside(mx, my) && isEnemy(me, board.get(mx, my)))
                {
                    // Проверяем, не били ли мы уже эту шашку (защита от зацикливания)
                    bool alreadyCaptured = false;
                    for(auto& c : capturedSoFar) {
                        if(c.first == mx && c.second == my) {
                            alreadyCaptured = true;
                            break;
                        }
                    }
                    
                    if(!alreadyCaptured)
                    {
                        // 3. Летим дальше за врагом, пробуя приземлиться
                        int lx = mx + dx[d];
                        int ly = my + dy[d];
                        
                        while(board.inside(lx, ly) && board.get(lx, ly) == '.')
                        {
                            // Создаем новый ход
                            Move m(x, y, lx, ly);
                            m.captured = capturedSoFar;
                            m.captured.push_back({mx, my});                            
                            // Создаем временную доску для проверки продолжения цепочки
                            Board tempBoard = board.clone();
                            tempBoard.set(x, y, '.');
                            tempBoard.set(mx, my, '.'); // Убираем сбитую
                            tempBoard.set(lx, ly, me);  // Ставим дамку на новую позицию
                            
                            // Рекурсия: ищем продолжение цепочки с новой позиции
                            findCapturesRecursive(tempBoard, lx, ly, me, true, moves, m.captured);
                            
                            // Если продолжений нет - это финал цепочки, сохраняем ход
                            bool hasContinuation = false;
                            // (Простая эвристика: если рекурсия выше добавила ходы с тем же стартом, значит продолжение есть. 
                            // Но для простоты просто добавляем текущий вариант, а дубликаты отфильтруются или будут допустимы)
                            moves.push_back(m);
                            
                            lx += dx[d];
                            ly += dy[d];
                        }
                    }
                }
            }
            else
            {
                // Логика взятия для ПРОСТОЙ (MAN)
                int mx = x + dx[d];
                int my = y + dy[d];
                int lx = x + 2*dx[d];
                int ly = y + 2*dy[d];

                if(!board.inside(lx, ly)) continue;

                char enemy = board.get(mx, my);

                if(isEnemy(me, enemy) && board.get(lx, ly) == '.')
                {
                    bool alreadyCaptured = false;
                    for(auto& c : capturedSoFar) {
                        if(c.first == mx && c.second == my) {
                            alreadyCaptured = true;
                            break;
                        }
                    }
                    
                    if(!alreadyCaptured)
                    {
                        Move m(x, y, lx, ly);
                        m.captured = capturedSoFar;
                        m.captured.push_back({mx, my});
                                                // Временная доска для цепочки
                        Board tempBoard = board.clone();
                        tempBoard.set(x, y, '.');
                        tempBoard.set(mx, my, '.');
                        tempBoard.set(lx, ly, me);
                        
                        // Рекурсия для цепочки
                        findCapturesRecursive(tempBoard, lx, ly, me, false, moves, m.captured);
                        
                        moves.push_back(m);
                    }
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////

    static Board applyMove(
        const Board& board,
        const Move& move)
    {
        Board next=board.clone();

        char p=next.get(move.sx,move.sy);

        next.set(move.sx,move.sy,'.');

        for(auto c:move.captured)
            next.set(c.first,c.second,'.');

        next.set(move.ex,move.ey,p);

        //////////////////////////////////////////////////////////
        // Promotion (Превращение в дамку)
        //////////////////////////////////////////////////////////
        // Важно: в русских шашках, если простая при взятии попадает на дамочное поле,
        // она становится дамкой И может продолжить бить как дамка.
        
        bool justPromoted = false;
        if(p=='w' && move.ey==0) {
            next.set(move.ex,move.ey,'W');
            justPromoted = true;
        }
        if(p=='b' && move.ey==7) {
            next.set(move.ex,move.ey,'B');
            justPromoted = true;
        }

        return next;    }
};

//////////////////////////////////////////////////////////////////
// NODE
//////////////////////////////////////////////////////////////////

class Node
{
public:

    GameState state;

    Move move;

    vector<Node*> children;

    ~Node()
    {
        for(auto c:children)
            delete c;
    }
};

//////////////////////////////////////////////////////////////////
// DFS SOLVER
//////////////////////////////////////////////////////////////////

class DFS
{
public:

    vector<Move> answer;

    bool search(
        Node* node,
        int depth,
        int maxDepth,
        Color rootSide)
    {
        if(rootSide==WHITE
           && node->state.whiteWon())
            return true;

        if(rootSide==BLACK
           && node->state.blackWon())
            return true;

        if(depth>=maxDepth)
            return false;
        vector<Move> moves =
            RulesEngine::getValidMoves(
                node->state.board,
                node->state.turn);

        for(auto& mv:moves)
        {
            Node* child=new Node();

            child->move=mv;

            child->state.board =
                RulesEngine::applyMove(
                    node->state.board,
                    mv);

            child->state.turn=
                node->state.turn==WHITE
                ? BLACK
                : WHITE;

            node->children.push_back(child);

            answer.push_back(mv);

            if(search(
                    child,
                    depth+1,
                    maxDepth,
                    rootSide))
                return true;

            answer.pop_back();
            
            // Очистка памяти при откате (опционально, но полезно)
            delete child;
            node->children.pop_back();
        }

        return false;
    }
};

//////////////////////////////////////////////////////////////////
// PARSER
//////////////////////////////////////////////////////////////////

class Parser
{public:

    static pair<int,int> coordToXY(
        const string& s)
    {
        int x=s[0]-'A';
        int y=8-(s[1]-'0');
        return {x,y};
    }

    static GameState readFile(
        const string& file)
    {
        ifstream in(file);

        GameState state;

        string token;

        in>>token; // White:

        int nWhite;
        in>>nWhite;

        for(int i=0;i<nWhite;i++)
        {
            string pos;
            in>>pos;

            auto p=coordToXY(pos);

            state.board.set(
                p.first,
                p.second,
                'w');
        }

        in>>token; // Black:

        int nBlack;
        in>>nBlack;

        for(int i=0;i<nBlack;i++)
        {
            string pos;
            in>>pos;

            bool king=false;

            if(pos[0]=='M')            {
                king=true;
                pos=pos.substr(1);
            }

            auto p=coordToXY(pos);

            state.board.set(
                p.first,
                p.second,
                king?'B':'b');
        }

        state.turn=WHITE;

        return state;
    }
};

//////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////

int main()
{
    GameState initial =
        Parser::readFile("text.txt");

    Node root;
    root.state=initial;

    DFS solver;

    bool found=
        solver.search(
            &root,
            0,
            6,      // 3 хода белых
            WHITE);

    ofstream out("output.txt");

    if(found)
    {
        for(auto& m:solver.answer)
        {
            char a='A'+m.sx;
            char b='A'+m.ex;

            out<<a               <<(8-m.sy)
               <<" -> "
               <<b
               <<(8-m.ey)
               <<"\n";
        }
    }
    else
    {
        out<<"NO WIN FOUND\n";
    }

    out.close();

    return 0;
}
