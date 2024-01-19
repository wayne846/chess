#ifndef PROMOTIONPIECE_H
#define PROMOTIONPIECE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class PromotionPiece : public QGraphicsPixmapItem{
    public:
        PromotionPiece(int type, int color);
        void setColor(int color);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);

    private:
        int type;
};

#endif // PROMOTIONPIECE_H
