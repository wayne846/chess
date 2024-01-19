#include "promotionpiece.h"
#include "gamemanager.h"
#include "visualize.h"

PromotionPiece::PromotionPiece(int type, int color){
    this->type = type;
    this->setPixmap(GameManager::images[color][type]->scaled(QSize(GameManager::square_width, GameManager::square_width)));
    this->setZValue(2);
    GameManager::scene->addItem(this);
}

void PromotionPiece::setColor(int color){
    this->setPixmap(GameManager::images[color][type]->scaled(QSize(GameManager::square_width, GameManager::square_width)));
}

void PromotionPiece::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Visualize::promotionClicked(type);
}
