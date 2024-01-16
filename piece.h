#ifndef PIECE_H
#define PIECE_H
#include <QGraphicsPixmapItem>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneEvent>

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

        Piece(int type, int color, int x, int y);
        int getColor();
        int getTyper();
        int getX();
        int getY();

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        int color;
        int type;
        int x;
        int y;
};

#endif // PIECE_H
