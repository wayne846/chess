#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <QGraphicsItem>
#include <vector>

using namespace std;

class Visualize
{
    public:
        static void showLegalMove(vector<QPoint> v);
        static void hideLegalMove();

        static void showHighlightSquare(int x, int y);
        static void hideHighlightSquare();

    private:
        static vector<QGraphicsEllipseItem*> legalMoveCircle;
        static QGraphicsRectItem *highlightSquare;
};

#endif // VISUALIZE_H
