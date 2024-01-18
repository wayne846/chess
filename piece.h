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

        static const vector<vector<QPoint>> moveOffset;

        Piece(int type, int color, int x, int y);
        bool edgeCheck(int tx, int ty);
        int getColor();
        int getType();
        int getX();
        int getY();
        void captured();

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

        bool hasFirstMove;

        void movetoSquare(int x, int y);
        void findLegalMove();
        void slideMove();
        void oneBlockMove();
};

#endif // PIECE_H
