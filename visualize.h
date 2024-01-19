#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <QGraphicsItem>
#include <vector>

using namespace std;

class PromotionPiece;
class Piece;

class Visualize
{
    public:
        static void showLegalMove(vector<QPoint> v);
        static void hideLegalMove();

        static void showHighlightSquare(int x, int y);
        static void hideHighlightSquare();

        static void showPromotionList(Piece *p);

    private:
        static vector<QGraphicsEllipseItem*> legalMoveCircle;
        static QGraphicsRectItem *highlightSquare;
        static Piece *promotionPawn;
        static vector<PromotionPiece*> promotionPieces;
        static QGraphicsRectItem *promotionBackground;

        static void promotionClicked(int type);

    friend PromotionPiece;
};

#endif // VISUALIZE_H
