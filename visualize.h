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

    private:
        static vector<QGraphicsEllipseItem*> legalMoveCircle;
};

#endif // VISUALIZE_H
