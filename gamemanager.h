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

        static const int square_width;
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
};

#endif // GAMEMANAGER_H
