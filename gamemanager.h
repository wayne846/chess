#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <string>
#include <vector>

using namespace std;

class MainWindow;
class Piece;

class GameManager
{
    public:
        static const QColor light_square_color;
        static const QColor dark_square_color;
        static const QColor gray;
        static const QColor white;

        static const int basic_square_width;
        static int square_width;
        static float sizePercent;
        static int window_width;
        static MainWindow *window;
        static QGraphicsScene *scene;
        static QGraphicsRectItem* squares[8][8]; //[y][x]
        static Piece* pieceOnSquare[8][8]; //[y][x]
        static QPixmap* images[2][6];
        static string fen;
        static vector<Piece*> pieces[2];
        static int turn;

        static void generateChessBoard();

        static void pieces_erase(Piece* p);
        static QPoint kingPosition(int color);
        static bool isSquareBeAttacked(int color, int tx, int ty); // is color attack that square
        static bool hasLegalMove(int color);
        static void changTurn(int color);
        static void endGame(int result);
};

#endif // GAMEMANAGER_H
