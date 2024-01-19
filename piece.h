#ifndef PIECE_H
#define PIECE_H
#include <QGraphicsPixmapItem>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneEvent>
#include <vector>

using namespace std;

class Piece : public QGraphicsPixmapItem{
    public:
        static const int pawn;
        static const int bishop;
        static const int knight;
        static const int rook;
        static const int queen;
        static const int king;

        static const int white;
        static const int black;

        static const int promotionTurn;

        static const vector<vector<QPoint>> moveOffset;

        Piece(int type, int color, int x, int y);
        bool edgeCheck(int tx, int ty);
        int getColor();
        int getType();
        int getX();
        int getY();
        void captured();
        int oppositeColor();

        //for pawn
        void addEnPassant(int c);
        void clearEnPassant();

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        int color;
        int type;
        int x;
        int y;

        vector<QPoint> legalMove;

        //for pawn
        vector<int> enPassant; //x position

        bool hasFirstMove;

        void move(int tx, int ty); //all thing about move
        void movetoSquare(int tx, int ty); //just move, will change GameManager::pieceOnSquare
        void findLegalMove();

        void slideMove(); //bishop rook queen
        void oneBlockMove(); //pawn knight king
};

#endif // PIECE_H
